
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                              BATTERY FUNCTIONS                              //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
  Initialize the battery missions (if needed)
*/
export function initRobBatt() {
    global g('simConf');

    if (!g->enableBattMiss) {
        logWarn('[BATT] Battery missions are disabled (sim config)');
        return;
    }
    else {
        // g->battPntStates = {}; // states of battery points array with : 0 = free and 1 = reserved

        //check that we have correct charger points
        switch(g->battPntStrategy) {
                // indivudual : 1 batt charger per robot
            case('individual');
                // check existence of charger named "batt[idrob]"
                i = for(1,g->nRob) {
                    // retrieve all 'charger' type points from model layer
                    battPnts = getDestinationTypePoints('charger',getRobModel(i));
                    battPnt = findElt(battPnts,sprintf("batt%i",i));
                    if(isempty(battPnt)) {
                        displayError(sprintf('[BATT- individual] No charger point "batt%i" found for rob %i!',i,i));
                        logError('Check the battery strategy or the circuit.');
                        return;
                    }
                }
                // collective : at least 1 batt charger per model -> robots choose the closest free charger
            case('collective');
                // check existence of at least 1 charger per layer
                i = for(1,g->nRob) {
                    // retrieve all 'charger' type points from model layer
                    battPnts = getDestinationTypePoints('charger',getRobModel(i));
                    if(isempty(battPnts)) {
                        displayError(sprintf('[BATT- collective] No charger points found for layer "%s"',getRobModel(i)));
                        logError('Check the battery strategy or the circuit.');
                        return;
                    }
                }
            case('taxi','taxiOpti');
                break;
                //do nothing

            //case('custom');
                //add your custom configuration here

            default;
                displayError(sprintf('[initRobBatt] Unknown battery strategy "%s"',g->battPntStrategy));
                return;
        }
    }

    initChargeParam(); // initializes the battery charge parameters
}


