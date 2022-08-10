
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                    CREATION OF FLOWS AND MISSIONS VARSPC                    //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Initialisation of flows and missions
    @return flows [L.varspc] -> list of flows varspc
*/
export function flows = initSimFlows() {
    global g('simConf');

    // Flow varspc
    flow = varspc('flowName',strn(),'flowOutput',[],'enable',[]);

    flows      = {};
    flowRph    = [];
    pals       = [];

    // Add missions from global
    k = for(g->missions) {
        v = g->missions.{k};

        if (g->factor != 1.0) {
            v->ratePerHour = v->ratePerHour * g->factor;
        }

        flow->flowName.(end+1) = v->title;
        flow->flowOutput.(end+1) = v->ratePerHour;
        flow->enable.(end+1) = v->enable;
        if(!v->enable)
            continue;
        [fluxl, pall] = acall('initFlow',v,k);
        kk = for(fluxl) {
            idx = findElt(flows,fluxl.{kk});
            if (size(idx)) {
                flowRph.(idx) = flowRph.(idx) + v->ratePerHour;
                pals.(idx) = pals.(idx) + v->ratePerHour * pall.(kk);
            } else {
                flows.{end+1} = fluxl.{kk};
                flowRph.(end+1) = v->ratePerHour;
                pals.(end+1) = v->ratePerHour * pall.(kk);
            }
        }
    }

    writeSimParam();
    writeFlowConfig('mission',flow);
    writeFlowConfig('flow',varspc('flowName',cell2Strn(flows),'flowOutput',flowRph,'enable', pals));
}


/*
    Creates the mission and adds it to the missions list 'g->missions' and generate automatic triggering
    @param v [varspc] -> mission definition varspc
    @param id [long] -> id of mission
    @return flows [L.char] -> List of flow names
    @return pal   [L.long] -> List of flow expected output (pal/h)
*/
export function [flows, pal] = initFlow(v,id) {
    global g('simConf');

    // Typing
    flows = {};
    pal   = [];

    // Checks flow consistency before going further
    checkFlowConsistency(v);

    tmp = 0;
    // Loops over all the flows in v
    k = for(0,size(v->flows)-1) {
        flows.{end+1} = v->flows.{k}->flowName;
        tmp = tmp + v->flows.{k}->numberOfStep;

        if (v->flows.{k}->flowDisplay) {
            pal.(end+1)   = getFlowPal(v, k, tmp-1);
        }
        else {
            pal.(end+1) = 0;
        }
    }

    // Adjust cadence to make sure that even with minimum utilization ratio we can still meet customer pallet move expectations
    aux = v->ratePerHour * 1.0 / (min(g->utilizationRatioTable));

    // Compute frequence and randomness for mission generation
    frequence = sprintf('%fp/h',aux);
    aleatoire = sprintHMS(3600.0/(aux*4),1);

    // Build varspc with all mission data
    vv = v;
    vv->id = id;

    //get priority
    if(!isvar(vv,'priority')) vv->priority = 1; //check if priority defined

    // Create callback for the scheduler and the agv
    vv->miss = sprintf('$addTaskInPcsol %s %i %i',vv->title,vv->id,vv->priority);

    // Generate automatically missions
    if(v->missAuto){
        setRandCbk(vv->miss,frequence,aleatoire); //creation of missions
    }
    else{
        setRandCbk(sprintf('$customMissProfileCbk %s',vv->title),'1s','0s');    // create mission if missAuto =0
    }
        

    // Add all mission data in the original varspc
    mergeVarspc(g->missions.{id},vv);
}


/*
    Verifies a flow definition (check number of steps and points / groups existence)
    @param v [varspc] -> mission definition varspc
*/
export function checkFlowConsistency(v) {
    global g('simConf');

    steps = size(v->objectives); // steps = number of steps written in 'objectives' in the mission varspace

    tmp = 0;
    k = for(0,size(v->flows)-1) {
        tmp = tmp + v->flows.{k}->numberOfStep; // tmp = number of steps as written in 'numberOfStep' in the mission varspace
    }

    // If steps =/= tmp then display an error
    if (steps != tmp) {
        logError(['Inconsistency detected in mission (',v->title,') configuration']);
        logError('Number of steps in mission: %z',steps);
        logError('Sum of flow steps: %z',tmp);
        displayError(sprintf(['Inconsistency in mission ',v->title]));
    }
    // Get the authorized robots for this mission
    robAuth = v->group;

    // Check points / groups existence
    k = for(v->objectives) {
        i = for(robAuth) {
            if(isvar(v->objectives.{k},"dst")) {
                // return all available ids for a layer (inclunding Common)
                allIds = getLayerAllIds(getRobModel(robAuth.(i)));
                dst = v->objectives.{k}->dst;
                if(isempty(findElt(allIds,name2id(dst)))) {
                    displayError(sprintf('[checkFlowConsistency] Point "%s" is invalid for rob %i (mission "%s") \n Check the existence of the point',v->objectives.{k}->dst,robAuth.(i),v->title));
                }
                // check move points validity
                dstType = char(missPntGetInfo(dst)->type);
                if(!strcmp(v->objectives.{k}->task,'move') && strcmp(dstType,"move")) {
                    displayError(sprintf('[checkFlowConsistency] Point %s (%s) is not a valid move point (mission "%s")',dst,dstType,v->title));
                }
            }
            else continue; //skip if "grp"

        }
    }
}


