
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                            STATS INITIALIZATION                             //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Initialisation de la variable globale enregistrant les statistiques
    @param flux [L.varspc] -> list of flows varspc ("flow" = "flux")
*/
export function initStats(flux){
    global g('simConf');

    typedef('metrics','flux:name tassign_moy:double tassign_max:long tmiss_moy:double  tmiss_max:long tatt:long nmiss:long npal:long npalh:double ttotal:long');
    // Definition of "stats type :
    //  flux : task name
    //  nrob : number of robots
    //  tassign_moy et tassign_max : mean and max assignment time (for missions)
    //  natt_moy et natt_max : mean and max number of waiting mission
    //  nmiss : number of missions done
    //  ttotal : total time passed in mission

    g->stats = new('metrics',0);
    g->statsNames = {
        'Flow',
        'Avg. assign\ntime','Max. assign\ntime',
        'Avg. mission\ntime','Max. mission\ntime',
        'No. missions\n done',
        'No. pals', 'No. pals / h'
    };

    k = for(flux){
        g->stats.flux(k) = flux.{k};
        g->stats.tassign_moy(k) = 0.;
        g->stats.tassign_max(k) = 0;
        g->stats.tmiss_moy(k) = 0.;
        g->stats.tmiss_max(k) = 0;
        g->stats.nmiss(k) = 0;
        g->stats.npal(k) = 0;
        g->stats.npalh(k) = 0;
        g->stats.ttotal(k) = 0;
        g->stats.tatt(k) = 0;
    }
}


