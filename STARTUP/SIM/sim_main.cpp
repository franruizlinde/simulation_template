
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                                MAIN FUNCTION                                //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Main function launched at the beginning of the simulation
*/
export function sim_main {
    //call('clc'); // Clears the Kiwi console

    createcsvDB();

    initCSV(); //read the csv files

    initErrWindow(); // Initialises the error window

    initGlobalVar(); // Initialises the global variable 'sol' from configuration

    ratePerHourFromCSV();   // Gets the ratePerHour value for the missions from the csv file

    initMissEditor(); // Initialises the mission window

    initRobTaxi(); // Initialises the robots and taxi missions

    initRobBatt(); // Initialises the battery missions

    initForks(); // Initialise the forks functions

    flows = initSimFlows(); // Initialises the flows and the missions

    initStats(flows); // Initialises the statistics variables

    initPallets(); // Initialises the pallets

    initSimSchd(); // Initialises the simulation scheduler DB

    initSimResults(); // Initialises the database containing the results

    initCantonCheck(); // Initialise the canton fake thread

    initSimWindow(); // Initialises the simulation windows

    openMissEditor(); // Open the mission editor windows

    optiNavOn(); // Initialise the optimized navigation

    initLogs(); // Initialise the log functions
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                           INITIALIZATION FUNCTIONS                          //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Initialisation of simulation global from conf
*/
export function initGlobalVar() {
    global g('simConf');
    if(isvar(g, 'dlg')) closeStatsWin;
    clearGlobal('simConf'); // clear the sim config global content

    // initialise sol (see getConfig function)
    conf = getConfig;
    setGlobal('simConf',conf);

}


/*
    Initialisatio of the pallets
*/
export function initPallets() {
    global g('simConf');

    // Init pallets definition
    palDef(g->pallets);
}

/*
    Initialisation of the logs functions
*/
export function initLogs() {
    global g('simConf');

    // Select the console log level
    logConsoleLevel(g->logConsoleLevel);

    if(g->enableLogReplay) {
        logInfo('[initLogs] starting log replay recording');
        acall('startRobotManagerLogs');
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                           CONFIGURATION FUNCTIONS                           //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
  Import the coustom configuration from the .c file
  @return conf [varspc] -> configuration varspc
*/
export function conf = getConfig() {

    // Import custom configuration
    conf = acall('customConfig');

    // ** GENERAL CONFIGURATION **
    conf->startCollectData      = conf->startCollectData*60*60;  // convert value from hours to seconds
    conf->simulationRunTime     = conf->simulationRunTime*60*60; // convert value from hours to seconds
    conf->simulationStartTime   = -1; // start time of the simulation recording
    conf->simulationEndTime     = -1; // end time of the simulation recording
    conf->utilizationRatioTable = [1,0.95,0.90,0.85];

    // Distance from Taxi point where we don't take new missions
    conf->maxTaxiDistance = 2.0;

    // VNA forks rotation time
    conf->VNA_frkRotationTime = 15.; // 15 seconds

    // Loop will create an error if a canton loop is detected during the simulation
    conf->errorOnCantonLoop = 1;

    // Enable or disable log replay recording
    conf->enableLogReplay = 0;
    conf->enableReplayDB = 0; // cannot be enabled without "enableLogReplay"

    // Enable or disable JSON outputs
    conf->enableJsonOutputs = 0;

    // Enable or disable miss editor windows
    conf->missEditorEnable = 0;

    // Choose console log level
    conf->logConsoleLevel = 0; //0: DEBUG 1: INSTALL 2: INFO 3: WARN 4: ERROR 5: NONE

    // Enable or disable opportunity charging
    conf->enableOPC = 1;

    // Check that required fields in customConfig have been set properly
    verifyCustomConf(conf);
}


/*
    Check that required fields in customConfig have been set properly
    @param conf -> configuration varsp [varspc]
*/
export function verifyCustomConf(conf) {
    // Test that all required fields from customConfig() are defined or crash an error
    if (!isvar(conf, 'format'))
        displayError(sprintf('Field "format" is not configured in function customConfig()'));
    else if (!istype(conf->format,'char'))
        displayError(sprintf('Field "format" is not a string in function customConfig()'));
    else if (strcmp(conf->format,'EU') && strcmp(conf->format,'US'))
        displayError(sprintf('Field "format" is not "EU" or "US" in function customConfig()'));

    if (!isvar(conf, 'nRob'))
        displayError(sprintf('Field "nRob" is not configured in function customConfig()'));
    else if (!istype(conf->nRob,'long'))
        displayError(sprintf('Field "nrob" is not an integer in function customConfig()'));
    else if (conf->nRob > size(getPreferenceValue('general/trucksSerial')))
        displayError(sprintf('Not enough truck declared in the preference file "install.xml"'));
    else if (conf->nRob < 1)
        displayError(sprintf('Field "nrob" must be greater than 0 in function customConfig()'));

    if (!isvar(conf, 'enableBattMiss'))
        displayError(sprintf('Field "enableBattMiss" is not configured in function customConfig()'));
    else if (!istype(conf->enableBattMiss,'long'))
        displayError(sprintf('Field "enableBattMiss" is not an integer in function customConfig()'));
    else if (conf->enableBattMiss !=0 && conf->enableBattMiss !=1)
        displayError(sprintf('Field "enableBattMiss" should be equal to 0 or 1 in function customConfig()'));

    if (!isvar(conf, 'battPntStrategy'))
        displayError(sprintf('Field "battPntStrategy" is not configured in function customConfig()'));
    else if (!istype(conf->battPntStrategy,'char'))
        displayError(sprintf('Field "battPntStrategy" is not a string in function customConfig()'));
    else if (strcmp(conf->battPntStrategy,'individual') && strcmp(conf->battPntStrategy,'collective') && strcmp(conf->battPntStrategy,'taxi') && strcmp(conf->battPntStrategy,'taxiOpti'))
        displayError(sprintf('Field "battPntStrategy" is not "individual" or "collective" in function customConfig()'));

    if (!isvar(conf, 'scheduler'))
        displayError(sprintf('Field "scheduler" is not configured in function customConfig()'));
    else if (!istype(conf->scheduler,'char'))
        displayError(sprintf('Field "scheduler" is not a string in function customConfig()'));
    else if (strcmp(conf->scheduler,'FIFO') && strcmp(conf->scheduler,'closest') && strcmp(conf->scheduler,'LIFO'))
        displayError(sprintf('Field "scheduler" is not "FIFO", "closest" or "LIFO" in function customConfig()'));

    if (!isvar(conf, 'startCollectData'))
        displayError(sprintf('Field "startCollectData" is not configured in function customConfig()'));
    else if (!istype(conf->startCollectData,'long'))
        displayError(sprintf('Field "startCollectData" is not an integer in function customConfig()'));
    else if (conf->startCollectData < 0)
        displayError(sprintf('Field "startCollectData" should be at least 1 hour in function customConfig()'));

    if (!isvar(conf, 'simulationRunTime'))
        displayError(sprintf('Field "simulationRunTime" is not configured in function customConfig()'));
    else if (!istype(conf->simulationRunTime,'long'))
        displayError(sprintf('Field "simulationRunTime" is not an integer in function customConfig()'));
    else if (conf->simulationRunTime < 1)
        displayError(sprintf('Field "simulationRunTime" should be at least 1 hour in function customConfig()'));
    //else if (conf->simulationRunTime > 172800)
    //    displayError(sprintf('Field "simulationRunTime" should not exceed 48 hours in function customConfig()'));

    if (!isvar(conf, 'pallets'))
        displayError(sprintf('Field "pallets" is not configured in function customConfig()'));
    else if (!istype(conf->pallets,'obj'))
        displayError(sprintf('Field "pallets" is not a cell in function customConfig()'));
    else if (size(conf->pallets) == 0)
        displayError(sprintf('Field "pallets" is empty in function customConfig'));
    k = for(0,size(conf->pallets)-1) {
        pal = conf->pallets.{k};
        if (!istype(pal,'obj'))
            displayError(sprintf('Field "pallets" is not a list of cell in function customConfig()'));
        else if (size(pal) != 3)
            displayError(sprintf(['Field "pallets" requires 3 arguments for pallet ', num2str(k+1),' in function customConfig()']));
        else if (!istype(pal.{0},'char'))
            displayError(sprintf(['Field "pallets" : argument 1 is not a character for pallet ', num2str(k+1),' in function customConfig()']));
        else if (!istype(pal.{1},'cpx'))
            displayError(sprintf(['Field "pallets" : argument 2 is not a cpx for pallet ', num2str(k+1),' in function customConfig()']));
        else if (!istype(pal.{2},'char'))
            displayError(sprintf(['Field "pallets" : argument 3 is not a character ', num2str(k+1),' in function customConfig()']));
    }

    if (!isvar(conf, 'factor'))
        displayError(sprintf('Field "factor" is not configured in function customConfig()'));
    else if (!istype(conf->factor,'double') && !istype(conf->factor,'long'))
        displayError(sprintf('Field "factor" is not a number'));
    else if (conf->factor <= 0.0)
        displayError(sprintf('Field "factor" should be strictly higher than 0.0'));

    if (!isvar(conf, 'missions'))
        displayError(sprintf('Field "missions" is not configured in function customConfig()'));
    else if (!istype(conf->missions,'obj'))
        displayError(sprintf('Field "missions" is not a cell in function customConfig()'));

    //check that all missions are well defined
    missions_titles = strn("");
    i = for(0,size(conf->missions)-1) {
        task = conf->missions.{i};

        totalStep = 0;
        totalFlowStep = 0;

        if (!istype(task,'varspc'))
            displayError(sprintf('Field "missions" is not a list of varspc in function customConfig()'));

        if (!isvar(task, 'title'))
            displayError(sprintf('Field "missions->title" is not configured in function customConfig()'));
        else if (!istype(task->title,'char'))
            displayError(sprintf('Field "missions->title" is not a string in function customConfig()'));

        // check existence of [space] character
        spaceElt = strfind(task->title,' ');
        if(!isempty(spaceElt)) displayError(sprintf('Title "%s" contains invalid space character. Please use "_" instead.',task->title));
        task->title = strReplace(task->title,' ','_');

        // check unicity of missions title
        found = findElt(missions_titles, task->title);
        if(!isempty(found))   displayError(sprintf('Title "%s" (id %i) already exists (id %z)',task->title, i, found));
        else        missions_titles.(i) = task->title;

        if (!isvar(task, 'enable'))
            displayError(sprintf('Field "missions->enable" is not configured for mission "%s"',task->title));
        else if (!istype(task->enable,'long'))
            displayError(sprintf('Field "missions->enable" is not a number for mission "%s"',task->title));
        else if (task->enable != 0 && task->enable != 1)
            displayError(sprintf('Field "missions->enable" should be 0 or 1 for mission "%s"',task->title));

        if (!isvar(task, 'ratePerHour'))
            displayError(sprintf('Field "missions->ratePerHour" is not configured for mission "%s"',task->title));
        else if (!istype(task->ratePerHour,'long') && !istype(task->ratePerHour,'double'))
            displayError(sprintf('Field "missions->ratePerHour" is not a number for mission "%s"',task->title));
        else if (task->ratePerHour < 0)
            displayError(sprintf('Field "missions->ratePerHour" should be > 0 for mission "%s"',task->title));

        if (!isvar(task, 'group'))
            logWarn('Field "missions->group" is not configured for mission "%s" --> all trucks authorized by default !',task->title);
        else if (!isempty(task->group) && !istype(task->group,'long'))
            displayError(sprintf('Field "missions->group" is not a number or list of numbers for mission "%s"',task->title));
        //else if (!isFunc(task->group))
        //  displayError(sprintf('Field "missions->group (%s)": function doesn t exists',task->group));

        if (!isvar(task, 'missAuto'))
            displayError(sprintf('Field "missions->missAuto" is not configured for mission "%s"',task->title));
        else if (!istype(task->missAuto,'long'))
            displayError(sprintf('Field "missions->missAuto" is not a number for mission "%s"',task->title));
        else if (task->missAuto != 0 && task->missAuto != 1)
            displayError(sprintf('Field "missions->missAuto" should be 0 or 1 for mission "%s"',task->title));

        if (!isvar(task, 'objectives'))
            displayError(sprintf('Field "missions->objectives" is not configured for mission "%s"',task->title));
        else if (!istype(task->objectives,'obj'))
            displayError(sprintf('Field "missions->objectives" is not a cell for mission "%s"',task->title));
        else if (size(task->objectives) == 0)
            displayError(sprintf('Field "missions->objectives" is empty'));

        j = for(0,size(task->objectives)-1) {
            step = task->objectives.{j};
            totalStep = totalStep + 1;

            if (!istype(step,'varspc'))
                displayError(sprintf('Field "missions->objectives" is not a list of varspc for mission "%s"',task->title));

            if (!isvar(step, 'task'))
                displayError(sprintf('Field "missions->objectives->task" is not configured for mission "%s"',task->title));
            else if (!istype(step->task,'char'))
                displayError(sprintf('Field "missions->objectives->task" is not a string for mission "%s"',task->title));
            else if (strcmp(step->task,'pick') && strcmp(step->task,'drop') && strcmp(step->task,'move'))
                displayError(sprintf('Field "missions->objectives->task" is != "pick" | "drop" | "move"'));

            if(!isvar(step,'dst') && !isvar(step,'grp'))
                displayError(sprintf('Field "missions->objectives->dst" or "missions->objectives->grp" is not configured in function customConfig'));
            else if (isvar(step,'dst') && isvar(step,'grp'))
                displayError(sprintf('You cannot have a field "missions->objectives->dst" AND "missions->objectives->grp" Keep one.'));
            else if (isvar(step,'dst') && !istype(step->dst,'char'))
                displayError(sprintf('Field "missions->objectives->dst" is not a string for mission "%s"',task->title));
            else if(!strcmp(step->task,'move') && isvar(step,'grp'))
                displayError(sprintf('Cannot use "grp" points for a "move" mission. Please use single "dst".'));
            else if (isvar(step,'grp') && !istype(step->grp,'char') && !istype(step->grp,'obj'))
                displayError(sprintf('Field "missions->objectives->grp" is not a string nor a cell for mission "%s"',task->title));

            if (isvar(step, 'nbPal')) {
                if (!istype(step->nbPal,'long'))
                    displayError(sprintf('Field "missions->objectives->nbPal" is not a number for mission "%s"',task->title));
                else if (step->nbPal < 0)
                    displayError(sprintf('Field "missions->objectives->nbPal" should be >= 0 for mission "%s"',task->title));
            }
            if (isvar(step, 'slot')) {
                if (!istype(step->slot,'long'))
                    displayError(sprintf('Field "missions->objectives->slot" is not a number for mission "%s"',task->title));
                else if (step->slot < 0)
                    displayError(sprintf('Field "missions->objectives->slot" should be >= 0 for mission "%s"',task->title));
            }
            if (isvar(step, 'stopTime'))
                displayError(sprintf('Field "missions->objectives->stopTime" is deprecated. Please use approachTime or destinationTime.'));
            if(isvar(step,'approachTime')) {
                if (!istype(step->approachTime,'long') && !istype(step->approachTime,'double'))
                    displayError(sprintf('Field "missions->objectives->approachTime" is not a number for mission "%s"',task->title));
                else if (step->approachTime < 0)
                    displayError(sprintf('Field "missions->objectives->approachTime" should be > 0 for mission "%s"',task->title));
            }

            if(!isvar(step,'destinationTime') && !strcmp(step->task,'move'))
                displayError(sprintf('Field "destinationTime" is missing for miss "%s" (Mandatory for "move" tasks)',task->title));
            if(isvar(step,'destinationTime')) {
                if (!istype(step->destinationTime,'long') && !istype(step->destinationTime,'double'))
                    displayError(sprintf('Field "missions->objectives->destinationTime" is not a number for mission "%s"',task->title));
                else if (step->destinationTime < 0)
                    displayError(sprintf('Field "missions->objectives->destinationTime" should be > 0 for mission "%s"',task->title));
            }

            if(isvar(step,'detectionTime')) {
                if (!istype(step->detectionTime,'long') && !istype(step->detectionTime,'double'))
                    displayError(sprintf('Field "missions->objectives->detectionTime" is not a number for mission "%s"',task->title));
                else if (step->detectionTime < 0)
                    displayError(sprintf('Field "missions->objectives->detectionTime" should be > 0 for mission "%s"',task->title));
            }

            if (isvar(step, 'strategy') && !istype(step->strategy,'char'))
                displayError(sprintf('Field "missions->objectives->strategy" is not a string for mission "%s"',task->title));

            if (isvar(step, 'pallet') && !istype(step->pallet,'char'))
                displayError(sprintf('Field "missions->objectives->pallet" is not a string for mission "%s"',task->title));
        }

        if (!isvar(task, 'flows'))
            displayError(sprintf('Field "missions->flows" is not configured for mission "%s"',task->title));
        else if (!istype(task->flows,'obj'))
            displayError(sprintf('Field "missions->flows" is not a cell for mission "%s"',task->title));
        else if (size(task->flows) == 0)
            displayError(sprintf('Field "missions->flows" is empty'));

        j = for(0,size(task->flows)-1) {
            flow = task->flows.{j};

            if (!istype(flow,'varspc'))
                displayError(sprintf('Field "missions->flows" is not a list of varspc for mission "%s"',task->title));

            if (!isvar(flow, 'flowName'))
                displayError(sprintf('Field "missions->flows->flowName" is not configured for mission "%s"',task->title));
            else if (!istype(flow->flowName,'char'))
                displayError(sprintf('Field "missions->flows->flowName" is not a string for mission "%s"',task->title));

            if (!isvar(flow, 'flowDisplay'))
                displayError(sprintf('Field "missions->flows->flowDisplay" is not configured for mission "%s"',task->title));
            else if (!istype(flow->flowDisplay,'long'))
                displayError(sprintf('Field "missions->flows->flowDisplay" is not an integer for mission "%s"',task->title));
            else if ((flow->flowDisplay != 0) && (flow->flowDisplay != 1))
                displayError(sprintf('Field "missions->flows->flowDisplay" should be 0 or 1 for mission "%s"',task->title));

            if (!isvar(flow, 'numberOfStep'))
                displayError(sprintf('Field "missions->flows->numberOfStep" is not configured for mission "%s"',task->title));
            else if (!istype(flow->numberOfStep,'long'))
                displayError(sprintf('Field "missions->flows->numberOfStep" is not a number for mission "%s"',task->title));
            else if (flow->numberOfStep < 1)
                displayError(sprintf('Field "missions->flows->numberOfStep" should be > 0 for mission "%s"',task->title));

            totalFlowStep = totalFlowStep + flow->numberOfStep;
        }

        if (totalFlowStep > totalStep)
            displayError(sprintf('Total Flow Step != Total step, Total Flow Step is too big for mission "%s"',task->title));
        else if (totalFlowStep < totalStep)
            displayError(sprintf('Total Flow Step != Total step, Total Flow Step is too low for mission "%s"',task->title));
    }

    if(0 == conf->enableLogReplay && 1 == conf->enableReplayDB) {
      logWarn("[REPLAY] Replay DB cannot be enabled without LogReplay : Disable ReplayDB.");
      global g('simConf');
      g->enableReplayDB = 0;
    }

}


/*
    Function called when the simulation is going to stop
*/
export function simEndCbk()
{

}