/*
    Initialise the mission varspace
    @param step [varspc] -> information of the task (step of the mission)
    @param destination [char] -> destination point name
    @param missionType [char] -> 'slot', 'line', 'move', 'elevated', 'elevatedVNA'
    @param idPal [long] -> id of the pallet
    @return varspcMiss [varspc] -> mission varspc
*/
export function varspcMiss = initMissVarspc(step,destination,missionType,idPal)
{

    // ** TRUCK TYPE **
    idrob = robGetId;
    serial = number2Serial(idrob);
    truckType = getPreferenceValue("general/truckType",serial);
    robInfo = robGet(idrob);
    robModel = getRobModel(idrob);
    robPos = robGetZ;

    // ** DST POINT CONFIGURATION **
    if(!isNumeric(destination)) dstId = name2id(destination);
    else dstId = destination;
    pointData = missPntGetInfo(dstId);

    // ** VIA POINT CONFIGURATION **
    if(!isempty(step->via)) {
        try viaPnt = TRACK_GetDestinationPoint(robModel,step->via);
        catch displayError(sprintf("[initMissVarspc] - Via point invalid (%z)",step->via));
        viaCpx = viaPnt->point.{0};
    }
    else viaCpx = cpx();

    // ** PATHFLAG **
    flag = getPreferenceValue("navigation/pathFlag",serial);

    // ** FORWARD AND BACKWARD SPEED **
    vmax = getPreferenceValue("trajectoryDriver/speedLimit/maxForwardSpeed",serial);
    vmaxar = getPreferenceValue("trajectoryDriver/speedLimit/maxBackwardSpeed",serial);

    // ** USEFUL LENGHTS **
    DIST_ROLLERS_BACKREST = getPreferenceValue("general/backrestAbscissa",serial);
    DIST_ROLLERS_ENDFRK   = abs(getPreferenceValue("general/forksEndAbscissa",serial)); //always positive
    DIST_FORKS  					= DIST_ROLLERS_BACKREST + DIST_ROLLERS_ENDFRK;

    // ** PALLET DIMENSION **
    dimPal = palDefGetDim(step->pallet);
    palLenght = dimPal.re;
    palletSide = new('char',0); // only for VNA

    // ** FORKS OFFSETS && PARAMETERS **
    forkOffsetCarry                 = max(DIST_FORKS,palLenght) + pointData->safetyPointMargin;
    forkOffsetEmpty                 = DIST_FORKS + pointData->safetyPointMargin;
    forkOffsetMove                  = 0.15; //TODO: check config
    [verticalForksEnabled, initialLiftEnabled, moveWithInitialLiftUp] = areForksEnabled(idrob);
    heightApproach                  = 0.0;  //default height at rpk approach
    heightDestination               = 0.0;  //default height at rpk destination
    iliftApproach                   = -1;   //no initial lift movement by default
    iliftDestination                = -1;   //no initial lift movement by default
    travelLimit                     = getPreferenceValue("safety/design/maxHeightOkForMovement",serial);


    // ** RMATIC LENGTHS **
    if(isRmatic) {
        DIST_ROLLERS_BACKREST_DEPLOYED  = getPreferenceValue("general/backrestDeployedAbscissa",serial);
        DIST_FORKS  					          = DIST_ROLLERS_BACKREST_DEPLOYED + DIST_ROLLERS_ENDFRK;
        forkOffsetCarry                 = max(DIST_FORKS,(palLenght - DIST_ROLLERS_BACKREST)) + pointData->safetyPointMargin;
        forkOffsetEmpty                 = DIST_FORKS + pointData->safetyPointMargin;
        DIST_ROLLERS_BACKREST = DIST_ROLLERS_BACKREST_DEPLOYED;
    }

    // ** COMPUTE RPK AND POSITIONS **
    if(strcmp(step->task,'move')) {
        // ** RPK DESTINATION AND END OF LINE POSITION **
        endOfLineCpx = cpx(pointData->zend.(0).xposition, pointData->zend.(0).yposition);
        destiCpx = endOfLineCpx;

        // ** MISSION TYPE AND RPKAPPROACH **
        zstartCpx = cpx(pointData->zstart.(0).xposition, pointData->zstart.(0).yposition);
        zendCpx = cpx(pointData->zend.(0).xposition, pointData->zend.(0).yposition);

    }
    else destiCpx = cpx(pointData->zend.(0).xposition, pointData->zend.(0).yposition);

    // ** LINES RANDOM SLOT CHOICE **
    if(pointData->npalsx < 2) {
        if(step->slot >= 0) logWarn('[initMissVarspc] %s-%s : Parameter "slot" not taken in account (%i slot available).',missionType,destination,pointData->npalsx);
        lineSlot = 0;
    }
    else {
        if((step->slot + 1) > pointData->npalsx) {
            logWarn('[initMissVarspc] %s-%s : Slot defined (%i) greater than number of slot available (%i). Random behavior chosen.',missionType,destination,step->slot,pointData->npalsx);
            lineSlot = rand%(pointData->npalsx);
        }
        else if (step->slot >= 0) {
            logDebug('[initMissVarspc] %s-%s : Slot %i chosen from mission configuration.',missionType,destination,step->slot);
            lineSlot = step->slot;
        }
        else {
            lineSlot = rand%(pointData->npalsx);
            logDebug('[initMissVarspc] %s-%s : Choose slot %i (random).',missionType,destination,lineSlot);
        }
    }


    // ** PICK MISSIONS **
    if(!strcmp(step->task,'pick')) {
        //positions
        rpkDestination = max(0.02,pointData->slotEmplacement.{0}.(lineSlot) + pointData->backPalletOverflow - DIST_ROLLERS_BACKREST);
        rpkApproach = rpkDestination + forkOffsetEmpty + 0.150;
        //forks
        if(verticalForksEnabled) {
            heightApproach = pointData->heightTable.{0};
            heightDestination = heightApproach + forkOffsetMove;
        }
        //initialLift
        if(initialLiftEnabled && moveWithInitialLiftUp) {
            iliftApproach = 0;
            iliftDestination = (verticalForksEnabled) ? 0 : 1;
        }
        //taskname
        taskName = [step->task,totitle(missionType)];
    }
    // ** DROP MISSIONS **
    else if(!strcmp(step->task,'drop')) {
        //positions
        rpkDestination = max(0.02,pointData->slotEmplacement.{0}.(lineSlot) + pointData->backPalletOverflow - DIST_ROLLERS_BACKREST);
        rpkApproach = rpkDestination + forkOffsetCarry + 0.150;
        //forks
        if(verticalForksEnabled) {
            heightDestination = pointData->heightTable.{0};
            heightApproach = heightDestination + forkOffsetMove;
        }
        //initialLift
        if(initialLiftEnabled && moveWithInitialLiftUp) {
            iliftApproach = (heightDestination > 0.0) ? 0 : 1;
            iliftDestination = 0;
        }
        //taskname
        taskName = [step->task,totitle(missionType)];
        // error if no pallet defined for a drop
        if(idPal == 0) displayError(sprintf('[initMissVarspc] No pallet defined for miss %s on %s. (You cannot start with a drop)',step->task,destination));
    }
    // ** MOVE MISSIONS **
    else if(!strcmp(step->task,'move')) {
        rpkDestination = 0.0;
        rpkApproach = -1;
        taskName = totitle(missionType);
    }
    else displayError(sprintf("[initMissVarspc] Unknow task %z" , step->task));

    // ** SPECIAL MOVE FOR VNA **
    if(!strcmp(truckType,'VNA') && strcmp(step->task,'move')) {
        [destiCpx,palletSide] = computeRpkObjectivesVNA(pointData);
        rpkDestination = 0.0;
        rpkApproach = -1;
    }

    // ** ITINERARY AND LAST REVERSE **
    rpkLastAR = 0;

    // ** EVACUATION PARAMETERS **
    evacDst = (idPal > 0)? forkOffsetCarry : forkOffsetEmpty;
    evacDst = evacDst + pointData->safetyPointMargin;
    evacPnt = TRACK_GetPointsAtDistance(robModel,evacDst,robPos);
    evacIdx = 0;
    if(!isempty(evacPnt)) {
        i = for(evacPnt) {
            if(TRACK_isPointInPath(robModel,robPos,destiCpx,cpx(evacPnt.(i).xposition, evacPnt.(i).yposition),flag)) evacIdx = i;
        }
        evacCpx = cpx(evacPnt.(evacIdx).xposition, evacPnt.(evacIdx).yposition);
    }
    else evacCpx = cpx();
    vmaxEvac = getPreferenceValue("trajectoryDriver/speedLimit/vmaxEvac",serial);

    // ** DETECTION TIME **
    if(step->detectionTime < 0) { //automatic behavior
        if(1 == pointData->rackCell.detectionUsed.pallet3D || 1 == pointData->rackCell.detectionUsed.freeSpace3D) {
            step->detectionTime = 5; //new perception TODO : ne pas hard coder ça
        }
    }

    // ** VARIABLE DEFINITION **
    varspcMiss = varspc(
        'task',                         step->task,                               // If mission is pick or drop type
        'missionType',                  missionType,                              // Mission type : slot, line, elevatd , ..
        'destination',                  destination,                              // Destination of the agv in the track
        'destiPos',				  				    destiCpx,  							  						    // Position of the end of line (stock) or pnt (move)
        'pointData',                    pointData,                                // Data from point conf
        'rpkDestination',              	rpkDestination,                          	// The destination rpk where the trolley should stop for clean drop
        'rpkApproach',                  rpkApproach,                              // The approach rpk where AGV speed must be minimum speed
        'rpkBeforeLastReverse',     		rpkLastAR,                                // The rpk of the last agv's reverse
        //'rpkEvac',											rpkEvac,																	// rpk for end of evacuation
        'taskName',                     taskName,                                 // The mission's name
        //'type',                         pointData->type,                          // Type of the destination
        'approachTime',									step->approachTime,												// time to wait at the end before entering destination (manual)
        'detectionTime',								step->detectionTime,											// time to wait for simulate detection fonctions (manual)
        'destinationTime',							step->destinationTime,										// time to wait when reached destination (manual)
        'pathFlag',                     flag,                                    	// Flag for path finding algorithm
        'vmax',                         vmax,                                     // The maximum authorized speed during straight front mouvement
        'vmaxar',                       vmaxar,                                   // The maximum authorized speed during backward mouvement
        'vmaxEvac',											vmaxEvac,																	// The maximum authorized speed during evacuation
        'evacDst',                      evacDst,                                  // Distance of evacuation
        'evacPos',                      evacCpx,                                  // Position of the evacuation
        'heightApproach',               heightApproach,                           // Target height at rpk approach
        'heightDestination',            heightDestination,                        // Target height at rpk destination
        'iliftApproach',                iliftApproach,                            // initial lift target position at rpk approach
        'iliftDestination',             iliftDestination,                         // initial lift target position at rpk destination
        'travelLimit',                  travelLimit,                              // Forks height limit where the robot can move
        //'strategy',                     pointData->fillStrategy,            			// The fill strategy
        'dimPal',                       dimPal,                                   // Dimension of the pallet
        'nbPal',                        step->nbPal,                              // Nb de loads
        'idPal',							 					idPal,										 								// Id of the pallet/load
        'pallet',                       step->pallet,                             // Name of the pallet
        'palletSide',                   palletSide,                               // Side of the pallet (for VNA)
        'truckType',                    truckType,                                // Type of the truck
        'via',							     				step->via,											 					// Point via name
        'viaPos',                       viaCpx,                                   // Point via position
        //'rti',							     				rti                                       // itinerary
        'startTime',                    0,                                        // Start task timestamp
    );
}

