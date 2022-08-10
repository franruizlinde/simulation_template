// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                            REPLAY INITIALIZATION                            //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/**
 * Function to start the robot manager log thread
 *
 * @public
 */
export function startRobotManagerLogs(robLs) {

    // get globals
    global g('simConf');
    global rp('simReplay');

    id = getSimxGlobalId();
    global w(id);

    // return if log replay disabled
    if(!g->enableLogReplay) return;

    // check that folder "results" exist, create it otherwise
    rp->fileDir = fnameConc(w->env->proj,'results');
    if(!isdir(rp->fileDir)) mkdir(rp->fileDir);

    // Set log file duration
    rp->threadTempo = 1; // in seconds

    // Choose which robot is recorded
    if(nargin <1) rp->robots = robFindAll(); // all robot are recorded
    else rp->robots = robLs;

    // Temp name (writing)
    rp->outputFileTempName = "TEMP_LogReplay_Writing.csv";
    rp->outputFileTempPath = fnameConc(rp->fileDir,rp->outputFileTempName);

    // Delete old temp file
    if(isfile(rp->outputFileTempPath)) rm(rp->outputFileTempPath);

    // variable for writing the csv file
    rp->counter = 1;
    rp->maxCounter = 10; // write every 10 simulated seconds //TOTEST

    // store replay data
    rp->content = new('char',0);
    //rp->log = new('char',0);

    // replay db name
    initReplayDB();

    // Launch main thread
    setRandCbk('$robotManagerLogsThread',sprintf('%is',rp->threadTempo),'0s');
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                            REPLAY DB DEFINITION                             //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Function that creates replay DB and delete all previous records if any
*/
export function initReplayDB() {
    global g('simConf');

    //return if json replay DB not enabled
    if(0 == g->enableReplayDB) {
      return;
    }
    logInfo('[REPLAY] Initializing custom replayDB');

    // get DB definition
    dbDef = simReplayDBDef();

    // Create DB from definition
    DBcreate_gbl('simReplayDB',dbDef,'id',0);

    // if keys, delete them all to re-start fresh
    keys = DBgetAllKeys_gbl('simReplayDB');
    if (size(keys)) {
        //logDebug('[RESULTS] Deleting old keys %z',keys);
        DBdel_gbl('simReplayDB',keys);
    }
}


/*
    simReplayDB definition
    @return def [varspc] -> simReplayDB definition varspc
*/
export function def = simReplayDBDef() {
    def = varspc;
    def->id            = 0;          // DB key, unique id
    def->date          = strn('');   // Date of the entry
    def->time          = strn('');   // Time of the entry
    def->idrob         = 0;          // Robot id
    def->xpos          = 0.0;        // x coordinates
    def->ypos          = 0.0;        // y coordinates
    def->cap           = 0.0;        // Cap angle
    def->state         = strn('');   // State of the robot
    def->batt          = 0.0;        // Battery level
    def->taskId        = 0;          // Task id
    def->taskName      = strn('');   // Task name
    def->taskStep      = 0;          // Task step
    def->dst           = strn('');   // Name of the destination
    def->lockedByIdrob = 0;          // Robot id that lock
    def->loop          = 0;          // Cantonloop (bool)
    def->idrobStp      = 0;          // Robot id that stop
    def->speed         = 0.0;        // Speed
    def->accel         = 0.0;        // Acceleration
    def->frk           = 0.0;        // Forks height
    def->LI            = 0.0;        // Initial lift height
}


/*
    Function to add an entry in the replay DB
    @param date          [char] -> Date of the entry
    @param t             [char] -> Time of the entry
    @param idrob         [long] -> Robot id
    @param xpos          [double] -> x coordinates
    @param ypos          [double] -> y coordinates
    @param cap           [double] -> Cap angle
    @param state         [char] -> State of the robot
    @param batt          [double] -> Battery level
    @param taskId        [long] -> Task id
    @param taskName      [char] -> Task name
    @param taskStep      [long] -> Task step
    @param dst           [char] -> Name of the destination
    @param lockedByIdrob [long] -> Robot id that lock
    @param loop          [long] -> Cantonloop (bool)
    @param idrobStp      [long] -> Robot id that stop
    @param speed         [double] -> Speed
    @param accel         [double] -> Acceleration
    @param frk           [double] -> Forks height
    @param LI            [double] -> Initial lift height
*/
export function simReplayAdd(date,t,idrob,xpos,ypos,cap,state,batt,taskId,taskName,taskStep,dst,lockedByIdrob,loop,idrobStp,speed,accel,frk,LI) {

    // Create DB entry
    v = varspc();
    v->date          = strn(date);     // Date of the entry
    v->time          = strn(t);        // Time of the entry
    v->idrob         = idrob;          // Robot id
    v->xpos          = xpos;           // x coordinates
    v->ypos          = ypos;           // y coordinates
    v->cap           = cap;            // Cap angle
    v->state         = strn(state);    // State of the robot
    v->batt          = batt;           // Battery level
    v->taskId        = taskId;         // Task id
    v->taskName      = strn(taskName); // Task name
    v->taskStep      = taskStep;       // Task step
    v->dst           = strn(dst);      // Name of the destination
    v->lockedByIdrob = lockedByIdrob;  // Robot id that lock
    v->loop          = loop;           // Cantonloop (bool)
    v->idrobStp      = idrobStp;       // Robot id that stop
    v->speed         = speed;          // Speed
    v->accel         = accel;          // Acceleration
    v->frk           = frk;            // Forks height
    v->LI            = LI;             // Initial lift height

    // Add entry to DB
    DBadd_gbl('simReplayDB',v);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                              REPLAY FUNCTIONS                               //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/**
* Function to end the robot manager log thread and rename the final file
*
* @public
*/
export function endRobotManagerLogs(outputFileFinalPath) {
    global rp('simReplay');
    global g('simConf');

    // return if log replay disabled
    if(!g->enableLogReplay) return;

    // get dir
    fileDir = rp->fileDir;
    outputFileTempPath = rp->outputFileTempPath;

    // check that folder "results" exist, create it otherwise
    if(!isdir(fileDir)) mkdir(fileDir);

    // Rename temp file
    if(isfile(outputFileTempPath)) {
        ok = renamefile(outputFileTempPath,[strReplace(outputFileFinalPath,'.txt','_replay.csv')]);

        // check that everything is ok
        if(ok) logInfo("[robotManagerLogs] File %s created.",outputFileFinalPath);
        else logWarn("[robotManagerLogs] Impossible to rename temp file into %s",outputFileFinalPath);

    }
    else logWarn("[robotManagerLogs] File not found.");

    // Create log file
    /*
    if(!isempty(rp->logs)) {
      outputLogFilePath = strReplace(outputFileFinalPath,'.txt','_replay.csv');
      writeFile(outputLogFilePath,rp->logs);
      logWarn("[robotManagerLogs] File %s created.",outputLogFilePath);
    }
    else logWarn("[robotManagerLogs] No logs recorded.");
    */
}

/**
 * Thread to collect robot manager logs
 *
 * @private
 */
export function robotManagerLogsThread()
{
    global rp('simReplay');
    global r('simRob');
    global g('simConf');
    global b('simBatt');

    // record only after startCollectData time reached
    if(simGetT < g->startCollectData) return;

    // Get SDK version
    sdkVersionAndTag 	= scmVersion();
    //sdkVersionTab 		= strSplit(sdkVersionAndTag,'.');
    //sdkVersion			= sdkVersionTab.{0};

    // Main loop
    // Init var
    coord   = new('cpx',0);
    cap     = new('double',0);
    speed   = new('double',0);
    accel   = new('double',0);
    forks   = new('double',0);
    LI      = new('double',0);
    battLvl = new('double',0);
    robotState = strn();
    taskId     = new('long',0);
    taskName   = strn();
    step       = new('long',0);
    dst        = strn();
    cantonVarspc = cantonDbg_GetState();
    cantonData   = varspc('occ', {}, 'rsv', {}, 'stock', {});

    // Loop around each robot
    k = for(rp->robots) {
        // Easier to understand
        idrob = rp->robots.(k);
        // Add all data
        coord.(end+1) = robGetZ(idrob);
        cap.(end+1) = robGetcap(idrob);
        // FIXME: Don't save canton dbg for now
        //[occ,rsv,stock] = cantonDbg_GetOcc(idrob);
        occ = [];
        rsv = [];
        stock = [];
        cantonData->occ.{end+1} = occ;
        cantonData->rsv.{end+1} = rsv;
        cantonData->stock.{end+1} = stock;
        // forks and initial lift
        forks.(end+1) = robGetForksHeight(idrob);
        LI.(end+1) = robGetInitialLift(idrob);
        // batt
        if(g->enableBattMiss) battLvl.(end+1) = b->battLevel.(idrob-1);
        else battLvl.(end+1) = 100;
        // state
        robotState.(end+1) = getRobotStateForReplay(idrob);
        // speed
        speed.(end+1) = robGetSpeed(idrob);
        // accel
        //accel.(end+1) = robGetAccel(idrob);
        accel.(end+1) = 0.0;
        // TO TEST: only retrieve value in memory to not be CPU intensive
        robTaskId = robFindTaskId(idrob);
        if (robTaskId < 0) {
            taskId.(end+1) = -1;
            taskName.(end+1) = !strcmp(robotState.(end), 'OFFLINE') ? 'OFF' : 'Taxi';
            step.(end+1) = 0;
            dst.(end+1) = strn(sprintf('taxi%i',idrob));
        }
        else {
            //robTaskData = tskGetData(robTaskId);
            taskId.(end+1) = robTaskId;
            taskName.(end+1) = robFindTask(idrob);
            robStep = tskGetStep(robTaskId);
            robDst = r->currentDst.{idrob-1};
            // step.(end+1) = stepToName(robStep);  //useless ?
            step.(end+1) = robStep;
            dst.(end+1) = strn(robDst);
        }
    }

    // Write it to the disk
    robotManagerWriteLogs(coord, cap, speed, accel, forks, LI, battLvl, robotState, taskId, taskName, step, dst, cantonVarspc, cantonData);
}

/**
 * Function to write robot manager logs into a csv file
 *
 * @param {L.long} idrobs Robot IDs
 * @param {L.cpx} coord List of robots coordinates
 * @param {L.double} cap List of robots cap
 * @param {L.double} speed List of robots speed
 * @param {L.double} forks List of robots forks height
 * @param {L.double} LI List of robots initial lift height
 * @param {L.double} battLvl List of robots battLvl
 * @param {name} robotState List of robots robotState
 * @param {L.long} taskId List of robots taskId
 * @param {name} taskName List of robots taskName
 * @param {name} step List of robots step
 * @param {name} dst List of robots dst
 * @param {varspc} cantonVarspc Canton varspc
 * @param {varspc} cantonData Canton list varspc
 *
 * @private
 */
export function robotManagerWriteLogs(coord, cap, speed, accel, forks, LI, battLvl, robotState, taskId, taskName, step, dst,cantonVarspc,cantonData)
{
    global rp('simReplay');
    global g('simConf');

    // Get file directory
    fileDir = rp->fileDir;
    outputFileTempPath = rp->outputFileTempPath;

    // Get the timestamp
    timestamp = time();
    timestampSim = simGetT();

    // Get DATE/TIME
    csvDate = sprintDate('%Y-%m-%d', timestamp);
    csvTime = sprintHMS(timestampSim);
    // Init content
    content = '';

    // Take format into account
    /*switch(g->format) {
        case('US');
            colSep = ',' ;
        case('EU');
            colSep = ';' ;
    }*/
    colSep = ',' ;

    // Loop over each robot
    j = for(rp->robots)
    {
        // Extract info
        idrob = rp->robots.(j);
        csvRob = num2str(idrob);
        csvXpos = num2str(coord.(j).re);
        csvYpos = num2str(coord.(j).im);
        csvCap = num2str(cap.(j));
        csvSpeed = num2str(speed.(j));
        csvAccel = num2str(accel.(j));
        csvFrk = num2str(forks.(j));
        csvLI = num2str(LI.(j));
        csvState = char(robotState.(j));
        csvBatt = num2str(battLvl.(j));
        csvTaskId = num2str(taskId.(j));
        csvTaskName = char(taskName.(j));
        csvStep = num2str(step.(j));
        csvDst = char(dst.(j));
        csv_lockedByIdrob = num2str(cantonVarspc->lockedByIdrob.(idrob-1));
        csv_loop = num2str(cantonVarspc->loop.(idrob-1));
        csv_idrobStp = num2str(cantonVarspc->idrobStp.(idrob-1));
        // Canton debug
        csv_cantonOcc = '';
        k = for(cantonData->occ.{j}) csv_cantonOcc = strJoin({csv_cantonOcc, num2str(cantonData->occ.{j}.(k))}, ' ');
        if (!size(csv_cantonOcc)) csv_cantonOcc = ' ';
        csv_cantonRsv = '';
        k = for(cantonData->rsv.{j}) csv_cantonRsv = strJoin({csv_cantonRsv, num2str(cantonData->rsv.{j}.(k))}, ' ');
        if (!size(csv_cantonRsv)) csv_cantonRsv = ' ';
        csv_cantonStock = '';
        k = for(cantonData->stock.{j}) csv_cantonStock = strJoin({csv_cantonStock, num2str(cantonData->stock.{j}.(k))}, ' ');
        if (!size(csv_cantonStock)) csv_cantonStock = ' ';
        // Build line
        csvLine = strJoin(strn(csvDate, csvTime, csvRob, csvXpos, csvYpos, csvCap, csvState, csvBatt, csvTaskId, csvTaskName, csvStep, csvDst, csv_lockedByIdrob, csv_loop, csv_idrobStp, csv_cantonOcc, csv_cantonRsv, csv_cantonStock, csvSpeed, csvFrk, csvLI),colSep);
        // Add line to DB
        if(1 == g->enableReplayDB) {
          simReplayAdd(csvDate, csvTime, idrob, coord.(j).re, coord.(j).im, cap.(j), char(robotState.(j)), battLvl.(j), taskId.(j), char(taskName.(j)), step.(j), char(dst.(j)), cantonVarspc->lockedByIdrob.(idrob-1), cantonVarspc->loop.(idrob-1), cantonVarspc->idrobStp.(idrob-1), speed.(j), accel.(j), forks.(j), LI.(j));
        }
        // Add it to content
        content = [content, '\n', csvLine];
    }

    // TO TEST: store everything in memory and write to file when output file change? every minute? every 10 sec? should be faster
    //
    if (rp->counter > rp->maxCounter)
    {
        // Write everything to the disk
        try
        {
            // Verify if the file already exists
            if (!isfile(outputFileTempPath))
            {
                header = ["date",colSep,"time",colSep,"ROB",colSep,"xpos",colSep,"ypos",colSep,"cap",colSep,"state",colSep,"batt",colSep,"taskID",colSep,"taskName",colSep,"step",colSep,"currentDst",colSep,"lockedByIdrob",colSep,"loop",colSep,"idrobStp",colSep,"cantonOcc",colSep,"cantonRsv",colSep,"cantonStock",colSep,"speed",colSep,"frk",colSep,"LI"];
                appendText(outputFileTempPath, header);
            }
            // Add the content
            appendText(outputFileTempPath, rp->content);
            // Reset the global variables
            rp->counter = 1;
            rp->content = content;
        }
        catch
        {
            logError('[robotManagerWriteLogs] %s', lastError());
            logError('[robotManagerWriteLogs] Adding content to global var instead');
            // Increment the glabal variable
            rp->counter = rp->counter + 1;
            rp->content = [rp->content, content];
        }
    }
    else
    {
        // Increment the glabal variable
        rp->counter = rp->counter + 1;
        rp->content = [rp->content, content];
    }
}


/**
 * Function to get the robot's stat *
 * @param {long} idrob idrob
 *
 * @return {string} State of the robot (RUNNING, OFFLINE, MISSION_ERROR, NAVIGATION_ERROR, BATTERY_ERROR, MANUAL, STOP_TRAFFIC, STOP_NAVIGATION, CURTAIN_LASER, HARDWARE_SAFETY, FRONT_SAFETY, REAR_SAFETY, GPIO_ERROR)
 *
 * @public
 */
export function robotState = getRobotStateForReplay(idrob)
{
    // Init robotState variable
    robotState = "N/A";
    // Get data from rob
    data = robGet(idrob);

    // mission state
    missState = data.state_event;
    // canton state
    navState = data.nav.infoCanton;

    if(4 == missState) robotState = "OFFLINE";
    else if (3 == missState) robotState = "MANUAL";
    else if (2 == navState) robotState = "STOP_TRAFFIC";
    else if (36 == navState) robotState = "STOP_NAVIGATION";
    //else if (0 == missState) robotState = "READY";
    else if (0 == missState || 1 == missState || 2 == missState) robotState = "RUNNING";
    else logWarn("[getRobotStateForReplay] Unknown state case for rob %i (missState = %z, navState = %z)",idrob,missState,navState);

}


/*
    Function that write logSim into text file .log
    @param str [string] -> message to write
*/
export function logSimWriteReplay(str) {
  global rp('simReplay');

  //append logs variable
  rp->logs = [rp->logs, str];
}