/*
    Initializes the battery charge parameters
*/
export function initChargeParam() {
    clearGlobal('simBatt');

    global g('simConf');
    global b('simBatt');

    b->robTPPL = new('long',0);
    b->robLTO = new('long',0);
    b->robLd = new('long',0);

    // initialises the battery variables for each robot
    k = for(1, g->nRob) {
        b->battLevel.(k-1) = 100. - rand%51; // initialises the battery level between 50% and 100 %
        logInfo('[BATT] Robot %i started with %.f percents of battery', k, b->battLevel.(k-1));

        b->updateBattLevelPeriod = 60; // callback period for updateBattLevel function

        serial = number2Serial(k);
        truckModel = getPreferenceValue("general/modelFileName", serial);
        modelType = getPreferenceValue("general/truckModel", serial);
        truckType = getPreferenceValue("general/truckType", serial);
        hardwareBatt = getPreferenceValue("battery/hardware", serial);
        switch(hardwareBatt) {
            case("TPPL");
                b->battType.{k-1} = 'TPPL';
                b->robTPPL.(k-1) = k;
            case("CAN", "Voltage", "Kiwi");
                b->battType.{k-1} = 'Ld';
                b->robLd.(k-1) = k;
                if (g->enableOPC == 1) {
                    logWarn('[BATT] Cant use opportunity charging with lead-acid battery. Disabling opportunity charging.');
                    g->enableOPC = 0;
                }
            case("Emrol");
                b->battType.{k-1} = 'LTO';
                b->robLTO.(k-1) = k;
            default;
                logError('[BATT] Battery type %z does not exist. Assign "Ld" type instead.', hardwareBatt);
                b->battType.{k-1} = "Ld";
        }
        battType = b->battType.{k-1};

        // initialization of charge parameters
        b->U.(k-1) = 0.; // U = voltage [V]
        b->C.(k-1) = 0.; // C = electric charge [A.h]
        b->P.(k-1) = 0.; // P = mean power of truck in use [W]
        b->use.(k-1) = 0.; // use of the battery over a defined period of time (updateBattLevelPeriod)
        b->numBattMissDone.(k-1) = 0; // number of battery missions done
        b->aggregateDischarge.(k-1) = 0.; // cumulated discharge of the battery
        b->robInCharge.(k-1) = 0; // see also in sim_miss
        b->dischargeThreshold.(k-1) = 10000; // discharge threshold for TPPL battery (voluntarily very high at initialisation)

        // 1. Incompatible cases
        if ((/* TPPL incompatibility */(!strcmp(battType, "TPPL") && (!strcmp(truckModel, "L16HD_3PZS") || !strcmp(truckModel, "L16HD_4PZS") || !strcmp(truckModel, "L20HD_3PZS")))) || /* LTO incompatibility */(!strcmp(battType, "LTO") && (!strcmp(modelType, "YALE_MC") || !strcmp(modelType, "YALE_MO") || !strcmp(truckType, "HYSTER_B80Z_IP7")))) {
            displayError(sprintf('[BATT] Rob %i : Model %z is incompatible with %z battery. Please change battery type.', k, truckModel, battType));
        }
        // 2. Ld
        else if (!strcmp(battType, "Ld") && (!strcmp(truckModel, "R16HD_4PZS") || strMatch(truckModel,"RGen2_4PZS") || strMatch(truckModel,"R16HD_Gen2_4PZS"))) {
            b->U.(k-1) = 48.; // U = voltage [V]
            b->C.(k-1) = 560.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "Ld") && (strMatch(truckModel, "R16HD_5PZS") || strMatch(truckModel, "RGen2_5PZS"))) {
            b->U.(k-1) = 48.; // U = voltage [V]
            b->C.(k-1) = 700.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "Ld") && (!strcmp(truckModel, "L12AC_HL") || strMatch(truckModel, "L12AC_LL") || !strcmp(truckModel, "L12HP_VarianteA") || !strcmp(truckModel, "L12HP_VarianteB") || !strcmp(truckModel, "L12HP_VarianteC") || !strcmp(truckModel, "L16HD_3PZS") || !strcmp(truckModel, "L20HD_3PZS") || !strcmp(truckModel, "T30"))) { // TODO - Add Tmatic model when possible
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 375.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "Ld") && (!strcmp(truckModel, "L16AC_HL") || strMatch(truckModel, "L16AC_LL") || !strcmp(truckModel, "P50_A-Frame") || !strcmp(truckModel, "P50_FV") ||  !strcmp(truckModel, "L16HD_4PZS"))) {
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 500.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "Ld") && !strcmp(truckType, "VNA")) {
            // TODO - Put correct values
            // Two types of batteries according to Balyo : 48 and 80 V
            // Three batteries on Linde base truck (not necessarily Balyo trucks) : U = 80 V and C = 465 or 775 or 930
        }
        else if (!strcmp(battType, "Ld") && (!strcmp(truckModel, "MC10_CAN") || !strcmp(truckModel, "MC10_Torque") || !strcmp(truckModel, "MC10_TorqueUS"))) {
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 300.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "Ld") && (!strcmp(truckModel, "MC12_CAN") ||  !strcmp(truckModel, "MC12_Torque") ||  !strcmp(truckModel, "MC12_TorqueUS") ||  !strcmp(truckModel, "MC15_CAN") ||  !strcmp(truckModel, "MC15_Torque") ||  !strcmp(truckModel, "MC15_TorqueUS"))) {
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 400.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "Ld") && !strcmp(modelType, "YALE_MO")) {
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 620.; // C = electric charge [A.h]
        }
        // 3. LTO
        else if (!strcmp(battType, "LTO") && !strcmp(truckType, "ReachTruck")) {
            b->U.(k-1) = 48.; // U = voltage [V]
            b->C.(k-1) = 280.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "LTO") && !strcmp(truckType, "VNA")) {
            b->U.(k-1) = 48.; // U = voltage [V]
            // TODO - Put correct values
            // Two types of batteries : C = 268 or 1005
        }
        else if (!strcmp(battType, "LTO")) {
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 140.; // C = electric charge [A.h]
        }
        // 4. TPPL
        else if (!strcmp(battType, "TPPL") && !strcmp(truckType, "ReachTruck")) {
            b->U.(k-1) = 48.; // U = voltage [V]
            b->C.(k-1) = 560.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "TPPL") && (!strcmp(truckModel, "L12AC_HL") || strMatch(truckModel, "L12AC_LL") || !strcmp(truckModel, "L12HP_VarianteA") || !strcmp(truckModel, "L12HP_VarianteB") || !strcmp(truckModel, "L12HP_VarianteC") || !strcmp(truckModel, "T30"))) { // TODO - Add Tmatic model when possible
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 375.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "TPPL") && (!strcmp(truckModel, "L16AC_HL") || strMatch(truckModel, "L16AC_LL") || !strcmp(truckModel, "P50_A-Frame") || !strcmp(truckModel, "P50_FV"))) {
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 500.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "TPPL") && !strcmp(truckType, "VNA")) {
            // TODO - Put correct values
            // Two types of batteries according to Balyo : 48 and 80 V
            // Three batteries on Linde base truck (not necessarily Balyo trucks) : U = 80 V and C = 465 or 775 or 930
        }
        else if (!strcmp(battType, "TPPL") && (!strcmp(truckModel, "MC10_CAN") || !strcmp(truckModel, "MC10_Torque") || !strcmp(truckModel, "MC10_TorqueUS"))) {
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 300.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "TPPL") && (!strcmp(truckModel, "MC12_CAN") ||  !strcmp(truckModel, "MC12_Torque") ||  !strcmp(truckModel, "MC12_TorqueUS") ||  !strcmp(truckModel, "MC15_CAN") ||  !strcmp(truckModel, "MC15_Torque") ||  !strcmp(truckModel, "MC15_TorqueUS"))) {
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 274.; // C = electric charge [A.h]
        }
        else if (!strcmp(battType, "TPPL") && !strcmp(modelType, "YALE_MO")) {
            b->U.(k-1) = 24.; // U = voltage [V]
            b->C.(k-1) = 620.; // C = electric charge [A.h]
        }
        // 5. Error
        else {
            logError(sprintf('[BATT] Rob %i : Missing information for model %s with %s battery (unknown case). Check initChargeParam().', k, truckModel, battType));
        }

        // Power by model
        // TODO - Check these values
        if (!strcmp(truckType, "ReachTruck")) {
            b->P.(k-1) = 3900.; // P = mean power of truck in use [W]
        }
        else if (!strcmp(truckType, "CounterbalancedPalletStacker") || !strcmp(truckType, "LINDE_1173")) {
            b->P.(k-1) = 1460.; // P = mean power of truck in use [W]
        }
        else if (!strcmp(truckType, "Tugger")) {
            b->P.(k-1) = 1070.; // P = mean power of truck in use [W]
        }
        else if (!strcmp(truckType, "PalletStacker")) {
            b->P.(k-1) = 970.; // P = mean power of truck in use [W]
        } else if (!strcmp(truckType, "LongPalletTruck") ||!strcmp(truckType, "PalletTruck")) {
            // TODO - Find values for T30, MPE080_Double-Forks and MP080_Single-Forks
        } else if (!strcmp(truckType, "VNA")) {
            // TODO - Find values for K-gen1_PROTO and K-gen2_PROTO
        }
        else {
            logWarn('[BATT] Rob %i : Missing information for truck type %s (model %s). Check function initChargeParam().', k, truckType, truckModel);
        }

        // If missing information = use standard discharge parameters
        if (b->U.(k-1) == 0. || b->C.(k-1) == 0. || b->P.(k-1) == 0.) {
            if (!strcmp(battType, "LTO")) {
                logWarn(sprintf('[BATT] Rob %i : Missing information for model %z with %z battery : U = %.f, C = %.f, P = %.f. Use LTO basic discharge time instead (4h30).', k, truckModel, battType, b->U.(k-1), b->C.(k-1), b->P.(k-1)));
                b->use.(k-1) =  100./(b->updateBattLevelPeriod * 4.5);
            } else if (!strcmp(battType, "Ld")) {
                logWarn(sprintf('[BATT] Rob %i : Missing information for model %z with %z battery : U = %.f, C = %.f, P = %.f. Use lead basic discharge time instead (8h)', k, truckModel, battType, b->U.(k-1), b->C.(k-1), b->P.(k-1)));
                b->use.(k-1) = 100./(b->updateBattLevelPeriod * 8);
            } else if (!strcmp(battType,"TPPL")) {
                logWarn(sprintf('[BATT] Rob %i : Missing information for model %z with %z battery : U = %.f, C = %.f, P = %.f. Use TPPL basic discharge time instead (4h)', k, truckModel, battType, b->U.(k-1), b->C.(k-1), b->P.(k-1)));
                b->use.(k-1) = 100./(b->updateBattLevelPeriod * 4);
            }
        }
        // Compute battery discharge
        else {
            b->use.(k-1) = (100 * b->P.(k-1) * b->updateBattLevelPeriod) / (3600 * b->U.(k-1) * b->C.(k-1)); // discharge of battery during the callback period
        }
    }

    // Opportunity charging parameters
    // TODO : Make groups of LTO and groups of TPPL
    if (size(find(b->robLTO)) != 0) {
        b->nbRobAvlbl = 0;
        b->lowWL = 0.5 * g->nRob;
        b->highWL = 0.8 * g->nRob;
        b->LTOLowBatt = getPreferenceValue("opportunityCharging/lowActivity/batteryThreshold",serial);
        b->LTOMedBatt = getPreferenceValue("opportunityCharging/mediumActivity/batteryThreshold",serial);
        b->LTOHighBatt = getPreferenceValue("opportunityCharging/highActivity/batteryThreshold",serial);
        b->LTOthresh = 25;
    }
    if (size(find(b->robTPPL)) != 0) {
        b->nbRobAvlbl = 0;
        b->lowWL = 0.5 * g->nRob;
        b->highWL = 0.8 * g->nRob;
        b->TPPLLowBatt = 60.;
        b->TPPLMedBatt = 50;
        b->TPPLHighBatt = 40;
        b->TPPLthresh = 40;
        logWarn('[BATT] If you want to simulate the equalisation charge with TPPL, take an adequate time frame (5 days recommended)');
    }

    setRandCbk('$updateBattLevel',sprintf('%is',b->updateBattLevelPeriod),'0s'); // creates callback that updates and checks battery levels every 60 seconds
}