/**
    Function to compute different rpk mission objectives for VNA
    @param miss varpsc Mission varspc containing all variables
*/
export function [destinationCpx, palletSide] = computeRpkObjectivesVNA(pointData)
{
    v = varspc();

    idrob = robGetId;
    serial = number2Serial(idrob);
    forksCenterAbscissaLeftSide = getPreferenceValue("forksParameters/forksCenterAbscissaLeftSide", serial, 0);
    forksCenterAbscissaRightSide = getPreferenceValue("forksParameters/forksCenterAbscissaRightSide", serial, 0);
    aisleWidth = 1.96; //FROM DBS010 --> to change !

    //zstartCpx = pointData->zstart;
    //zendCpx   = pointData->zend;
    zstartCpx = cpx(pointData->zstart.(0).xposition, pointData->zstart.(0).yposition);
    zendCpx   = cpx(pointData->zend.(0).xposition, pointData->zend.(0).yposition);

    // Estimate the pallet center position on aisle
    palletVector = zstartCpx - zendCpx;
    palletPositionOnAisle = zstartCpx + palletVector / abs(palletVector) * aisleWidth * 0.5;
    //logDebug("[computeRpkObjectivesVNA] palletPositionOnAisle %z", palletPositionOnAisle);

    // Project the estimated position on track to have a track position
    palletTrackPositionOnAisle = TRACK_GetTrackPosition(getRobModel(idrob), palletPositionOnAisle);
    //logDebug("[computeRpkObjectivesVNA] palletTrackPositionOnAisle %z", palletTrackPositionOnAisle);
    if(isnan(palletTrackPositionOnAisle.curvilinearAbscissa) || palletTrackPositionOnAisle.curvilinearAbscissa <= 0)
    {
        error("[] Bad projection => check your aisle length");
    }

    // Get the portion information
    portionInfo = TRACK_GetPortionInfoFromID(getRobModel(idrob), palletTrackPositionOnAisle.portionId);

    // Calculate the aisle start and end point
    aisleStartPoint = cpx(portionInfo.start{0}.xposition, portionInfo.start{0}.yposition);
    aisleEndPoint   = cpx(portionInfo.end{0}.xposition, portionInfo.end{0}.yposition);

    // Calculate side based on vertor determinant
    // det(AB, AC) = ||AB||.||AC||.sin(AB, AC)
    aisleVector = aisleEndPoint - aisleStartPoint;
    startAislePalletVector = zstartCpx - aisleStartPoint;
    determinant = det(startAislePalletVector, aisleVector);
    if(determinant < 0)
    {
        side = "left";
        palletTrackPositionOnAisle.curvilinearAbscissa = palletTrackPositionOnAisle.curvilinearAbscissa - forksCenterAbscissaLeftSide;
    }
    else
    {
        side = "right";
        palletTrackPositionOnAisle.curvilinearAbscissa = palletTrackPositionOnAisle.curvilinearAbscissa - forksCenterAbscissaRightSide;
    }

    // Calculate the real position aimed by the roller
    destinationPosition = varspc();
    destinationPosition->position = aisleStartPoint + palletTrackPositionOnAisle.curvilinearAbscissa * iexp(portionInfo.cap);
    destinationPosition->xposition = destinationPosition->position.re;
    destinationPosition->yposition = destinationPosition->position.im;
    destinationPosition->cap = portionInfo.cap;
    palletSide = side;
    palletTrackPositionOnAisle = palletTrackPositionOnAisle;

    //logInfo("[computeRpkObjectivesVNA] destinationPosition %z", destinationPosition->position);
    //logInfo("[computeRpkObjectivesVNA] palletSide %z", palletSide);

    destinationCpx = destinationPosition->position;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                                ROBOT PROGRAM                                //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Program launched on a rob
    @param missId [long] -> id of the mission (from the list g->missions) (in this case from 0 to 2)
*/
export function robMiss3(missId,taskId) {
    global g('simConf');
    idrob = robGetId;

    // Retrieve task from global varspace
    missId = str2num(missId);
    taskId = str2num(taskId);
    v = g->missions.{missId};

    // Launch start callback
    taskStartCbk(taskId, v->title, idrob);

    // Stop robot at the beginning
    robSetvmax(0.0);

    // Statistiques
    startTime   = simGetT();
    endOfStep   = [startTime];
    flows       = v->flows;
    oldFlowStep = 0;
    flowStep    = 0;

    // Get mission steps
    missionSteps = v->objectives;

    //initialize pallet id
    idPal = 0;

    // Run each step
    k = for(0,size(missionSteps)-1) {
        step = missionSteps.{k};

        //update step in sdk schd
        simWait(1.1); //mandatory sim wait to be sure schd is updated
        tskSetStep(robFindTaskId(idrob),k+1);

        //check optional parameters
        if(!isvar(step,"via")) step->via = ''; //default configuration = no via
        if(!isvar(step,"pallet")) step->pallet = g->pallets.{0}.{0}; //default configuration = first pallet type
        if(!isvar(step,"nbPal")) step->nbPal = 1; //default configuration = 1 pallet
        if(!isvar(step,"slot")) step->slot = -1; //default configuration = no slot defined (random)
        if(!isvar(step,"strategy")) step->strategy = 'random'; //default configuration = random
        if(!isvar(step,"approachTime")) step->approachTime = -1; // default configuration = automatic
        if(!isvar(step,"detectionTime")) step->detectionTime = -1; // default configuration = automatic
        if(!isvar(step,"destinationTime")) step->destinationTime = -1; //default configuration = automatic

        // Run move task
        if(!strcmp(step->task,'move')) {
            if(isKmatic) missMoveVNA(step,step->dst,idPal);
            else missMove(step,step->dst,idPal);
        }
        
        // Run pick/drop task
        else {
            //------ if there is data in the csv file the dst is selected from there, if there is not, the dst is selected randomly-----------------
            logInstall("The missId: %z and taskId:%z", missId, taskId);
            if( strMatch(step->task, "pick") && !isempty(DBget_gbl("simSchdDB",(taskId),"srcPoint")))
            {
                if(isempty(str2num(DBget_gbl("simSchdDB",(taskId),"srcPoint"))))    // If the point is selected from the id or from the name of the point
                {
                    dst = DBget_gbl("simSchdDB",(taskId),"srcPoint");
                }
                else{dst = id2name(str2num(DBget_gbl("simSchdDB",(taskId),"srcPoint")));}

                logWarn("The src: %z for picking is selected from the csv file", dst);
            }
            else if(strMatch(step->task, "drop") && !isempty(DBget_gbl("simSchdDB",(taskId),"dstPoint")))
            {
                if(isempty(str2num(DBget_gbl("simSchdDB",(taskId),"dstPoint"))))    // If the point is selected from the id or from the name of the point
                {
                    dst = DBget_gbl("simSchdDB",(taskId),"dstPoint");
                }
                else{dst = id2name(str2num(DBget_gbl("simSchdDB",(taskId),"dstPoint")));}

                logWarn("The dst: %z for dropping is selected from the csv file", dst);
            }
            else    // There is no data in the csv file so the points are selected randomly
            {
                //choose destination according to type / grp / strategy                
                if(!isvar(step,"dst") && isvar(step,"grp")) dst = chooseRealDestination(step->grp,step->strategy, step->task);
                else dst = step->dst;
                logWarn("The dst: %z is selected randomly", dst);
            }
            
            //get point data
            if(!isNumeric(dst)) dstId = name2id(dst);
            else dstId = dst;
            pntData = missPntGetInfo(dstId);
            missType = pntData->type;

            switch(missType) {
                case('slotZone', 'slot');
                    if(isKmatic) displayError(sprintf('[ROB %i] Kmatic are not allowed to do slot mission (%s) !',idrob,v->title));
                    idPal = missSlot(step,dst,idPal);
                case('elevated','rack','conveyor','move');
                    if(isKmatic) idPal = missElevatedVNA(step,dst,idPal);
                    else idPal = missElevated(step,dst,idPal);
                case('line', 'lineZone');
                    idPal = missLine(step,dst,idPal);
                default;
                logWarn("UNKNOWN TYPE OF MISSION %z", missType);
                    displayError(sprintf(['Unknown type of mission (',missType,')'])); // Unknown type of mission
            }
        }


        //store end of step time
        endOfStep.(end+1) = simGetT();

        // Update stats if we just finished the flow
        if (k == flows.{flowStep}->numberOfStep + oldFlowStep - 1) {
            // Compute mission duration
            flowDuration = long(round(endOfStep.(end) - endOfStep.(oldFlowStep)));
            // Get flow name
            flowName = flows.{flowStep}->flowName;
            // Get the number of pal for the flow
            pals = getFlowPal(v, flowStep, k);
            // Update flow stats
            if (endOfStep.(end) > g->startCollectData) {
                tcreate = flowStep == 0 ? robFindTaskCreate(robGetId) : endOfStep.(oldFlowStep);
                statsUpdate(flowName,tcreate, endOfStep.(oldFlowStep), endOfStep.(end), pals);
                // Update simResultsDB
                simResultsAdd(taskId,flowName,v->title,flows.{flowStep}->flowDisplay,tcreate, endOfStep.(oldFlowStep), endOfStep.(end), pals, robGetId);
            }
            // Update variables
            oldFlowStep  = k + 1;
            flowStep     = flowStep + 1;
        }

    }

    taskDoneCbk(taskId,v->title,idrob);
}

/*
    Chooses one point among a group of point, according to a filling / emptying strategy
    @param grp [char] -> a point/grp name
    @param strategy [char] -> 'random', 'equally', 'custom'
    @return dst [char] -> destination point name
*/
export function dst = chooseRealDestination(grp,strategy, step) {
    global g('simConf');    

    if(!isempty(grp)) {
        //var to store all available points
        pntLs = new('long',0);

        //loop over grp if several declared
        i = for(0,size(grp)-1) {
            //find the grp
            if(istype(grp,'char')) ids = DBfind_gbl('stkDB','groupName','==',grp);
            else ids = DBfind_gbl('stkDB','groupName','==',grp.{i});

            if(!isempty(ids)) {
                //find type from first id
                type = missPntGetInfo(ids.(0))->type;

                //define pnt list
                if(strMatch(type,'line') || strMatch(type,'slot')) { //get scan list from pnt configuration
                    pntLs = [pntLs, missPntGetInfo(ids.(0))->scanList.{0}];
                }
                else if(strMatch(type,'elevated')) { //dst list is already equal to ids
                    pntLs = [pntLs, ids];
                }
                else displayError(sprintf('[chooseRealDestination] Error in type : %s',type));
            }
            else {
              if(istype(grp,'char')) displayError(sprintf('Group %s does not exist in stkDB !',grp));
              else displayError(sprintf('Group %s does not exist in stkDB !',grp.{i}));
            }
        }
        // logWarn("[chooseRealDestination] The list of possible points is: %z", pntLs);
    }
    else  {
        //displayError(sprintf('Variable "grp" is empty, please check your missions !',grp));
        //return;
        logWarn("[chooseRealDestination] There has been an error. There are not values in the .c file for this mission. It is: %z", pntLs);
    }

    //get size of list
    nbPnt = size(pntLs);

    //compute dst from strategy
    switch(strategy) {
            // random
        case('random');
            // random index
            idx = rand()%nbPnt;
            // return the selected pnt
            dst = id2name(pntLs.(idx));

            // other
        case('equally');
            //TODO
            displayError('[chooseRealDestination] Strategy "equally" not developed yet !');

        case('custom');
            // write here you code
            // you are the best
            // will you marry me
    }

    //  logInstall('[chooseRealDestination] with Strategy %s. Dst %s chosen from group %z',strategy,dst,grp);

}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                           PICK AND DROP FUNCTIONS                           //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Parameters of the pick/drop functions :
    @param step [varspc] -> task varspc (mission step)
    @param destination [char] -> destination point name
    @param missionType [char] -> 'slot', 'line', 'move', 'elevated', 'elevatedVNA'
    @param idPal [long] -> id of the pallet
*/


/*
    Pick or drop in a single slot
*/
export function id = missSlot(step,destination,idPal) {

    // Initialization of the mission varspc
    miss = initMissVarspc(step,destination,'slot',idPal);
    // start agv movement and evacuation
    startMission(miss);
    // manage evacuation
    goToEvacuation(miss);
    // navigate to rpkLastAR and setReversemode
    goToLastReverse(miss);
    //navigate to rpk approach and move forks (ONLY PICK)
    if(!strcmp(step->task,'pick') || step->detectionTime > 0 || step->approachTime > 0) goToRpkApproach(miss);
    //navigate to rpk Destination
    goToDestination(miss);
    //update scheduler and stats
    id = endTask(miss);
}


/*
    Pick or drop in a single line
*/
export function id = missLine(step,destination,idPal) {
    //TODO: Compute line number and number of slot for each lines

    // Initialization of the mission varspc
    miss = initMissVarspc(step,destination,'line',idPal);
    // start agv movement and evacuation
    startMission(miss);
    // manage evacuation
    goToEvacuation(miss);
    // navigate to rpkLastAR and setReversemode
    goToLastReverse(miss);
    // compute fake scan results
    //miss = chooseLineDest(miss);
    //navigate to rpk approach and move forks (ONLY PICK)
    if(!strcmp(step->task,'pick') || step->detectionTime > 0) goToRpkApproach(miss);
    // navigate to rpk Destination
    goToDestination(miss);
    //update scheduler and stats
    id = endTask(miss);
}


/*
    Pick or drop in a single elevated position (conveyor, rack, others)
*/
export function id = missElevated(step,destination,idPal) {

    // Initialization of the mission varspc
    miss = initMissVarspc(step,destination,'elevated',idPal);
    // start agv movement and evacuation
    startMission(miss);
    // manage evacuation
    goToEvacuation(miss);
    // navigate to rpkLastAR and setReversemode
    goToLastReverse(miss);
    // navigate to rpk approach and move forks
    goToRpkApproach(miss);
    // navigate to rpk Destination
    goToDestination(miss);
    // update miss scheduler and stats
    id = endTask(miss);
}


/*
    Pick or drop in a single elevated position (VNA only)
*/
export function id = missElevatedVNA(step,destination,idPal) {

    // Initialization of the mission varspc
    miss = initMissVarspc(step,destination,'elevatedVNA',idPal);
    // start agv movement and evacuation
    startMission(miss);
    // rotate forks if needed (VNA)
    rotateForksVNA(miss);
    // manage evacuation
    // goToEvacuation(miss); TODO
    // navigate to rpkLastAR and setReversemode
    // goToLastReverse(miss);
    // navigate to rpk Destination
    goToDestination(miss);
    // update miss scheduler and stats
    id = endTask(miss);
}


/*
    Move to a single destination position
*/
export function missMove(step,destination,idPal) {

    // Initialization of the mission varspc
    miss = initMissVarspc(step,destination,'move',idPal);
    // start agv movement and evacuation
    startMission(miss);
    // manage evacuation
    goToEvacuation(miss);
    // navigate to rpkLastAR and setReversemode
    goToLastReverse(miss);
    // navigate to rpk approach
    goToDestination(miss);
    // update miss scheduler and stats
    endTask(miss);
}

/*
    Move to a single destination position (VNA only)
*/
export function missMoveVNA(step,destination,idPal) {

    // Initialization of the mission varspc
    miss = initMissVarspc(step,destination,'moveVNA',idPal);
    // start agv movement and evacuation
    startMission(miss);
    // rotate forks if needed (VNA)
    rotateForksVNA(miss);
    // manage evacuation
    // goToEvacuation(miss); TODO
    // navigate to rpkLastAR and setReversemode
    // goToLastReverse(miss);
    // navigate to rpk approach
    goToDestination(miss);
    // update miss scheduler and stats
    endTask(miss);
}

/*

export function id = missMulti(type,strategy,way,destination,via) {

	// Initialization of the mission varspc
	miss = initMissVarspc(destination,'move',via);
	//LOOP single missions
	if(!strcmp(way,'furthest')) {
	//
	}
	else {
		//
	}
	switch(strategy) {
		case('equality');
		case('line');
		case('random');
	}

	// update miss scheduler and stats
  id = endTask(miss);
}
*/


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                             NAVIGATION FUNCTIONS                            //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Start the navigation of the robot
    @param miss [varspc] -> mission varspc
*/
export function startMission(miss) {
    global g('simConf');
    global r('simRob');
    idrob = robGetId;

    logInstall('[%s] : Start on rob %i (destination %s)',miss->taskName,robGetId,miss->destination);

    // send rob to destination point
    moveToDestination(miss);

    //set rpkBeforeLastReverse
    miss->rpkBeforeLastReverse = robGetRpkLastAR();
    if(miss->rpkBeforeLastReverse > robGetrpk) miss->rpkBeforeLastReverse = -1;

    // Set canton and counter for tugger
    if(isTugger(idrob)) {
        cantonSetTrailerLength(robGetId,(miss->nbPal*miss->dimPal.re));
    }

    // Save taxi time
    saveTaxiTime(robGetId);

    // store startTime for counters
    miss->startTime = simGetT();

    // set destination in robot global
    r->currentDst.{idrob-1} = miss->destination;

}


/*
  Send robot to destination point
  @param miss [varspc] -> mission varspc
*/
export function moveToDestination(miss) {

    // make sure robot is stopped before create itinerary //TODO make a variable for the speed limit
    if(abs(robGetSpeed) > 0.2) {
        robSetvmax(0.0);
        logDebug('[moveToDestination] Speed = %z - Need to stop (rob %i)',robGetSpeed(),robGetId());
        while(abs(robGetSpeed()) > 0.2) simWait(0.1);
    }

    //check that itinerary exists
    [i,j] = TRACK_FindItinerary(getRobModel(robGetId),robGetZ,miss->destiPos,miss->pathFlag);
    if(isempty(i) && isempty(j)) {
        displayError(sprintf('[moveToDestination] Cannot find itinerary from rob %i (%z) to %s (%z)',robGetId,robGetZ,miss->destination,miss->destiPos));
        logError('Check your circuit and your missions.');
        return;
    }

    // we aim for the end of line and we will stop before (green point)
    // with a via point
    if(!isempty(miss->viaPos)) {
        gotoPnt(miss->destiPos,miss->pathFlag,0.0,miss->viaPos);
    }
    //without via
    else {
        gotoPnt(miss->destiPos,miss->pathFlag,0.0);
    }
}


/*
  Manage line evacuation
  @param miss [varspc] -> mission varspc
*/
export function goToEvacuation(miss) {
    // flag 1: itinerary need reset 0: no need to reset the itinerary
    resetIti = 0;

    //logInstall("[EVACUATION] START - robPos %z, evacPos %z",robGetZ,miss->evacPos);

    // set evacuation speed
    robSetvmax(miss->vmaxEvac);

    // if evacuation point exist -> go to point
    if(!isempty(miss->evacPos)) {
        // if evac point is ahead of the robot (no need to reset iti)
        if(TRACK_isPointInPath(getRobModel(robGetId),robGetZ,miss->destiPos,miss->evacPos,miss->pathFlag)) {
            // wait for reach end of evacuation (if dist non nulle)
            if((robGetrpk - miss->evacDst) > 0.0) robWaitrpk(robGetrpk - miss->evacDst);
            // move forks to travel height
            robMoveForksTravel(miss,'Forward','evac');
        }
        else {
            // evac point is behind truck (need to leave the stock line)
            [i,j] = TRACK_FindItinerary(getRobModel(robGetId),robGetZ,miss->evacPos,miss->pathFlag);
            if(!isempty(i) || !isempty(j)) {
                // we would need to reset iti has we change it for the evacuation
                resetIti = 1;
                // launch robot to evacuation point
                gotoPnt(miss->evacPos,miss->pathFlag);
                // wait to be at destination
                robWaitrpk(0.0);
                robStop();
            }
            else logWarn('[goToEvacuation] Error in evac itinerary! Skip evacuation.');
            // move forks to new height directly
            robMoveForksTask(miss->heightApproach,miss->iliftApproach,0,'evac');
        }
    }
    else {
        logWarn('[goToEvacuation] evacPos not defined for miss %s (rob %i)',miss->taskName,robGetId);
    }

    //logInstall('[EVACUATION] END -');

    // reset iti if needed
    if(resetIti) moveToDestination(miss);

    // set max forward speed
    robSetvmax(miss->vmax);
}


/*
    Navigate to last reverse
    @param miss [varspc] -> mission varspc
*/
export function goToLastReverse(miss) {

    // navigate to last reverse
    if(miss->rpkBeforeLastReverse > 0) {
        // wait for robot to stop at rpk last reverse
        robStoprpk(miss->rpkBeforeLastReverse);
        // lift forks for reverse movement (if needed)
        robMoveForksTravel(miss,'Backward','lastAR');
        // reset max speed
        robSetvmax(miss->vmax);
    }

    //TODO create pallet if pick mission

}


/*
    Navigate to rpk approach
    @param miss [varspc] -> mission varspc
*/
export function goToRpkApproach(miss) {

    // wait for robot to stop at rpk approach
    if(miss->rpkApproach >= 0) robStoprpk(miss->rpkApproach);

    //* * FORKS MOVEMENTS * *
    // if destinationTime defined in mission wait for destinationTime
    if(miss->approachTime >= 0) simWait(miss->approachTime);
    // otherwise simulate frk mvt time at destination
    else {
        // move forks
        robMoveForksTask(miss->heightApproach,miss->iliftApproach,1,'approach');
    }

    //* * DETECTION MOVEMENTS * *
    if(miss->detectionTime >=0) simWait(miss->detectionTime); //TODO if condition not needed

    // relaunch agv to destination
    robSetvmax(miss->pointData->VMAX_REVERSE_SLOW); //enter the position at slow speed to finish mission
}


/*
    Navigate to rpk destination
    @param miss [varspc] -> mission varspc
*/
export function goToDestination(miss) {

    // wait for robot to stop at rpk approach
    robStoprpk(miss->rpkDestination);

    // stop robot
    robStop();

    //* * FORKS MOVEMENTS * *
    // if destinationTime defined in mission wait for destinationTime
    if(miss->destinationTime >= 0) simWait(miss->destinationTime);
    // if kmatic special mvt
    else if(isKmatic) simWait(getFrkMvtVNA(miss)); //TODO gerer le mvt de cabine avec le K
    // otherwise simulate frk mvt time at destination
    else {
        // move forks
        robMoveForksTask(miss->heightDestination,miss->iliftDestination,1,'destination');
    }
}


/*
    Slow down and stop a robot
    @param pk [float] -> rpk where the robot has to stop
*/
export function robStoprpk(pk) {

    //get the distance between the robot and the destination
    rpk = robGetrpk() - pk;
    //special behavior if distance < 2000 mm
    if(rpk<=2) {
        robStoprpk_mini(pk);
        return;
    }

    robWaitrpk(pk+2);	robSetvmax(0.5);
    robWaitrpk(pk+1); 	robSetvmax(0.4);
    robWaitrpk(pk+0.5);	robSetvmax(0.2);
    robWaitrpk(pk+0.2); robSetvmax(0.1);
    robWaitrpk(pk+0.01); robSetvmax(0.0);
    //robStop();
}


/*
    Slow down and stop a robot (only for small mvt (<2m))
    @param pk [float] -> rpk where the robot has to stop
*/
export function robStoprpk_mini(pk) {
    robSetvmax(0.25);
    robWaitrpk(pk+0.5);	robSetvmax(0.2);
    robWaitrpk(pk+0.2); robSetvmax(0.1);
    robWaitrpk(pk+0.01); robSetvmax(0.0);
    //robStop();
}

/*
    Navigate to batt point
    @param miss [varspc] -> mission varspc
*/
export function goToBatt(miss) {
    global b('simBatt');

    // wait for robot to stop at rpk approach
    robStoprpk(miss->rpkDestination);

    // stop robot
    robStop();

    idrob = robGetId;
    b->robInCharge.(idrob-1) = 1;

    // store moving to battery time
    endBattMoveTime(idrob, miss->startTime);
    miss->startTime = simGetT(); // reset start time

    //* * FORKS MOVEMENTS * *
    // lower forks
    robMoveForksTask(0.0,0,1,'batt_stop');
    // if destinationTime defined in mission wait for destinationTime
    if(miss->destinationTime >= 0) simWait(miss->destinationTime);
    // if kmatic special mvt
    else if(isKmatic) simWait(getFrkMvtVNA(miss)); //TODO gerer le mvt de cabine avec le K
    // otherwise simulate frk mvt time at destination
    else {
        // move forks
        robMoveForksTask(miss->heightDestination,miss->iliftDestination,1,'destination');
    }

    b->robInCharge.(idrob-1) = 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                          MISSION RELATED CALLBACKS                          //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    End of the task (pallet management + counters)
    @param idrob [long] -> id of the robot
    @param startTime [long] -> task start time
*/
export function id = endTask(miss) {
    idrob = robGetId;
    //
    idPnt = miss->pointData->id;
    cap = robGetcap(idrob);
    z = robGetz(idrob);

    // robTake or robDrop according to mission type
    if(!strcmp(miss->task,'pick')) {
        id = palNew(miss->pallet,z,cap,0); //todo : check where the pallet is created
        robTake(id);
        endEmptyCounter(idrob,miss->startTime);
    }
    else if (!strcmp(miss->task,'drop')) {
        robDrop();
        palDelete(miss->idPal);
        id = 0; //no pal
        endCarryCounter(idrob,miss->startTime);
    }
    else if (!strcmp(miss->task,'move')) {
        if(miss->nbPal > 0) endCarryCounter(idrob,miss->startTime); //check if we carry pallets during the move
        else endEmptyCounter(idrob,miss->startTime);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                          MISSIONS USEFUL FUNCTIONS                          //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Manually launch a mission from its name
    @param taskName [char] -> name of the function to launch
*/
export function launchMiss(taskName, varsCSV) {
    global g('simConf');
    //if(nargin<2) force = 0;

    v = varspc();

    idMiss = new('char',0);
    idTemp = new('long',0);
    group = new('char',0);
    unint missions = g->missions;
 
    //find the data of the mission from the mission list
    m = for(missions) {
        if(!strcmp(missions.{m}->title,taskName)) {
            idTemp = m;
            break;
            logInstall("[launchMiss] Name of the mission to launch: %z", taskName);
        }
        
    }
 
    //if mission is not found return an error
    if(isempty(idTemp)) displayError(sprintf('Unknow mission %s',taskName));

    //if exist get data of the mission
    if(!isvar(missions.{idTemp},'priority')) priority = '1';
    else priority = num2str(missions.{idTemp}->priority);

    idMiss = num2str(idTemp);

    //add the mission to the simulation scheduler
    addTaskInPcsol(taskName,idMiss,priority, varsCSV);
}


/*
    This function returns matching agvs between available agvs (lrob) and authorized agvs (authorizedAgvs)
    @param lrob [long_cell] -> list of all id robs
    @param authorizedAgvs [long_cell] -> list of authorized id robs
    @return agvs [long_cell] -> selected id robs
*/
export function agvs = selectAgvs(lrob, authorizedAgvs) {
    // Initialization
    agvs = [];
    // Research authorized agvs
    k = for(0,size(lrob)-1) {
        j = for(0,size(authorizedAgvs)-1) {
            if (lrob.(k) == authorizedAgvs.(j))
                agvs.(end+1) = lrob.(k);
        }
    }
}