/*
    Function that creates results DB and delete all previous records if any
*/
export function initSimResults() {
    logInfo('[SCHD] Initializing custom simResultsDB');

    // get DB definition
    dbDef = simResultsDBDef();

    // Create DB from definition
    DBcreate_gbl('simResultsDB',dbDef,'id',0);

    // if keys, delete them all to re-start fresh
    keys = DBgetAllKeys_gbl('simResultsDB');
    if (size(keys)) {
        //logDebug('[RESULTS] Deleting old keys %z',keys);
        DBdel_gbl('simResultsDB',keys);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                           COUNTERS FUNCTIONS                                //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


/*
    End the empty counter
    @param idrob [long] -> id of the robot
    @param startTime [long] -> time stamp empty
*/
export function endEmptyCounter(idrob, startTime) {
    global g('simConf');
    global r('simRob');
    simuTime = simGetT();
    // if we are after startCollectData, then update counters emptyStart and totalEmptyTime
    if(simuTime > g->startCollectData) {
        startTime = max(startTime,g->startCollectData); //record time only from startCollectData
        r->timeEmpty.(idrob-1) = r->timeEmpty.(idrob-1) + (simuTime - startTime);
    }
}


/*
    End the carry counter
    @param idrob [long] -> id of the robot
    @param startTime [long] -> time stamp carry
*/
export function endCarryCounter(idrob, startTime) {
    global g('simConf');
    global r('simRob');
    simuTime = simGetT();
    // if we are after startCollectData, then update counters carryStart and totalCarryTime
    if(simuTime > g->startCollectData) {
        startTime = max(startTime,g->startCollectData); // record time only from startCollectData
        r->timeCarry.(idrob-1) = r->timeCarry.(idrob-1) + (simuTime - startTime);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                            RESULTS DB DEFINITION                            //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    simResultsDB definition
    @return def [varspc] -> simResultsDB definition varspc
*/
export function def = simResultsDBDef() {
    def = varspc;
    def->id = 0;              // DB key, unique id
    def->schdId = 0;          // Scheduler id
    def->flowName = strn(''); // Name of the flow
    def->taskName = strn(''); // Name of the task
    def->flowDisplay = 1;     // Display the flow in the results
    def->addDate = 0;         // timestamp for add date
    def->startDate = 0;       // timestamp for start date
    def->endDate = 0;         // timestamp for end date
    def->npal = 0;            // No of pallets
    def->rob = 0;             // rob running the task
}


/*
    Function to add an entry in the results DB
    @param schdId [long] -> id of the task (simSchdDB key)
    @param flowName [strn] -> Name of the flow
    @param taskName [strn] -> Name of the task
    @param tcreate [long] -> Timestamp for add time
    @param tdebut [long] -> Timestamp for start time
    @param tfin [long] -> Timestamp for end time
    @param pals [long] -> No of pallets
    @param rob [long] -> rob running the task
*/
export function simResultsAdd(schdId,flowName,taskName,flowDisplay,tcreate, tdebut, tfin, pals,rob) {
    // Create DB entry
    v = varspc();
    v->schdId = schdId;           // Scheduler id
    v->flowName = strn(flowName); // Name of the flow
    v->taskName = strn(taskName); // Name of the task
    v->flowDisplay = flowDisplay; // Display the flow in the results
    v->addDate = tcreate;         // timestamp for add time
    v->startDate = tdebut;        // timestamp for start time
    v->endDate = tfin;            // timestamp for end time
    v->npal = pals;               // No of pallets
    v->rob = rob;                 // rob running the task

    // Add entry to DB
    DBadd_gbl('simResultsDB',v);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                         EXPORT AND RESULTS FUNCTIONS                        //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Return the simulation parameters in a string and print it
    @param returnContent [bool] -> 1 content is only returned, 0 content is also printed
    @return res [char] -> simulation parameters
*/
export function res = writeSimParam(returnContent) {
    id = getSimxGlobalId();
    global w(id);
    global g('simConf');

    //
    if(nargin<1) returnContent = 0;
    res = '';

    //
    offsetTime = isvar(g,'startTime') ? g->simulationStartTime : g->startCollectData;
    runTime = isvar(g,'startTime') ? simGetT - g->simulationStartTime : g->simulationRunTime;

    //
    res = [res, 'SIMULATION PARAMETERS :\n'];
    res = [res, '    Date : ',sprintDate('%x - %X',time()),'\n'];
    res = [res, '    SDK : ',formatString(scmVersionInfo(), 22, ' '),'\n'];
    res = [res, '    Circuit : ',fnameConc(w->env->proj,[w->env->open]),'\n'];

    if(g->enableBattMiss) {
      res = [res, '    Battery Missions : Enabled (',g->battPntStrategy,];
      if(g->enableOPC) res = [res, ' - with OPC)','\n'];
      else res = [res, ' - without OPC)','\n'];
    }
    else res = [res, '    Battery Missions : Disabled','\n'];

    res = [res, '    Mission scheduler : ',g->scheduler,'\n'];
    res = [res, '    Offset Time : ',sprintHMS(offsetTime),'\n'];
    res = [res, '    Run Time    : ',sprintHMS(runTime),'\n'];
    res = [res, '    Robots :\n'];

    robSerial = getPreferenceValue('general/trucksSerial');
    res = [res,'       ',formatString("Serial", 20, ' '),' | ',formatString("Model", 18, ' '),' | ',
           formatString("Battery", 8, ' '),' | ',formatString("Forward", 9, ' '),' | ',
           formatString("Backward", 9, ' '),' | ',formatString("Lateral max speed (m/s)", 25, ' '),'\n'];
    k = for(0,g->nRob-1) {
        batt = formatString(char(getPreferenceValue("battery/hardware", robSerial.(k))), 8, ' ');
        model = formatString(char(getPreferenceValue('general/modelFileName',robSerial.(k))), 18, ' ');
        vmax = formatString(char(num2str(getPreferenceValue("trajectoryDriver/speedLimit/maxForwardSpeed",robSerial.(k)))), 9,' ');
        vmaxar = formatString(char(num2str(getPreferenceValue("trajectoryDriver/speedLimit/maxBackwardSpeed",robSerial.(k)))), 9,' ');
        wcapmax = formatString(char(num2str(getPreferenceValue("trajectoryDriver/speedLimit/maxLateralSpeed",robSerial.(k)))), 25,' ');
        rob = formatString(char(robSerial.(k)), 20, ' ');
        res = [res,'       ', rob,' | ',model,' | ',batt,' | ',vmax,' | ',vmaxar,' | ',wcapmax,'\n'];
    }

    res = [res,'  \n'];

    if (!returnContent)
    {
        printcf('red',res);
    }
}


/*
    Write the results in the console
    @param type [char] -> type of results to be printed : MISSION, FLOW or RESULTS
    @param v [varspc] -> mission definition varspc
*/
export function writeFlowConfig(type,v) {
    id = getSimxGlobalId();
    global w(id);
    global g('simConf');
    global b('simBatt');
    global r('simRob');

    // Separate data
    flowName = v->flowName;
    flowOutput = v->flowOutput;

    // Start building the header
    switch(toupper(type)) {
        case('MISSION');
            // generate header
            header  = 'MISSION NAME';
            sep     = '------------';
            flowEnable = v->enable;
        case('FLOW');
            // generate header
            header  = 'FLOW NAME';
            sep     = '---------';
            pals    = v->enable;
        case('RESULTS');
            // generate header
            header  = 'FLOW NAME';
            sep     = '---------';
            results = v->results;
            resultsPerHour = v->resultsPerHour;
        default;
            displayError(sprintf(['Unknown type ',type,' for writeFlowConfig']));
    }

    maxSize = size(header);
    i = for(0,size(flowName)-1) {
        if (size(char(flowName.(i))) > maxSize)
            maxSize = size(char(flowName.(i)));
    }

    header = formatString(header, maxSize, ' ');
    sep    = formatString(sep   , maxSize, '-');

    //
    switch(toupper(type)) {
        case('MISSION');
            header = [header,' | ENABLE | CADENCE \n'];
            sep    = [sep,   '-|--------|---------\n'];
        case('FLOW');
            header = [header,' | CADENCE | PAL/H \n'];
            sep    = [sep,   '-|---------|-------\n'];
        case('RESULTS');
            header = [header,' | OBJECTIVE AT 85% | No PAL'];
            sep    = [sep,   '-|------------------|-------'];
            // Display results with utilisation ratio
            unint utilizationRatioTable = g->utilizationRatioTable;
            k = for(0,size(utilizationRatioTable)-1) {
                header = [header,' | PAL/H at ',sprintf('%.0f',utilizationRatioTable.(k)*100),'%'];
                sep    = [sep,   '-|-------------'];
            }
            header = [header,'\n'];
            sep    = [sep,'\n'];
        default;
            displayError(sprintf(['Unknown type ',type,' for writeFlowConfig']));
    }

    // Print header
    logInstall('[CONFIG] %z configuration : \n',toupper(type));

    // Prepare the results file to be written
    if (!strcmp(toupper(type), 'RESULTS')) {
        content1 = '';
        content2 = '';
        resLine1    = 'AGV             ';
        resLine2a   = 'Moving Taxi time';
        resLine2b   = 'Static Taxi time';
        resLine2c   = 'Moving Batt time';
        resLine2d   = 'Static Batt time';
        resLine2e   = 'Total Taxi&Batt ';
        resLine3a   = 'Empty trips time';
        resLine3b   = 'Carry trips time';
        resLine4    = 'Traffic time    ';
        resLineSep  = '----------------';


        k = for(0,g->nRob-1) {
            saveTaxiTime(k+1);
            resLine1 = [resLine1,' | ',formatString(num2str(k+1), 9, ' ')];
            resLine2a = [resLine2a,' | ',formatString([sprintf('%.01f',r->timeTaxiMove.(k)*100.0/(simGetT - g->simulationStartTime)),'%'], 9, ' ')];
            resLine2b = [resLine2b,' | ',formatString([sprintf('%.01f',r->timeTaxiStop.(k)*100.0/(simGetT - g->startCollectData)),'%'], 9, ' ')];
            numBattMissDone = (g->enableBattMiss) ? b->numBattMissDone.(k) : 0;
            resLine2c = [resLine2c,' | ',formatString([sprintf('%.01f',r->timeBattMove.(k)*100.0/(simGetT - g->startCollectData)),'%'], 9, ' ')];
            resLine2d = [resLine2d,' | ',formatString([sprintf('%.01f',r->timeBattStop.(k)*100.0/(simGetT - g->startCollectData)),'% ', sprintf('(%i)', numBattMissDone)], 9, ' ')];
            resLine2e = [resLine2e,' | ',formatString([sprintf('%.01f',(r->timeTaxiMove.(k)+r->timeTaxiStop.(k)+r->timeBattMove.(k)+r->timeBattStop.(k))*100.0/(simGetT - g->startCollectData)),'%'], 9, ' ')];
            resLine3a = [resLine3a,' | ',formatString([sprintf('%.01f',r->timeEmpty.(k)*100.0/(simGetT - g->startCollectData)),'%'], 9, ' ')];
            resLine3b = [resLine3b,' | ',formatString([sprintf('%.01f',r->timeCarry.(k)*100.0/(simGetT - g->startCollectData)),'%'], 9, ' ')];
            resLine4 = [resLine4,' | ',formatString([sprintf('%.01f',g->trafficTime.(k)*100.0/(simGetT - g->startCollectData)),'%'], 9, ' ')];
            resLineSep = [resLineSep,'-------------'];
        }

        printcf('green','  %s\n',resLine1);
        content2 = [content2, resLine1, '\n'];

        printcf('green','  %s\n',resLineSep);
        content2 = [content2, resLineSep, '\n'];

        printcf('green','  %s\n',resLine2a);
        content2 = [content2, resLine2a, '\n'];

        printcf('green','  %s\n',resLine2b);
        content2 = [content2, resLine2b, '\n'];

        printcf('green','  %s\n',resLine2c);
        content2 = [content2, resLine2c, '\n'];

        printcf('green','  %s\n',resLine2d);
        content2 = [content2, resLine2d, '\n'];

        printcf('green','  %s\n',resLine2e);
        content2 = [content2, resLine2e, '\n'];

        printcf('green','  %s\n',resLineSep);
        content2 = [content2, resLineSep, '\n'];

        printcf('green','  %s\n',resLine3a);
        content2 = [content2, resLine3a, '\n'];

        printcf('green','  %s\n',resLine3b);
        content2 = [content2, resLine3b, '\n'];

        printcf('green','  %s\n',resLineSep);
        content2 = [content2, resLineSep, '\n'];

        printcf('green','  %s\n',resLine4);
        content2 = [content2, resLine4, '\n'];

        printcf('green','  %s\n\n',resLineSep);
        content2 = [content2, resLineSep, '\n\n'];


    }

    printcf('green','%s',['  ',header]);
    printcf('green','%s',['  ',sep]);

    // Print data
    k = for(0,size(flowName)-1) {
        flowNameStr = char(flowName.(k));

        flowNameStr = formatString(flowNameStr, maxSize, ' ');
        switch(toupper(type)) {
            case('MISSION');
                printcf('green','  %s |      %i | %.02f\n',flowNameStr,flowEnable.(k),flowOutput.(k));
            case('FLOW');
                printcf('green','  %s | %s | %.02f\n',flowNameStr,formatString(num2str(flowOutput.(k)), 7, ' '),pals.(k));
            case('RESULTS');
                resLine = sprintf('%s | %s | %s',flowNameStr,formatString(num2str(flowOutput.(k)), 16, ' '),formatString(num2str(results.(k)), 6, ' '));
                //
                i = for(0,size(utilizationRatioTable)-1) {
                    resLine = sprintf('%s | %s',resLine,formatString(num2str(resultsPerHour.(k)*utilizationRatioTable.(i)), 12, ' '));
                }
                resLine = [resLine,'\n'];
                printcf('green','  %s',resLine);
                content1 = [content1, resLine];
            default;
                displayError(sprintf(['Unknown type ',type,' for writeFlowConfig']));
        }

    }
    printcf('green',['  ',sep,'\n\n']);

    if (!strcmp(toupper(type), 'RESULTS')) {
        content = writeSimParam(1);
        content = [content,'\n\nSIMULATION RESULTS :\n\n',content2,'\n\n',header,sep,content1,sep];
        resLine = sprintf('%s | %s | %s',formatString('TOTAL', maxSize, ' '),formatString(num2str(sum(flowOutput)), 16, ' '),formatString(num2str(sum(results)), 6, ' '));
        //
        i = for(0,size(utilizationRatioTable)-1) {
            resLine = [resLine,' | ',formatString(sprintf('%.02f',sum(resultsPerHour)*utilizationRatioTable.(i)), 12, ' ')];
        }
        resLine = [resLine,'\n'];
        printcf('green','  %s\n',resLine);
        content = [content,resLine];
        filename = [sprintDate('%Y%m%d_%H%M%S',time()),'-',strSplit(w->env->proj,'\\').{end},'-',fnameName(strSplit(w->env->open,'\\').{end}),'.txt'];

        // check that folder "results" exist, create it otherwise
        if(!isdir(fnameConc(w->env->proj,'results'))) mkdir(fnameConc(w->env->proj,'results'));
        // save results in folder
        file = fnameConc(w->env->proj,'results',filename);

        // save .txt file
        saveText(file, content);
        logInfo('[writeFlowConfig] Results .txt file created.');

        // save .csv file
        generateCsvFile(file);

        // save .json file
        writeJsonResults(file);

        // save .csv file (v2)
        //writeCsvResults(file);

        // save logReplay file
        if(isFunc("endRobotManagerLogs")) acall("endRobotManagerLogs",file);
    }
}



/*
    Function that fetch results from DB and print the output in the kiwi console
*/
export function writeSimResults() {
    global g('simConf');

    // Initialization
    flow           = {}; // flow name
    pals           = []; // pal / h objective
    results        = []; // Total pal done
    resultsPerHour = []; // pal / h done
    totalMissTime  = 0;  // Total time in mission
    startTime  = 0;  // Total time in mission
    endTime  = 0;  // Total time in mission

    // Iterate over missions
    k = for(g->missions) {
        v = g->missions.{k};
        // If mission not enable, skip
        if(!v->enable || !strcmp(v->title, 'battery'))
            continue;
        // Find flow name and pals / h
        [fluxl, pall] = getResultsFlowPal(v);
        kk = for(fluxl) {
            idx = findElt(flow,fluxl.{kk});
            if (size(idx)) {
                // flow already exists, just add pal/h
                pals.(idx) = pals.(idx) + v->ratePerHour * pall.(kk);
            } else {
                // flow doesn't exists, add it with his pal/hkiwi
                flow.{end+1} = fluxl.{kk};
                pals.(end+1) = v->ratePerHour * pall.(kk);
                results.(end+1) = 0;
                resultsPerHour.(end+1) = 0.0;
            }
        }
    }

    // Fill results and totalMissTime from DB
    k = for(flow) {
        // Find all records with flowName in the results DB
        ids = DBfind_gbl('simResultsDB','flowName','==',strn(flow.{k}));
        data = DBget_gbl('simResultsDB',ids);

        // 20180705: Added check to see if flow has any data to try and avoid idxextract error upon completion of simulation
        if (isvar(data,'startDate') && isvar(data,'endDate') && !isempty(data->startDate) && !isempty(data->endDate)) {
            // Init variable to count agv running time
            if (!startTime) {
                startTime = data->startDate.(0);
                endTime = data->endDate.(0);
            }

            // Update startTime and endTime if needed
            startTime = min([data->startDate, startTime]);
            endTime = max([data->endDate, endTime]);

            // Count all pal from flowName
            i = for(data->flowName) {
                if (data->flowDisplay.(i))
                    results.(k) = results.(k) + data->npal.(i);
            }
        }
    }

    totalMissTime = endTime - startTime;
    //    logDebug('totalMissTime: %z',totalMissTime);
    // Calc resultsPerHour
    k = for(flow) {
        resultsPerHour.(k) = results.(k) * 3600.0 / totalMissTime;
    }

    // Print results
    v = varspc(
        'flowName',cell2Strn(flow),
        'flowOutput',pals,
        'results', results,
        'resultsPerHour', resultsPerHour
    );

    // Write results in the kiwi console
    writeSimParam();
    writeFlowConfig('RESULTS',v);


    // Creates an JSON file to save the results of the simulation
    id = getSimxGlobalId();
    global w(id);
    
    filename = [sprintDate('%Y%m%d_%H%M%S',time()),'-',strSplit(w->env->proj,'\\').{end},'-',fnameName(strSplit(w->env->open,'\\').{end}),'.txt'];
    // The results created file is used as a parameter in this function just to use the same name.
    CUSTOM_writeJsonResults(fnameConc(w->env->proj,'results', filename));
}


/*
    Generate results into a csv file
    @param file [char] -> name of the .txt result file
*/
export function generateCsvFile(file) {
    global g('simConf');

    statgrid = getStatsRows();
    unint header = g->statsNames;
    unint format = g->format;
    content = '';

    if (strcmp(format,'EU') && strcmp(format,'US')) {
        logWarn('Unknown format %s, please use "US" or "EU"',format);
        return;
    }

    switch(format) {
        case('US');
            colSep = ',';
        case('EU');
            colSep = ';';
    }

    // Print header
    k = for(0,size(header) - 1) {
        content = [content,strReplace(header.{k},'\n',' '),colSep];
    }
    content.(end) = [];
    content = [content,'\n'];

    // Print lines
    k = for(0,size(statgrid) - 1) {
        row = statgrid.{k};
        length = size(row)-1;
        j = for(0,length) {
            if(j == length-2)
                content = [content,char(num2str(row.{j})),colSep];
            else if(j == length-1)
                content = [content,char(num2str(row.{j})),colSep];
            else if(j == length) {
                switch(format) {
                    case('US');
                        content = [content,strReplace(num2str(row.{j}),',','.')];
                    case('EU');
                        content = [content,strReplace(num2str(row.{j}),'.',',')];
                }
            } else
                content = [content,char(row.{j}),colSep];

        }
        content = [content,'\n'];
    }

    file = strReplace(file,'.txt','.csv');
    saveText(file, content);
    logDebug('writing to csvfile');
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                       STATS & RESULTS USEFUL FUNCTIONS                      //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Count the number of pallets moved by a flows
    @param v [varspc] -> misssion definition varspc (from g->missions)
    @param flowStep [long] -> step of flow
    @param k [long] -> number of task included in the flowStep
    @return pals [long] -> number of pal moved during a flow (objective)
*/
export function pals = getFlowPal(v, flowStep, k) {
    // Initialization
    nbPal = 1; //default number of pal
    pals = 0;
    idx = lin(k + 1 - v->flows.{flowStep}->numberOfStep,k);
    missionSteps = v->objectives.(idx);
    onlyMove = 1;

    // Count pals for each step
    i = for(0,size(missionSteps)-1) {
        if(isvar(missionSteps.{i},'nbPal')) nbPal = missionSteps.{i}->nbPal; //if nbPal defined -> take this one
        switch(missionSteps.{i}->task) {
            case('drop');
                pals = pals + nbPal;
                onlyMove = 0;
            case('move');
                pals = pals + nbPal;
            default;
                onlyMove = 0;
        }
    }

    // if only move, assume its done with a tugger and count only 1 move
    if (onlyMove)
        pals = 1;
    else if (!v->flows.{flowStep}->flowDisplay)
        pals = 0;
}


/*
    Function to get flows and expected pals from mission definition
    @param  v     varspc  Mission data varspc
    @return flows L.char  List of flow names
    @return pal   L.long  List of flow expected output (pal/h)
*/
export function [flows, pal] = getResultsFlowPal(v) {
    // Initialization
    flows = {};
    pal   = [];
    tmp = 0;

    // Iterate over flows from mission
    k = for(0,size(v->flows)-1) {
        if (v->flows.{k}->flowDisplay)
        {
            // Save flow name
            flows.{end+1} = v->flows.{k}->flowName;
            // Save flow expected output
            tmp = tmp + v->flows.{k}->numberOfStep;
            pal.(end+1)   = getFlowPal(v, k, tmp-1);
        }
    }
}


/*
    Mise a jour des stats a la fin d'une mission
    @param flux [char] -> flow name
    @param tcreate [long] -> creation time of the mission (seconds)
    @param tdebut [long] -> start time of the mission (seconds)
    @param tfin [long] -> end time of the mission (seconds)
    @param pals [long] -> number of pallets moved
*/
export function statsUpdate(flux,tcreate, tdebut, tfin, pals){
    //    logDebug('flux: %z, tcreate: %z, tdebut: %z, tfin: %z',flux,tcreate, tdebut, tfin);

    global g('simConf');

    if (sum(g->stats.npal) == 0 && g->simulationStartTime < 0) {
        unint g->simulationStartTime = tdebut;
        printf('[%s] Starting data gathering at %s\n',sprintHMS(simGetT),sprintHMS(tdebut));
    }

    nflux = find(g->stats.flux == flux);

    //Mise � jour du nombre de robot et du temps total d'occupation des robots
    tmission = tfin-tdebut;
    g->stats.ttotal(nflux) = g->stats.ttotal(nflux)+tmission;

    //Mise � jour du temps d'assignation
    tassign = tdebut-tcreate;
    g->stats.tassign_moy(nflux) = (g->stats.tassign_moy(nflux) * g->stats.nmiss(nflux) + tassign)/(g->stats.nmiss(nflux)+1);
    if(tassign > g->stats.tassign_max(nflux))
        g->stats.tassign_max(nflux) = tassign;

    //duree de mission
    //ATTENTION on ne prend pas en compte le temps d'assignation (sinon modifier tMiss)
    tMiss = tfin - tdebut;
    g->stats.tmiss_moy(nflux) = (g->stats.tmiss_moy(nflux)*g->stats.nmiss(nflux) + tMiss)/(g->stats.nmiss(nflux)+1);
    if(tMiss > g->stats.tmiss_max(nflux)) g->stats.tmiss_max(nflux) = tMiss;

    //Mise � jour du nombre de missions
    g->stats.nmiss(nflux) = g->stats.nmiss(nflux) +1;
    g->stats.npal(nflux) = g->stats.npal(nflux) + pals;

    runningTime = simGetT - g->simulationStartTime;
    k = for(g->stats.npalh) {
        g->stats.npalh(k) = long((g->stats.npal(k) * 1.0 / (runningTime * 1.0 / 3600)) * 100) * 1.0 / 100;
    }

    statsDisplay(getStatsRows);
}


/*
    Mise � jour des stats sur le nombre de missions en attente (� appeler � la cr�ation et � la prise en charge de la mission)
    @param flux [char] -> flow name
    @param mode [long] -> 1 pour la creation d'une mission, -1 pour la prise en charge d'une mission
    @param t [long] -> time when action has been realized (seconds)
*/
export function statsNattUpdate(flux,mode,t){
    // flux : nom du flux
    // mode : 1 pour la creation d'une mission, -1 pour la prise en charge d'une mission
    // t : temps auquel l'action a ete realisee
    global g('simConf');

    nflux = find(g->stats.flux == flux);

    //Mise a jour de tatt
    g->stats.tatt(nflux) = t;
}


/*
    Retourne les lignes de stats � afficher
    @param s0 [L.char] -> stats to display
*/
export function s0 = getStatsRows(){
    global g('simConf');

    //Affichage des stats par mission
    i = for(g->stats){
        s0.{end+1} = {
            g->stats.flux(i),
            sprintHMS(g->stats.tassign_moy(i)), sprintHMS(g->stats.tassign_max(i)),
            sprintHMS(g->stats.tmiss_moy(i)), sprintHMS(g->stats.tmiss_max(i)),
            g->stats.nmiss(i),g->stats.npal(i),g->stats.npalh(i)
        };
    }

    //Affichage des stats globales
    assigmoy = (sum(g->stats.nmiss)) ? sprintHMS(sum(g->stats.tassign_moy*g->stats.nmiss) / sum(g->stats.nmiss)) : sprintHMS(0);
    missmoy  = (sum(g->stats.nmiss)) ? sprintHMS(sum(g->stats.tmiss_moy*g->stats.nmiss)   / sum(g->stats.nmiss)) : sprintHMS(0);

    // Affichage des totaux
    s0.{end+1} = {
        'TOTAL',
        assigmoy, sprintHMS(max(g->stats.tassign_max)),
        missmoy, sprintHMS(max(g->stats.tmiss_max)),
        sum(g->stats.nmiss),sum(g->stats.npal),sum(g->stats.npalh),
    };
}


/*
    Returns the duration of a pick / drop (ALPHA) //TODO : rework
    @param truck [char] -> a truck type
    @param grpName [char] -> name of the group of points
    @return pickTime [float] -> duration of the pick
    @return dropTime [float] -> duration of the drop
*/
export function [pickTime, dropTime] = getPickDropTimes(truck, grpName) {
    /*  pntName = getGroupPnt(grpName).{0};
    pointData = missPntGetInfo(pntName);

    miss = varspc();

    miss->truckType = truck ;
    miss->pointData = pointData ;
    miss->missionType = pointData->type ;

    pickTime = 0. ;
    dropTime = 0. ;

    tasks = {'pick','drop'};
    steps = {'approach','destination'};
    t = for(tasks) {
        duration = 0. ;
        miss->task = tasks.{t};
        s = for(steps) {
            step = steps.{s};
            duration = duration + getFrkMvt(miss, step) ;
        }

        // rpkDestination
        pntId = cirFindPntName(pntName);
        pntPos = cirGetPntp(pntId);
        rpkDestination = pntPos.sa;
        // forkOffsetEmpty
        //        idrob = robGetId();
        //        serial = number2Serial(idrob);
        //        DIST_ROLLER_BACKREST =  getPreferenceValue("general/distRollerBackrest",serial);
        //        DIST_ROLLERS_ENDFRK =  getPreferenceValue("general/distanceRollerEndFork",serial);
        //        DIST_FORKS = DIST_ROLLER_BACKREST + DIST_ROLLERS_ENDFRK;
        DIST_FORKS = 1.2 ; // TODO
        forkOffsetEmpty = DIST_FORKS + pointData->safetyPointMargin;
        // forkOffsetCarry
        //        dimPal = palDefGetDim(pallet);
        //        palLenght = dimPal.re;
        palLenght = 1.2 ; // TODO
        forkOffsetCarry = max(DIST_FORKS, palLenght) + pointData->safetyPointMargin;
        // speed
        //        speed = getPreferenceValue("sim/simVmaxAR",serial) ;
        speed = 0.25 ; // TODO

        if(!strcmp(miss->task,'pick')) {
            pickTime = long(floor(duration + forkOffsetEmpty / speed));
        }
        else if(!strcmp(miss->task,'drop')) {
            dropTime = long(ceil(duration + forkOffsetCarry / speed));
        }
    }
 */
}


/*
    Function to write JSON results files from results DB
    @param file [string] -> name (path) of the results file
*/
export function writeJsonResults(file) {
    global g('simConf');

    // return if JSON outputs disabled 
    if(0 == g->enableJsonOutputs) return;

    // create content and fill time
    content = ['{\n\t"exportDate":"', sprintDate('%Y%m%d', time() ), '",'];
    content = [content, '\n\t"exportTime":"', sprintDate('%H%M%S', time() ), '",'];

    // fill json content
    content = addJsonContentInitialData(content);
    content = addJsonContentResultData(content);
    content = addJsonContentComputedData(content);

    // close json content
    content = [content, '\n}'] ;

    // Select the file extension
    extension = '.json';
    file = strReplace(file,'.txt',extension);

    saveText(file,content);

    logDebug("[writeJsonResults] Json file created.");
}

/*
  Function that add tasks field to json content
  - project info
  - simulation config
  - robots list
  - missions list
  @param content_in [string] -> json content
  @return content_out [string] -> content filled
*/
export function content_out = addJsonContentInitialData(content_in) {
  id = getSimxGlobalId();
  global w(id);
  global g('simConf');

  // * * * * * * * Title * * * * * * * * * * *
  content = [content_in, '\n\t"initial data":{'];

  // * * * * * Project info * * * * * * * * * *
  content = [content, '\n\t\t"project": "', getPreferenceValue('general/projectName'), '",'];
  content = [content, '\n\t\t"circuit": "', strReplace(fnameConc(w->env->proj,[w->env->open]),"\\","//"),'",'];
  content = [content, '\n\t\t"sdk": "', scmVersionInfo(),'",'];
  content = [content, '\n\t\t"sim": "', simToolGetVersion(),'",'];

  // * * * Simulation configuration * * * * * *
  content = [content, '\n\t\t"config": {'];
  content = [content, '\n\t\t\t"scheduler": "', char(g->scheduler),'",'];
  content = [content, '\n\t\t\t"battery": {'];
  if(1 == g->enableBattMiss) {
    content = [content, '\n\t\t\t\t"strategy": "', char(g->battPntStrategy),'",'];
    if(1 == g->enableOPC) content = [content, '\n\t\t\t\t"OPC": true'];
    else content = [content, '\n\t\t\t\t"OPC": false'];
  }
  else {
    content = [content, '\n\t\t\t\t"enable": false'];
  }
  content = [content, '\n\t\t\t},'];
  content = [content, '\n\t\t\t"runtime": {'];
  content = [content, '\n\t\t\t\t"start": ',   num2str(g->startCollectData),','];
  content = [content, '\n\t\t\t\t"runtime": ', num2str(g->simulationRunTime)];
  content = [content, '\n\t\t\t}'];
  content = [content, '\n\t\t},'];

  // * * * * * * * Robot list * * * * * * * *
  content = [content, '\n\t\t"robots": ['];
  k = for(1, g->nRob) {
      //get globals
      global r ('simRob');
      global b ('simBatt');
      //write info of each rob
      elt = (k == 1) ? '\n\t\t\t{' : ',\n\t\t\t{' ;
      elt = [elt,   '"id": ', num2str(k)];
      elt = [elt, ', "truck": "',  char(r->serial.{k-1}), '"'];
      elt = [elt, ', "model": "',  char(r->model.{k-1}), '"'];
      elt = [elt, ', "nav": {'];
      elt = [elt,     '"vmax": {','"forward": ', num2str(r->vmaxforward.(k-1)), ', "backward": ', num2str(r->vmaxbackward.(k-1)), ', "lateral": ', num2str(r->vmaxlateral.(k-1)),'}'];
      elt = [elt,   ', "accel": {','"carry": ',  num2str(r->accelcarry.(k-1)), ', "empty": ',     num2str(r->accelempty.(k-1)),'}'];
      elt = [elt,   ', "decel": {','"carry": ',  num2str(r->decelcarry.(k-1)), ', "empty": ',     num2str(r->decelempty.(k-1)),'}'];
      elt = [elt,   '}'];
      elt = [elt, ', "battery": {'];
      if(1 == g->enableBattMiss) {
          elt = [elt,   '"type": "', char(b->battType.{k-1}), '"'];
          elt = [elt, ', "data": {','"power": ', num2str(b->P.(k-1)), ', "voltage": ', num2str(b->U.(k-1)), ', "charge": ', num2str(b->C.(k-1)), ', "use": ', num2str(b->use.(k-1)), '}'];
      }
      else elt = [elt, '"enable": 0'];
      elt = [elt,   '}'];
      elt = [elt, '}'];
      content = [content, elt]; // Add it to the global content
  }
  content = [content, '\n\t\t],'];   // Close the content (robot list)

  //* * * * * * Mission list * * * * * * * *
  content = [content, '\n\t\t"missions": ['];
  unint missions = g->missions;
  k = for(missions) {
    elt = (k == 0) ? '\n\t\t\t{' : ',\n\t\t\t{' ;
    elt = [elt,   '"id": ',     num2str(k)];
    elt = [elt, ', "name": "',  char(missions.{k}->title), '"'];
    if(isempty(missions.{k}->group) || isnull(missions.{k}->group)) {
      elt = [elt, ', "group": ', tab2str(robFindAll)];
    }
    else {
      elt = [elt, ', "group": ',  tab2str(missions.{k}->group)];
    }
    elt = [elt, ', "throughput": ',  num2str(missions.{k}->ratePerHour)];
    elt = [elt, ', "steps": ['];
    i = for(missions.{k}->objectives) {
      elt = (i == 0)? [elt,'{'] : [elt,', {'];
      elt = [elt,   '"task": "',  char(missions.{k}->objectives.{i}->task), '"'];
      if(isvar(missions.{k}->objectives.{i},"grp")) {
        if(istype(missions.{k}->objectives.{i}->grp, 'L.char')) {
            elt = [elt, ', "dst": ["',strJoin(missions.{k}->objectives.{i}->grp,'","'),'"]'];
        }
        else {
            elt = [elt, ', "dst": "',  char(missions.{k}->objectives.{i}->grp), '"'];
        }
      }
      else {
        elt = [elt, ', "dst": "',  char(missions.{k}->objectives.{i}->dst), '"'];
      }
      if(isvar(missions.{k}->objectives.{i},"slot"))  elt = [elt, ', "slot": ',   num2str(missions.{k}->objectives.{i}->slot)];
      if(isvar(missions.{k}->objectives.{i},"via"))   elt = [elt, ', "via": "',   char(missions.{k}->objectives.{i}->via),'"'];
      if(isvar(missions.{k}->objectives.{i},"nbPal")) elt = [elt, ', "nbPal": ',  num2str(missions.{k}->objectives.{i}->nbPal)];
      if(isvar(missions.{k}->objectives.{i},"strategy"))  elt = [elt, ', "strategy": "',  char(missions.{k}->objectives.{i}->strategy), '"'];
      if(isvar(missions.{k}->objectives.{i},"approachTime"))  elt = [elt, ', "approachTime": ',  num2str(missions.{k}->objectives.{i}->approachTime)];
      if(isvar(missions.{k}->objectives.{i},"detectionTime"))  elt = [elt, ', "detectionTime": ',  num2str(missions.{k}->objectives.{i}->detectionTime)];
      if(isvar(missions.{k}->objectives.{i},"destinationTime"))  elt = [elt, ', "destinationTime": ',  num2str(missions.{k}->objectives.{i}->destinationTime)];
      elt = [elt, '}'];
    }
    elt = [elt, ']'];
    elt = [elt, ', "flows": ['];
    j = for(missions.{k}->flows) {
      elt = (j == 0)? [elt,'{'] : [elt,', {'];
      elt = [elt,   '"name": "',   char(missions.{k}->flows.{j}->flowName), '"'];
      elt = [elt, ', "steps": ',   num2str(missions.{k}->flows.{j}->numberOfStep)];
      elt = [elt, '}'];
    }
    elt = [elt, ']'];
    elt = [elt, '}'];

    content = [content, elt]; // add it to the global content
  }

  content = [content, '\n\t\t]'];   // Close the content (mission list)
  content_out = [content, '\n\t},'];   // Close the content (initial data)
}


/*
    Function that add tasks field to json content
    - tasks (from schd)
    - flows (from results)
    - robot info (from replay if enabled)
    @param content_in [string] -> content of the json file
    @return content_out [string] -> new content
*/
export function content_out = addJsonContentResultData(content_in) {
    // * * * * * * * Title * * * * * * * * * * *
    content = [content_in, '\n\t"result data":{'];

    // * * * * * * * scheduler * * * * * * * * *
    schdData = DBget_gbl('simSchdDB'); // get results data

    // check that result data validity
    if(isempty(schdData)) displayError("[writeJsonResults] Schd DB empty !");
    else if(!isvar(schdData, 'id')) displayError("[writeJsonResults] Field 'id' does not exist in schd DB !");
    else if(isempty(schdData->id)) displayError("[writeJsonResults] Field 'id' is empty in schd DB !");

    // fill the flows from results DB
    content = [content, '\n\t\t"tasks": ['] ;
    k = for(schdData->id) {
        // Write the content of each key
        elt = (k == 0)? '\n\t\t\t{' : ',\n\t\t\t{' ;
        elt = [elt,   '"id": ',           num2str(schdData->id.(k))];
        elt = [elt, ', "taskName": "',    char(schdData->taskName.(k)) ,'"'];
        elt = [elt, ', "missId": ',       num2str(schdData->missId.(k))];
        elt = [elt, ', "pgm": "',         char(schdData->pgm.(k)),'"'];
        elt = [elt, ', "addDate": ',      num2str(schdData->addDate.(k))];
        elt = [elt, ', "startDate": ',    num2str(schdData->startDate.(k))];
        elt = [elt, ', "endDate": ',      num2str(schdData->endDate.(k))];
        elt = [elt, ', "state": ',        num2str(schdData->state.(k))];
        elt = [elt, ', "robAuth": ',      tab2str(schdData->robAuth.(k).{0})];
        elt = [elt, ', "rob": ',          num2str(schdData->rob.(k))];
        elt = [elt, ', "priority": ',     num2str(schdData->priority.(k))];
        elt = [elt, '}'] ;

        // Add it to the global content
        content = [content, elt];
    }
    content = [content, '\n\t\t],'];  // Close the content (tasks)

    //* * * * * * flows * * * * * * * * * *
    resData = DBget_gbl('simResultsDB');  // get results data

    // check that result data validity
    if(isempty(resData)) displayError("[writeJsonResults] Results DB empty !");
    else if(!isvar(resData, 'id')) displayError("[writeJsonResults] Field 'id' does not exist in results DB !");
    else if(isempty(resData->id)) displayError("[writeJsonResults] Field 'id' is empty in results DB !");

    // fill the flows from results DB
    content = [content, '\n\t\t"flows": ['] ;
    k = for(resData->id) {
        // Write the content of each key
        elt = (k == 0)? '\n\t\t\t{' : ',\n\t\t\t{' ;
        elt = [elt,   '"id": ',           num2str(resData->id.(k))];
        elt = [elt, ', "schdId": ',       num2str(resData->schdId.(k))];
        elt = [elt, ', "flowName": "',    char(resData->flowName.(k)) ,'"'];
        elt = [elt, ', "taskName": "',    char(resData->taskName.(k)) ,'"'];
        if(1 == resData->flowDisplay.(k)) {
          elt = [elt, ', "flowDisplay": true'];
        }
        else {
          elt = [elt, ', "flowDisplay": false'];
        }
        elt = [elt, ', "addDate": ',      num2str(resData->addDate.(k))];
        elt = [elt, ', "startDate": ',    num2str(resData->startDate.(k))];
        elt = [elt, ', "endDate": ',      num2str(resData->endDate.(k))];
        elt = [elt, ', "npal": ',         num2str(resData->npal.(k))];
        elt = [elt, ', "rob": ',          num2str(resData->rob.(k))];
        elt = [elt, '}'] ;

      // Add it to the global content
      content = [content, elt] ;
    }
    content = [content, '\n\t\t],']; // Close the content (flows)

    // * * * * * * replay * * * * * *
    global g('simConf');

    if(0 == g->enableReplayDB) { // return if log replay not enabled
      content = [content, '\n\t\t"replay": {"enable":false}'];
      content_out = [content, '\n\t\t},'];  // Close the content (result data)
      return;
    }

    repData = DBget_gbl('simReplayDB');  // get results data

    // check that result data validity
    if(isempty(repData)) displayError("[writeJsonResults] Replay DB empty !");
    else if(!isvar(repData, 'id')) displayError("[writeJsonResults] Field 'id' does not exist in replay DB !");
    else if(isempty(repData->id)) displayError("[writeJsonResults] Field 'id' is empty in replay DB !");

    // fill the flows from results DB
    content = [content, '\n\t\t"replay": ['] ;
    k = for(repData->id) {
        // Write the content of each key
        elt = (k == 0)? '\n\t\t\t{' : ',\n\t\t\t{';
        elt = [elt,   '"date": "',         char(repData->date.(k)) ,'"'];
        elt = [elt, ', "time": "',         char(repData->time.(k)) ,'"'];
        elt = [elt, ', "idrob": ',         num2str(repData->idrob.(k))];
        elt = [elt, ', "xpos": ',          sprintf("%.3f",repData->xpos.(k))];
        elt = [elt, ', "ypos": ',          sprintf("%.3f",repData->ypos.(k))];
        elt = [elt, ', "cap": ',           sprintf("%.3f",repData->cap.(k))];
        elt = [elt, ', "state": "',        char(repData->state.(k)) ,'"'];
        elt = [elt, ', "batt": ',          sprintf("%.3f",repData->batt.(k))];
        elt = [elt, ', "taskId": ',        num2str(repData->taskId.(k))];
        elt = [elt, ', "taskName": "',     char(repData->taskName.(k)) ,'"'];
        elt = [elt, ', "step": ',          num2str(repData->taskStep.(k))];
        elt = [elt, ', "dst": "',          char(repData->dst.(k)) ,'"'];
        elt = [elt, ', "lockedByIdrob": ', num2str(repData->lockedByIdrob.(k))];
        elt = [elt, ', "loop": ',          num2str(repData->loop.(k))];
        elt = [elt, ', "idrobStp": ',      num2str(repData->idrobStp.(k))];
        elt = [elt, ', "speed": ',         sprintf("%.3f",repData->speed.(k))];
        elt = [elt, ', "accel": ',         sprintf("%.3f",repData->accel.(k))];
        elt = [elt, ', "frk": ',           sprintf("%.3f",repData->frk.(k))];
        elt = [elt, ', "LI": ',            sprintf("%.3f",repData->LI.(k))];
        elt = [elt, '}'] ;

      // Add it to the global content
      content = [content, elt] ;
    }
    content = [content, '\n\t\t]']; // Close the content (replay)

    // Close the content (result data)
    content_out = [content, '\n\t\t},'];
}


/*
    Function that add tasks field to json content
    - flows (from resultsDB and schdDB)
    - robots (from counters)
    @param content_in [string] -> content of the json file
    @return content_out [string] -> new content
*/
export function content_out = addJsonContentComputedData(content_in) {
    global r('simRob');
    global g('simConf');
    global b('simBatt');

    // * * * * * * * Title * * * * * * * * * * *
    content = [content_in, '\n\t"computed data":{'];

    //* * * * * * flows * * * * * * * * * *
    resData = DBget_gbl('simResultsDB');  // get results data

    // check result data validity
    if(isempty(resData)) displayError("[writeJsonResults] Results DB empty !");
    else if(!isvar(resData, 'id')) displayError("[writeJsonResults] Field 'id' does not exist in results DB !");
    else if(isempty(resData->id)) displayError("[writeJsonResults] Field 'id' is empty in results DB !");

    // fill the flows computed results
    content = [content, '\n\t\t"flows": ['] ;
    k = for(resData->id) {
        // Write the content of each key
        flowName = resData->flowName.(k);
        elt = (k == 0)? '\n\t\t\t{' : ',\n\t\t\t{' ;
        elt = [elt,   '"id": ',           num2str(resData->id.(k))];
        elt = [elt, ', "flowName": "',    char(flowName) ,'"'];
        elt = [elt, ', "throughput": ',   num2str(resComputeRatePerHour(flowName))];
        elt = [elt, ', "npal": ',         num2str(resComputePalMoved(flowName))];
        // assign times
        [assMin, assAvg, assMax] = resComputeFlowAssignTime(flowName);
        elt = [elt, ', "assign time": {'];
        elt = [elt,     '"min": ', num2str(assMin)];
        elt = [elt,   ', "avg": ', num2str(assAvg)];
        elt = [elt,   ', "max": ', num2str(assMax)];
        elt = [elt,   '}'];
        // flow duration
        [durMin, durAvg, durMax] = resComputeFlowDuration(flowName);
        elt = [elt, ', "duration": {'];
        elt = [elt,     '"min": ', num2str(durMin)];
        elt = [elt,   ', "avg": ', num2str(durAvg)];
        elt = [elt,   ', "max": ', num2str(durMax)];
        elt = [elt,   '}'];
        elt = [elt, '}'];

      // Add it to the global content
      content = [content, elt];
    }
    content = [content, '\n\t\t],']; // Close the content (flows)

    //* * * * * * robots * * * * * * * * * *
    content = [content, '\n\t\t"robots": ['] ;

    k = for(r->id) {
        // Write the content of each key
        elt = (k == 0)? '\n\t\t\t{' : ',\n\t\t\t{';
        idrob = r->id.(k);
        elt = [elt,   '"id": ', num2str(idrob)];
        // computed times of each robot
        [timeEmpty, timeCarry, timeTaxiMove, timeTaxiStop, timeBattMove, timeBattStop, timeTrafficStop] = resComputeRobotTimes(idrob);
        elt = [elt, ', "times": {'];
        elt = [elt,     '"running": {'];
        elt = [elt,       '"empty": ', num2str(timeEmpty)];
        elt = [elt,     ', "carry": ', num2str(timeCarry)];
        elt = [elt,     '}'];
        elt = [elt,   ', "taxi": {'];
        elt = [elt,       '"move": ', num2str(timeTaxiMove)];
        elt = [elt,     ', "stop": ', num2str(timeTaxiStop)];
        elt = [elt,     '}'];
        elt = [elt,   ', "batt": {'];
        elt = [elt,       '"move": ', num2str(timeBattMove)];
        elt = [elt,     ', "stop": ', num2str(timeBattStop)];
        elt = [elt,     '}'];
        elt = [elt,   ', "traffic": ', num2str(timeTrafficStop)];
        elt = [elt,   '}'];
        // number of missions done
        [nbMissDone, missName] = resComputeRobotMissDone(idrob);
        elt = [elt, ', "missions": ['];
        i = for(nbMissDone) {
          elt2 = (i == 0)? '{' : ', {';
          elt2 = [elt2,   '"name": "',  char(missName.{i}), '"'];
          elt2 = [elt2, ', "nbmiss": ', num2str(nbMissDone.(i))];
          elt2 = [elt2, '}'];
          elt  = [elt, elt2];
        }
        elt = [elt, ']'];
        // number of battery mission done
        nbbatt = (1 == g->enableBattMiss)? b->numBattMissDone.(k) : 0;
        elt = [elt, ', "nbbatt": ', num2str(nbbatt)];
        // speeds during the simulation
        [minSpeed, avgBackSpeed, avgForSpeed, avgSpeed, maxSpeed] = resComputeRobotSpeed(idrob);
        elt = [elt, ', "speed": {'];
        elt = [elt,     '"min": ', num2str(minSpeed)];
        elt = [elt, ',   "average": {'];
        elt = [elt,       '"backward": ', num2str(avgBackSpeed)];
        elt = [elt,     ', "forward": ',  num2str(avgForSpeed)];
        elt = [elt,     ', "global": ',   num2str(avgSpeed)];
        elt = [elt,     '}'];
        elt = [elt,   ', "max": ', num2str(maxSpeed)];
        elt = [elt,   '}'];

        elt = [elt, '}'];
        // Add it to the global content
        content = [content, elt] ;
    }
    content = [content, '\n\t\t]']; // Close the content (robots)

    // Close the content (computed data)
    content_out = [content, '\n\t\t}'];
}



/*
    Function to write CSV results files from results DB
    @param file [string] -> name (path) of the results file
*/
export function writeCsvResults(file) { //TODO
    global g('simConf');

    // get results data
    resData = DBget_gbl('simResultsDB');

    // check that data validity
    if(isempty(resData)) displayError("[writeCsvResults] Results DB empty !");
    else if(!isvar(resData, 'id')) displayError("[writeCsvResults] Field 'id' does not exist in results DB !");
    else if(isempty(resData->id)) displayError("[writeCsvResults] Field 'id' is empty in results DB !");

    //csv column separator
    switch(g->format) {
        case('US');
            colSep = ',';
        case('EU');
            colSep = ';';
    }

    header = ["id",colSep,"flowName",colSep,"flowDisplay",colSep,"addDate",colSep,"startDate",colSep,"endDate",colSep,"npal",colSep,"rob"];

    // Start filling the content
    content = [header];

    k = for(1, size(resData->id)) {
      // Write the content of each id
      csvId         =   num2str(resData->id.(k-1));
      csvSchdId     =   num2str(resData->schdId.(k-1));
      csvFlowName   =   char(resData->flowName.(k-1));
      csvTaskName   =   char(resData->flowName.(k-1));
      csvDisplay    =   num2str(resData->flowDisplay.(k-1));
      csvAddDate    =   num2str(resData->addDate.(k-1));
      csvStartDate  =   num2str(resData->startDate.(k-1));
      csvEndDate    =   num2str(resData->endDate.(k-1));
      csvNbpal      =   num2str(resData->npal.(k-1));
      csvRob        =   num2str(resData->rob.(k-1));
      //build line
      csvLine = strJoin(strn(csvId,csvSchdId,csvFlowName,csvTaskName,csvDisplay,csvAddDate,csvStartDate,csvEndDate,csvNbpal,csvRob),colSep);
      // Add it to the content
      content = [content, '\n', csvLine] ;
    }

    // Select the file extension
    extension = '.csv';
    file = strReplace(file,'.txt',extension);

    saveText(file,content);

    logDebug("[writeCsvResults] Csv file created.");
}

/*
    Function to write CSV schd files from schd DB
    @param file [string] -> name (path) of the results file
*/
export function writeCsvSchdData(file) { //TODO
    global g('simConf');

    // get results data
    schdData = DBget_gbl('simSchdDB');

    // check that data validity
    if(isempty(schdData)) displayError("[writeCsvSchdData] Results DB empty !");
    else if(!isvar(schdData, 'id')) displayError("[writeCsvSchdData] Field 'id' does not exist in results DB !");
    else if(isempty(schdData->id)) displayError("[writeCsvSchdData] Field 'id' is empty in results DB !");

    //csv column separator
    switch(g->format) {
        case('US');
            colSep = ',';
        case('EU');
            colSep = ';';
    }

    header = ["id",colSep,"taskName",colSep,"missId",colSep,"pgm",colSep,"addDate",colSep,"startDate",colSep,"endDate",colSep,"state",colSep,"robAuth",colSep,"rob",colSep,"priority"];

    // Start filling the content
    content = [header];

    k = for(1, size(schdData->id)) {
      // Write the content of each id
      csvId         =   num2str(schdData->id.(k-1));
      csvTaskName   =   char(schdData->taskName.(k-1));
      csvMissId     =   num2str(schdData->missId.(k-1));
      csvPgm        =   char(schdData->pgm.(k-1));
      csvAddDate    =   num2str(schdData->addDate.(k-1));
      csvStartDate  =   num2str(schdData->startDate.(k-1));
      csvEndDate    =   num2str(schdData->endDate.(k-1));
      csvState      =   num2str(schdData->state.(k-1));
      csvRobAuth    =   tab2str(schdData->robAuth.(k-1).{0});
      csvRob        =   num2str(schdData->rob.(k-1));
      csvPriority   =   num2str(schdData->priority.(k-1));
      //build line
      csvLine = strJoin(strn(csvId,csvTaskName,csvMissId,csvPgm,csvAddDate,csvStartDate,csvEndDate,csvState,csvRobAuth,csvRob,csvPriority),colSep);
      // Add it to the content
      content = [content, '\n', csvLine] ;
    }

    // Select the file extension
    extension = '.csv';
    file = strReplace(file,'.txt',extension);

    saveText(file,content);

    logDebug("[writeCsvSchdData] Csv file created.");
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                        COMPUTE RESULTS FUNCTIONS                            //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Function that return the number of pallets moved for a flow
    @param flowName [char] -> name of the flow
    @return res [double] -> number of pallets moved
*/
export function res = resComputePalMoved(flowName) {

    // find flow ids from results DB
    flowIds = DBfind_gbl('simResultsDB','flowName','==',flowName);

    // return 0 if no results in DB
    if(isempty(flowIds)) {
      res = 0;
      logWarn("[resComputePalMoved] %s : %i (no entry in DB).",flowName,res);
      return;
    }

    // get the number of pallet moved
    flowPals = DBget_gbl('simResultsDB', flowIds, 'npal');

    // compute number of pallet moved
    res = sum(flowPals);

    //logInfo("[resComputePalMoved] %s : %i pal moved.",flowName,res);
}


/*
    Function that compute the result throughput of a flow
    @param flowName [char] -> name of the flow
    @return res [double] -> throughput (pal/h)
*/
export function res = resComputeRatePerHour(flowName) {

    // get the number of pallets moved
    totalPals = resComputePalMoved(flowName);

    // compute rate per hour from simulation time
    simDuration = simGetDuration(); // in seconds
    res = (totalPals * 1.0) * 3600 / simDuration; // in pal/hours

    //logInfo("[resComputeRatePerHour] %s : %.3f pal/h.",flowName,res);
}


/*
    Function that compute the assignement times of a flow
    @param flowName [char]  -> name of the flow
    @return resMin [long]   -> min assign time (seconds)
    @return resAvg [double] -> average assign time (seconds)
    @return resMax [long]   -> max assign time (seconds)
*/
export function [resMin, resAvg, resMax] = resComputeFlowAssignTime(flowName) {

  // find flow ids from results DB
  flowIds = DBfind_gbl('simResultsDB','flowName','==',flowName);

  // return 0 if no results in DB
  if(isempty(flowIds)) {
    resMin = 0; resAvg = 0; resMax = 0;
    logWarn("[resComputeFlowAssignTime] %s : min = %is, avg = %.2fs and max = %is (no entry in DB).",flowName,resMin,resAvg,resMax);
    return;
  }

  // get the addDate and startDate of the flow
  flowData = DBget_gbl('simResultsDB', flowIds, strn('id','addDate','startDate'));

  // create the variable to store all assign time of each flow iteration
  flowAssignTimes = new('long',0);

  // loop over each flow to fill the flowAssignTimes
  i = for(flowData->id) {
      flowAssignTimes.(end+1) = max(0,(flowData->startDate.(i) - flowData->addDate.(i))); //strictly positive
  }

  // compute results
  resMin = min(flowAssignTimes);
  resAvg = long(mean(flowAssignTimes));
  resMax = max(flowAssignTimes);

  //logInfo("[resComputeFlowAssignTime] %s : min = %is, avg = %is and max = %is",flowName,resMin,resAvg,resMax);
}


/*
    Function that compute the duration of a flow
    @param flowName [char] -> name of the flow
    @return resMin [long] -> min duration (seconds)
    @return resAvg [long] -> average duration (seconds)
    @return resMax [long] -> max duration (seconds)
*/
export function [resMin, resAvg, resMax] = resComputeFlowDuration(flowName) {

    // find flow ids from results DB
    flowIds = DBfind_gbl('simResultsDB','flowName','==',flowName);

    // return 0 if no results in DB
    if(isempty(flowIds)) {
      resMin = 0; resAvg = 0; resMax = 0;
      logWarn("[resComputeFlowDuration] %s : min = %is, avg = %.2fs and max = %is (no entry in DB).",flowName,resMin,resAvg,resMax);
      return;
    }

    // get the startDate and endDate of the flow
    flowData = DBget_gbl('simResultsDB', flowIds, strn('id','startDate','endDate'));

    // create the variable to store all assign time of each flow iteration
    flowDuration = new('long',0);

    // loop over each flow to fill the flowAssignTimes
    i = for(flowData->id) {
        flowDuration.(end+1) = max(0,(flowData->endDate.(i) - flowData->startDate.(i))); //strictly positive
    }

    // compute results
    resMin = min(flowDuration);
    resAvg = long(mean(flowDuration));
    resMax = max(flowDuration);

    //logInfo("[resComputeFlowDuration] %s : min = %is, avg = %is and max = %is",flowName,resMin,resAvg,resMax);
}


/*
    Function that compute the assignement time of a mission
    @param missName [char] -> name of the mission
    @return resMin [long] -> min assign time (seconds)
    @return resAvg [long] -> average assign time (seconds)
    @return resMax [long] -> max assign time (seconds)
*/
export function [resMin, resAvg, resMax] = resComputeMissAssignTime(missName) {

    // find miss ids from schd DB (take only done);
    missIds = setInter(DBfind_gbl('simSchdDB','taskName','==',missName),simSchdGetTask('done'));

    // return 0 if no results in DB
    if(isempty(missIds)) {
      resMin = 0; resAvg = 0; resMax = 0;
      logWarn("[resComputeMissAssignTime] %s : min = avg = max = %is (no entry in DB).",missName,resMin);
      return;
    }

    // get the startDate and endDate of the mission
    missData = DBget_gbl('simSchdDB', missIds, strn('id','addDate','startDate'));

    // create the variable to store all assign time of each flow iteration
    missAssignTime = new('long',0);

    // loop over each flow to fill the flowAssignTimes
    i = for(missData->id) {
        missAssignTime.(end+1) = max(0,(missData->startDate.(i) - missData->addDate.(i))); //strictly positive
    }

    // compute results
    resMin = min(missAssignTime);
    resAvg = long(mean(missAssignTime));
    resMax = max(missAssignTime);

    //logInfo("[resComputeMissAssignTime] %s : min = %is, avg = %is and max = %is",missName,resMin,resAvg,resMax);
}


/*
    Function that compute the duration of a mission
    @param missName [char] -> name of the mission
    @return resMin [long] -> min duration (seconds)
    @return resAvg [long] -> average duration (seconds)
    @return resMax [long] -> max duration (seconds)
*/
export function [resMin, resAvg, resMax] = resComputeMissDuration(missName) {

    // find miss ids from schd DB (take only done);
    missIds = setInter(DBfind_gbl('simSchdDB','taskName','==',missName),simSchdGetTask('done'));

    // return 0 if no results in DB
    if(isempty(missIds)) {
      resMin = 0; resAvg = 0; resMax = 0;
      logWarn("[resComputeMissDuration] %s : min = avg = max = %is (no entry in DB).",missName,resMin);
      return;
    }

    // get the startDate and endDate of the mission
    missData = DBget_gbl('simSchdDB', missIds, strn('id','startDate','endDate'));

    // create the variable to store all assign time of each flow iteration
    missDuration = new('long',0);

    // loop over each flow to fill the flowAssignTimes
    i = for(missData->id) {
        missDuration.(end+1) = max(0,(missData->endDate.(i) - missData->startDate.(i))); //strictly positive
    }

    // compute results
    resMin = min(missDuration);
    resAvg = long(mean(missDuration));
    resMax = max(missDuration);

    //logInfo("[resComputeMissDuration] %s : min = %is, avg = %is and max = %is",missName,resMin,resAvg,resMax);
}

/*
    Function that compute the times of a robot
    @param idrob [long]     -> id of the robot
    @return resEmpty [long]       -> time running empty (seconds)
    @return resCarry [long]       -> time running carry (seconds)
    @return resTaxiMove [long]    -> time moving to taxi (seconds)
    @return resTaxiStop [long]    -> time at taxi position (seconds)
    @return resBattMove [long]    -> time moving to battery (seconds)
    @return resBattStop [long]    -> time at batt position (seconds)
    @return resTrafficStop [long] -> time stopped in traffic (seconds)
*/
export function [resEmpty, resCarry, resTaxiMove, resTaxiStop, resBattMove, resBattStop, resTrafficStop] = resComputeRobotTimes(idrob) {
    global r('simRob');
    global g('simConf');

    //fill results from globals
    resEmpty = r->timeEmpty.(idrob-1);
    resCarry = r->timeCarry.(idrob-1);
    resTaxiMove = r->timeTaxiMove.(idrob-1);
    resTaxiStop = r->timeTaxiStop.(idrob-1);
    resBattMove = r->timeBattMove.(idrob-1);
    resBattStop = r->timeBattStop.(idrob-1);
    resTrafficStop = g->trafficTime.(idrob-1);

    //logInfo("[resComputeRobotTimes] Rob%i : empty = %is, carry = %is, taxiMove = %is, taxiStop = %is, battMove = %is, battStop = %is, traffic = %is",idrob,resEmpty,resCarry,resTaxiMove,resTaxiStop,resBattMove,resBattStop,resTrafficStop);
}


/*
    Function that compute the number of missions done
    @param idrob [long]  -> id of the robot
    @return resNb [L.long] -> Nb of missions done by the robot (index corresponding to missId)
    @return resName [char] -> name of the mission
*/
export function [resNb, resName] = resComputeRobotMissDone(idrob) {
    global g('simConf');

    // get the mission list
    unint missions = g->missions;

    // create res variable from mission size
    resNb = new('long',size(missions));
    resName = new('L.char',size(missions));

    // loop over all missions and fill res
    i = for(missions) {
      // find missions in DB from title, robot id
      title = missions.{i}->title;
      resName.{i} = title;
      missIds = setInter(DBfind_gbl('simSchdDB','taskName','==',title),DBfind_gbl('simSchdDB','rob','==',idrob)); // filter by robot

      // set 0 if no results in DB
      if(isempty(missIds)) {
        logWarn("[resComputeRobotMissDone] Rob%i : 0 missions %s (no entry in DB).",idrob,title);
        resNb.(i) = 0;
        continue;
      }

      // keep only missions done
      missDoneIds = setInter(missIds, simSchdGetTask('done'));

      // set 0 if no results in DB
      if(isempty(missDoneIds)) {
        logWarn("[resComputeRobotMissDone] Rob%i : 0 missions %s done  (no entry in DB).",title,idrob);
        resNb.(i) = 0;
        continue;
      }

      // fill the res variable
      resNb.(i) = size(missDoneIds); //number of missions done
    }

    //logInfo("[resComputeRobotMissDone] Rob%i : missions done = %z",idrob,resNb);
}


/*
    Function that return the speed of a robot
    @param idrob [long] -> if of the robot
    @return resMin [double] -> min speed (m/s)
    @return resAvgBack [double] -> average negative speed (m/s) (0.0 not included)
    @return resAvgFor [double] -> average positive speed (m/s) (0.0 not included)
    @return resAvg [double] -> average speed (m/s) (0.0 included)
    @return resMax [double] -> max speed (m/s)
*/
export function [resMin, resAvgBack, resAvgFor, resAvg, resMax] = resComputeRobotSpeed(idrob) {
    global g('simConf');

    // return 0 if log replay DBnot enabled
    if(0 == g->enableReplayDB) {
      resMin = 0; resAvgBack = 0; resAvgBack = 0; resAvgFor = 0; resAvg = 0; resMax = 0;
      logWarn("[resComputeRobotSpeed] Rob%i: speeds = 0 (replayDB not enabled).",idrob);
      return;
    }

    // get robot entries
    robotIds = DBfind_gbl('simReplayDB','idrob','==',idrob);

    // return 0 if DB empty
    if(isempty(robotIds)) {
      resMin = 0; resAvgBack = 0; resAvgBack = 0; resAvgFor = 0; resAvg = 0; resMax = 0;
      logWarn("[resComputeRobotSpeed] Rob%i: speeds = 0 (no entry in DB).",idrob);
      return;
    }

    // get speeds
    robData = DBget_gbl('simReplayDB',robotIds,strn('id','speed'));
    speed = robData->speed;
    negSpeed = speed.(find(speed<0.0));
    posSpeed = speed.(find(speed>0.0));

    // compute results
    resMin      = min(speed);
    resAvgBack  = mean(negSpeed);
    resAvgFor   = mean(posSpeed);
    resAvg      = mean(speed);
    resMax      = max(speed);

}