/*
    Add battery mission into sim schd
    @param title [char] -> name of the batt mission
    @param idrob [long] -> id of the robot
    @param stopTime [long] -> duration of the charge
*/
export function batteryToPCsol(title,idrob,stopTime) {
    v = varspc();
    v->taskName = strn(title); // set taskName
    v->pgm = sprintf('rob_battery %i',stopTime); // set pgm
    v->priority = 1000; // set batt priority
    v->robAuth = {idrob}; // set robAuth
    v->addDate = simGetT(); // set addDate
    v->missId = -2; // set missId : -2 (reserved for batteries !)
    key = DBadd_gbl('simSchdDB',v); // add task in simSchdDB
    //    pcsolNewTask(title);
}


/*
    Choose closest available battery charger destination
    @param idrob [long] -> id of the robot
    @return dst [char] -> the batt point name the robot will go to
*/
export function dst = chooseCollectiveDest(idrob) {
    global g('simConf');

    robModel = getRobModel(idrob);
    flag = getModelFlag(idrob);

    dst = new('char',0);
    rpkAll = new('double',0); // rpk of all batt position
    rpkFree = new('double',0); // rpk of free batt position

    maxValue = 1000000.; //arbitrarily high value

    // retrieve all 'charger' type points from model layer
    battPnts = getDestinationTypePoints('charger',robModel);

    if(isempty(battPnts)) {
        displayError(sprintf('[chooseCollectiveDest] No charger found for layer %s.',robModel));
        return;
    }

    i = for(battPnts) {
        // get position of the battery destination point
        battName = battPnts.{i};
        battInfo = missPntGetInfo(name2id(battName));
        battPnt = TRACK_GetDestinationPoint(robModel,battName).point;
        battCpx = cpx(battPnt.{0}.xposition,battPnt.{0}.yposition);
        [_,_,_,battRpk] = TRACK_FindItinerary(robModel,robGetZ(idrob),battCpx,flag);

        // check that position is free (we use scanList i know its ugly) //TODO
        if(isempty(battRpk)) rpkAll.(end+1) = maxValue;
        if(!isempty(battInfo->scanList.{0})) rpkFree.(end+1) = maxValue;
        else rpkFree.(end+1) =  battRpk;
        rpkAll.(end+1) = battRpk;
    }

    // once we know the rpk values of each battery points, we aim for the closest vacant point
    [minRpk, minId] = min(rpkFree);
    if(minRpk >= maxValue) {
        [minRpk, minId] = min(rpkAll);
    }

    // get the id, the pnt and reserve it
    if(isempty(minId)) logError('[chooseCollectiveDest] No min rpk found!');

    //return the destination
    dst = battPnts.{minId.(0)};
    logDebug('[chooseCollectiveDest] Charger %s chosen for battery mission of rob %i',dst,idrob);

    //book the charger
    DBset_gbl('stkDB',name2id(dst),varspc('scanList',{idrob},'groupName',sprintf('batt_for_%i',idrob)));

}

/*
    Check if at least 1 charger is available (use this only for LTO)
    @param idrob [long] -> id of the robot
    @return isOk [bool] -> 1: if at least 1 charger is available, 0: all charger occupied
*/
export function isOk = availableCollectiveCharger(idrob) {
    robModel = getRobModel(idrob);
    isOk = 0; //not ok by default

    // retrieve all 'charger' type points from model layer
    battPnts = getDestinationTypePoints('charger',robModel);

    if(isempty(battPnts)) {
        displayError(sprintf('[availableCharger] No charger found for layer %s.',robModel));
        return;
    }

    i = for(battPnts) {
        // get scan list of charger point
        battName = battPnts.{i};
        battState = DBget_gbl('stkDB',name2id(battName),'scanList').{0};
        // if empty then charger is free
        if(isempty(battState)) {
          isOk = 1;
          break; //at least 1 charger is free : exit the loop
        }
    }

}


/*
    Battery mission called periodically according to type of batt
    @param stopTime [long] -> duration of the charge
    @param missId [long] -> TODO (useless ?)
    @param taskId [long] -> TODO
*/
export function rob_battery(stopTime, missId, taskId) {
    global g('simConf');
    global b('simBatt');

    //get rob info
    idrob = robGetId;
    serial = number2Serial(idrob);
    flag = getPreferenceValue("navigation/pathFlag",serial);
    robModel = getRobModel(idrob);

    //get mission info
    stopTime = str2num(stopTime);
    missId = str2num(missId); // useless ?
    taskId = str2num(taskId);

    // battery mission config
    battStep = varspc();
    battStep->via = ''; // no via
    battStep->approachTime = -1; // no approach time
    battStep->detectionTime = -1; // no detection time
    battStep->destinationTime = stopTime; // destination time = battery charge time
    battStep->task = 'move'; // similar to move mission
    battStep->nbPal = 0; // no pallet
    battStep->slot = -1; //no slot defined
    battStep->pallet = g->pallets.{0}.{0}; //pallet by default

    switch(g->battPntStrategy) {

        case('individual');
            dst = sprintf('batt%i',idrob);

        case('collective');
            dst = chooseCollectiveDest(idrob);

        case('taxi','taxiOpti');
            dst = sprintf('taxi%i',idrob);

            // case('custom');
            // dst = customFunction(idrob);

        default;
            logError('[rob_battery] Unknown battery strategy "%s". Check your configuration file.',g->battPntStrategy); //impossible to arrive here
    }

    // init miss varspc
    battMiss = initMissVarspc(battStep,dst,'move',0);

    // start counting time for statistics
    saveTaxiTime(idrob);
    g->battStart.{idrob-1} = simGetT;

    // start agv movement and evacuation
    startMission(battMiss);
    // manage evacuation
    goToEvacuation(battMiss);
    // navigate to rpkLastAR and setReversemode
    goToLastReverse(battMiss);
    // navigate to rpk approach
    goToBatt(battMiss);

    if (!strcmp(b->battType.{idrob-1}, "Ld")) b->battLevel.(idrob-1) = 100.; // battery level updated to 100 % if lead-acid battery
    b->numBattMissDone.(idrob-1) = b->numBattMissDone.(idrob-1) + 1;

    // free the batt point at the end of the charging
    if (!strcmp(g->battPntStrategy,'collective')) {
        DBset_gbl('stkDB',name2id(dst),varspc('scanList',{[]},'groupName',''));
    }

    // store stop at battery point time
    endBattStopTime(idrob,battMiss->startTime);

    // check if the simulation should continue
    isSimEndTime();

    // launch done cbk
    taskDoneCbk(taskId, 'BATTERY', idrob);

}


/*
    Function called periodically (60 s) to update and check battery level, creates battery mission when necessary
*/
export function updateBattLevel() {
    global g('simConf');
    global b('simBatt');

    // 1. Battery discharge / charge
    k = for(1, g->nRob) {
        if (b->battLevel.(k-1) >= 0.0) {
            // logDebug('[BATT] Rob %i : Battery level at %f', k, b->battLevel.(k-1));
            // logDebug('[BATT] Rob %i : Battery discharge at %f', k, b->aggregateDischarge.(k-1));

            currentTask = robFindTask(k);

            // If the robot is working, then the battery level has to decrease
            if (strcmp(currentTask, "noTask") && strcmp(currentTask, sprintf('BATT_%i',k)) && strcmp(currentTask, sprintf('BATT_EQ_%i',k))) {
                b->battLevel.(k-1) = b->battLevel.(k-1) - b->use.(k-1); // updates battery level
                b->aggregateDischarge.(k-1) = b->aggregateDischarge.(k-1) + b->use.(k-1) ; // total discharge
            }

            // If the robot is in charge, then increase the battery level (LTO and TPPL)
            else if (!strcmp(currentTask, sprintf('BATT_%i',k)) || !strcmp(currentTask, sprintf('BATT_EQ_%i',k)) && (b->robInCharge.(k-1) == 1)) {
                if (b->battLevel.(k-1) >= 100.) {
                    b->battLevel.(k-1) = 100.; // battery level might be slightly over 100.0
                } else {
                    switch(b->battType.{k-1}) {
                        case('LTO');
                            if (b->C.(k-1) == 0.) {
                                b->battLevel.(k-1) = b->battLevel.(k-1) + 75.*b->updateBattLevelPeriod/(17*60); // charge by default = from 25 % to 100 % in 17 min
                            } else {
                                b->battLevel.(k-1) = b->battLevel.(k-1) + 100*b->updateBattLevelPeriod*360/(3600*b->C.(k-1)); // charge if C is known
                            }
                        case('TPPL');
                            if (b->battLevel.(k-1) > 70.) { // charge above 70 % = from 70 % to 100 % in 1 h 30 min
                                b->battLevel.(k-1) = b->battLevel.(k-1) + 30*b->updateBattLevelPeriod/(1.5*3600);
                            } else { // charge below 70 %
                                b->battLevel.(k-1) = b->battLevel.(k-1) + 100*b->updateBattLevelPeriod*0.7/3600;
                            } // TODO : Add charge under 40 %
                    }
                }
            }

            // If the robot is at its taxi point
            else if (isRobotAtTaxi(k)) {
                // If the robot has to charge at taxi (taxi = batt)
                if (!strcmp(g->battPntStrategy,'taxiOpti')) {
                    if (b->battLevel.(k-1) >= 100.) {
                        b->battLevel.(k-1) = 100.; // battery level might be slightly over 100.0
                    } else {
                        switch(b->battType.{k-1}) {
                            case('LTO');
                                if (b->C.(k-1) == 0.) {
                                    b->battLevel.(k-1) = b->battLevel.(k-1) + 75.*b->updateBattLevelPeriod/(17*60); // charge by default = from 25 % to 100 % in 17 min
                                } else {
                                    b->battLevel.(k-1) = b->battLevel.(k-1) + 100*b->updateBattLevelPeriod*360/(3600*b->C.(k-1)); // charge if C is known
                                }
                            case('TPPL');
                                if (b->battLevel.(k-1) > 70.) { // charge above 70 % = from 70 % to 100 % in 1 h 30 min
                                    b->battLevel.(k-1) = b->battLevel.(k-1) + 30*b->updateBattLevelPeriod/(1.5*3600);
                                } else { // charge below 70 %. TODO : Add charge under 40 %
                                    b->battLevel.(k-1) = b->battLevel.(k-1) + 100*b->updateBattLevelPeriod*0.7/3600;
                                }
                        }
                    }
                }
                // If the robot is inactive, then the battery level decreases slowly
                else {
                    if (b->C.(k-1) == 0.) {
                        b->battLevel.(k-1) = b->battLevel.(k-1) - 0.033; // by default = 2 % per hour TODO : put correct value
                        b->aggregateDischarge.(k-1) = b->aggregateDischarge.(k-1) + 0.033; // total discharge
                    } else {
                        b->battLevel.(k-1) = b->battLevel.(k-1) - 100*5*b->updateBattLevelPeriod/(3600*b->C.(k-1)); // 5 A discharge
                        b->aggregateDischarge.(k-1) = b->aggregateDischarge.(k-1) + 100*5*b->updateBattLevelPeriod/(3600*b->C.(k-1)); // total discharge
                    }
                }
            }
        } else logInfo('[BATT] Rob %i : Battery level (%f) is incorrect', k, b->battLevel.(k-1));
    }

    // 2. Workload calculation for opportunity charging
    if(g->enableOPC) {
        // LTO case
        if (size(find(b->robLTO)) != 0) {
            LTOthresh = b->LTOthresh; //25 by default
            nbRobBusy = 1.*(g->nRob - size(simGetAvailableRob())); // number of inactive robots
            // logDebug('nbRobBusy : %z, LowWL : %z, HighWL : %z', nbRobBusy, b->lowWL, b->highWL);
            // Calculates work load and adapts threshold
            if (nbRobBusy < b->lowWL && !same(LTOthresh,b->LTOLowBatt)) {
                b->LTOthresh = b->LTOLowBatt;
                logInfo('[BATT] Rob activity is  now low (LTO battery threshold : %i)', b->LTOthresh);
            } else if (nbRobBusy > b->lowWL && nbRobBusy < b->highWL && !same(LTOthresh, b->LTOMedBatt)) {
                b->LTOthresh = b->LTOMedBatt;
                logInfo('[BATT] Rob activity is now medium (LTO battery threshold : %i)', b->LTOthresh);
            } else if (nbRobBusy > b->highWL && !same(LTOthresh, b->LTOHighBatt)) {
                b->LTOthresh = b->LTOHighBatt;
                logInfo('[BATT] Rob activity is now high (LTO battery threshold : %i)', b->LTOthresh);
            }
        }
        // TPPL case
        else if (size(find(b->robTPPL)) != 0) {
            TPPLthresh = b->TPPLthresh; // 40 by default
            nbRobBusy = 1.*(g->nRob - size(simGetAvailableRob())); // number of inactive robots
            // logDebug('nbRobBusy : %z, LowWL : %z, HighWL : %z', nbRobBusy, b->lowWL, b->highWL);
            // Calculates work load and adapts threshold
            if (nbRobBusy < b->lowWL && !same(TPPLthresh, b->TPPLLowBatt)) {
                b->TPPLthresh = b->TPPLLowBatt;
                logInfo('[BATT] Rob activity is now low (TPPL battery threshold : %z)', b->TPPLthresh);
            } else if (nbRobBusy > b->lowWL && nbRobBusy < b->highWL && !same(TPPLthresh, b->TPPLMedBatt)) {
                b->TPPLthresh = b->TPPLMedBatt;
                logInfo('[BATT] Rob activity is now medium (TPPL battery threshold : %z)', b->TPPLthresh);
            } else if (nbRobBusy > b->highWL && !same(TPPLthresh, b->TPPLHighBatt)) {
                b->TPPLthresh = b->TPPLHighBatt;
                logInfo('[BATT] Rob activity is now high (TPPL battery threshold : %z)', b->TPPLthresh);
            }
        }
    }


    k = for(1, g->nRob) {
        if (isempty(setDiff(DBfind_gbl('simSchdDB','taskName','==',sprintf('BATT_%i', k)), DBfind_gbl('simSchdDB','state','==',3)))) { // checks that there is no battery mission waiting

            // 3. Equalisaiton charge for TPPL
            if (!strcmp(b->battType.{k-1}, "TPPL")) { // Equalisation charge with TPPL
                // time slot inside which an equalisation charge can be launched
                eqChargeTimeSlot1 = 5; // equalisation charge before 5 h
                eqChargeTimeSlot2 = 21; // equalisation charge after 21 h
                b->dischargeThreshold.(k-1) = 480.; // cumulated discharge threshold above which there is a need for an equalisation charge
                if (b->aggregateDischarge.(k-1)>b->dischargeThreshold.(k-1) && ((simGetT()%86400<eqChargeTimeSlot1*3600.) || (simGetT()%86400>eqChargeTimeSlot2*3600.))) {
                    logInfo('[BATT] Rob %i : Equalisation charge', k);
                    if (b->C.(k-1) == 0.) { // if C is missing, default value
                        logWarn("[BATT] Rob %i : Missing information. Use TPPL equalisation charging time instead (5 hours)", k);
                        stopTime = 5 * 3600 ;
                    } else { // standard case
                        stopCharge = 70.;
                        amountToCharge = (stopCharge - b->battLevel.(k-1)) / 100;
                        stopTime = 60 + (3600 * amountToCharge / 0.7) + 1.5 * 3600 + 2 * 3600; // charging to 70 %, then 100 %, then equalisation
                        stopTime = long(ceil(stopTime));
                    }
                    batteryToPCsol(sprintf('BATT_EQ_%i', k), k, stopTime); // Launch BATT missions
                    b->aggregateDischarge.(k-1) = 0.;
                }
            }

            // 4. Battery charge / switch
            if (!strcmp(b->battType.{k-1}, "LTO")) {
                threshold = b->LTOthresh;
            } else if (!strcmp(b->battType.{k-1}, "TPPL")) {
                threshold = b->TPPLthresh;
            } else if (!strcmp(b->battType.{k-1}, "Ld")) {
                serial = number2Serial(k);
                threshold = 100 * getPreferenceValue("battery/level/low", serial);
            }

            if(b->battLevel.(k-1) < threshold) {
                logInfo('[BATT] Rob %i : Battery level at %.f (charging threshold is %.f). Creating battery mission.', k, b->battLevel.(k-1), threshold);

                switch(b->battType.{k-1}) {
                    case('Ld');
                        stopTime = 20*60; // 20 min for battery switch
                        stopTime = long(ceil(stopTime));

                    case('TPPL');
                        // TODO - Check that there is no equalisation charge mission running ?
                        stopCharge = 70.;
                        amountToCharge = (stopCharge - b->battLevel.(k-1)) / 100;
                        if (b->C.(k-1) == 0.) { // if information is missing
                            logWarn("[BATT] Rob %i : Missing information. Use TPPL basic charging time instead (27 min)", k);
                            stopTime = 28*60 ;
                        } else { // standard case
                            stopTime = 60 + (3600 * amountToCharge * b->C.(k-1)) / (0.7 * b->C.(k-1)) ;
                            stopTime = long(ceil(stopTime));
                        }

                    case('LTO');
                        availableCharger = availableCollectiveCharger(k);
                        if(0 == availableCharger) {
                          logWarn("[BATT] Rob %i : no LTO charger available. Skip battery mission for now.",k);
                          continue;
                        }
                        stopCharge = 100.;
                        amountToCharge = (stopCharge - b->battLevel.(k-1)) / 100;
                        if (b->C.(k-1) == 0.) { // if information is missing
                            logWarn("[BATT] Rob %i : Missing information. Use LTO basic charging time instead (17 min)", k);
                            stopTime = 17*60 ;
                        } else { // standard case
                            stopTime = (3600 * amountToCharge * b->C.(k-1)) / 360 ;
                            stopTime = long(ceil(stopTime));
                        }
                }
                logInfo('[BATT] Rob %i : Create battery mission, robot will charge for %i minutes.', k, stopTime/60);
                batteryToPCsol(sprintf('BATT_%i', k), k, stopTime); // Launch BATT missions
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                           TIME COUNTERS FUNCTIONS                           //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    End the battery MOVE counter
    @param idrob [long] -> id of the robot
    @param startTime [long] -> time stamp empty
*/
export function endBattMoveTime(idrob,startTime) {
    global g('simConf');
    global r('simRob');
    simuTime = simGetT();
    // if we are after startCollectData, then update counters emptyStart and totalEmptyTime
    if(simuTime > g->startCollectData) {
        startTime = max(startTime,g->startCollectData); //record time only from startCollectData
        r->timeBattMove.(idrob-1) = r->timeBattMove.(idrob-1) + (simuTime - startTime);
    }
}


/*
    End the battery STOP counter
    @param idrob [long] -> id of the robot
    @param startTime [long] -> time stamp empty
*/
export function endBattStopTime(idrob,startTime) {
    global g('simConf');
    global r('simRob');
    simuTime = simGetT();
    // if we are after startCollectData, then update counters emptyStart and totalEmptyTime
    if(simuTime > g->startCollectData) {
        startTime = max(startTime,g->startCollectData); //record time only from startCollectData
        r->timeBattStop.(idrob-1) = r->timeBattStop.(idrob-1) + (simuTime - startTime);
    }
}
