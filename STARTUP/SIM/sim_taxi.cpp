
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                                 TAXI FUNCTIONS                              //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Initialisation of robots, taxi and battery missions
*/
export function initRobTaxi() {
    global g('simConf');

    newTaskDef('TAXI','pgm','rob_taxi'); // creation of taxi task
    g->VNA_frkSide = {};

    i = for(1,g->nRob) {
        taxi = sprintf('taxi%e',i);
        robModel = getRobModel(i);

        // check taxi position and create rob
        if(findPntName(robModel,taxi)) {
            taxiPnt = TRACK_GetDestinationPoint(robModel,taxi).point;
            taxiCpx = cpx(taxiPnt.{0}.xposition,taxiPnt.{0}.yposition);
              logConsoleLevel(5); //remove logs to not spamm console when creating robots
            newRobot(robModel,taxiCpx);
              logConsoleLevel(g->logConsoleLevel); //set the log level back to its real value
            drawrob('newrob',i);
            g->taxi.{i-1} = taxiCpx; //definition du taxi
        }
        else displayError(sprintf("POSITION 'taxi%i' (rob %i - layer %s ) DOESNT EXIST !",i,i,robModel));

        //init frkSide (used only for VNAs)
        g->VNA_frkSide.{end+1} = '';
    }
    // clear windows
    // clc();

    // initialize the robots global
    initRobGlobal();

    // initialises the taxi time counter
    initTaxiCounter();
}

/*
    Initialize the robots global
*/
export function initRobGlobal() {
  global r('simRob');
  global g('simConf');

  // number of robots
  nr = g->nRob;

  // init global structure
  r->id             = new('long',nr);   // id of the robot
  r->serial         = new('L.char',nr); // serial number
  r->model          = new('L.char',nr); // robot model
  r->currentDst     = new('L.char',nr);   // current destination of the robot (TODO : remove this and use SDK instead)
  r->vmaxforward    = new('double',nr); // max forward speed
  r->vmaxbackward   = new('double',nr); // max backward speed
  r->vmaxlateral    = new('double',nr); // max lateral speed
  r->accelcarry     = new('double',nr); // acceleration (carry)
  r->accelempty     = new('double',nr); // acceleration (empty)
  r->decelcarry     = new('double',nr); // deceleration (carry)
  r->decelempty     = new('double',nr); // deceleration (empty)
  r->timeCarry      = new('long',nr);   // time running carry
  r->timeEmpty      = new('long',nr);   // time running empty
  r->timeTaxiStart  = new('long',nr);   // taxi start time counter
  r->timeTaxiEnd    = new('long',nr);   // taxi end time counter
  r->timeTaxiState  = new('long',nr);   // taxi state : 1 = moving, 2 = static
  r->timeTaxiMove   = new('long',nr);   // time spent moving to taxi position
  r->timeTaxiStop   = new('long',nr);   // time spent at taxi position
  r->timeBattMove   = new('long',nr);   // time spent moving to charger
  r->timeBattStop   = new('long',nr);   // time spent at charger position

  // fill the global with the preferences
  i = for(1,nr) {
    serial = number2Serial(i);
    r->id.(i-1)           = i;
    r->serial.{i-1}       = serial;
    r->model.{i-1}        = getRobModel(i);
    r->currentDst.{i-1}   = sprintf("taxi%i",i);
    r->vmaxforward.(i-1)  = getPreferenceValue("trajectoryDriver/speedLimit/maxForwardSpeed",serial);
    r->vmaxbackward.(i-1) = getPreferenceValue("trajectoryDriver/speedLimit/maxBackwardSpeed",serial);
    r->vmaxlateral.(i-1)  = getPreferenceValue("trajectoryDriver/speedLimit/maxLateralSpeed",serial);
    r->accelcarry.(i-1)   = getPreferenceValue("trajectoryDriver/speedLimit/accelerationCarry",serial);
    r->accelempty.(i-1)   = getPreferenceValue("trajectoryDriver/speedLimit/accelerationEmpty",serial);
    r->decelcarry.(i-1)   = getPreferenceValue("trajectoryDriver/speedLimit/decelerationCarry",serial);
    r->decelempty.(i-1)   = getPreferenceValue("trajectoryDriver/speedLimit/decelerationEmpty",serial);
  }

}


/*
    Initialize the taxi counter variables
*/
export function initTaxiCounter() {
    global r('simRob');

    // initialize taxi var for each rob
    k = for(1,size(r->id)) {
        // start recording taxi miss
        startTaxi(k);
        // set the robots at static state
        r->timeTaxiState.(k-1) = 2;
    }

    logDebug('[TAXI] Rob %i-%i taxi time initialized',1,(size(r->id)));
}


/*
    Start recording taxi time
    @param idrob [long] -> id of the robot
*/
export function startTaxi(idrob) {
    global r('simRob');

    // record that taxi mission has started
    simuTime = simGetT();
    r->timeTaxiStart.(idrob-1) = simuTime;
    r->timeTaxiEnd.(idrob-1) = 0;
    r->timeTaxiState.(idrob-1) = 1;

    // logDebug('Agv %i started taxi mission at %s',idrob,sprintHMS(simGetT));
}


/*
    Stop recording taxi moving time
    @param idrob [long] -> Id of the robot
*/
export function endTaxiMove(idrob) {
    global g('simConf');
    global r('simRob');

    // record that taxi mission has started
    simuTime = simGetT();

    if(simuTime > g->startCollectData) {
        r->timeTaxiMove.(idrob-1) = r->timeTaxiMove.(idrob-1) + simuTime - r->timeTaxiStart.(idrob-1);
        // logDebug('Saving taxiMoving time for rob %i (%s)',idrob,sprintHMS(simuTime - r->timeTaxiStart.(idrob-1)));
    }

    r->timeTaxiStart.(idrob-1) = 0;  //reset start counter
    r->timeTaxiEnd.(idrob-1) = simuTime; //store end counter
    r->timeTaxiState.(idrob-1) = 2; // set state to static

    // logDebug('Agv %i ended taxi mission at %s',idrob,sprintHMS(simGetT));
}


/*
    Save taxi time according to the robot state (moving or static)
    @param idrob [long] -> Id of the robot
*/
export function saveTaxiTime(idrob) {
    global g('simConf');
    global r('simRob');

    unint taxiState = r->timeTaxiState.(idrob-1);
    simuTime = simGetT();

    // save taxi time depending if the truck was moving or static
    switch (taxiState) {
        case(1); //taxi is moving
            if(simuTime > g->startCollectData) {
                if(r->timeTaxiMove.(idrob-1)==0 && r->timeTaxiStart.(idrob-1) < g->startCollectData) r->timeTaxiStart.(idrob-1) = g->startCollectData;
                r->timeTaxiMove.(idrob-1) = r->timeTaxiMove.(idrob-1) + (simuTime - r->timeTaxiStart.(idrob-1));
                // logDebug('Saving taxiMoving time for rob %i (%s)',idrob,sprintHMS(simuTime - r->timeTaxiStart.(idrob-1)));
            }
            r->timeTaxiStart.(idrob-1) = 0;
            r->timeTaxiEnd.(idrob-1) = 0;
            r->timeTaxiState.(idrob-1) = 0;
        case(2); //taxi is static
            if(simuTime > g->startCollectData) {
                if(r->timeTaxiStop.(idrob-1)==0 && r->timeTaxiEnd.(idrob-1) < g->startCollectData) r->timeTaxiEnd.(idrob-1) = g->startCollectData;
                r->timeTaxiStop.(idrob-1) = r->timeTaxiStop.(idrob-1) + (simuTime - r->timeTaxiEnd.(idrob-1));
                // logDebug('Saving taxiStatic time for rob %i (%s)',idrob,sprintHMS(simuTime - r->timeTaxiEnd.(idrob-1)));
            }
            r->timeTaxiStart.(idrob-1) = 0;
            r->timeTaxiEnd.(idrob-1) = 0;
            r->timeTaxiState.(idrob-1) = 0;
        default;
            // if(simuTime > g->startCollectData)
            // logWarn('saveTaxiTime called for robot %i but state = %i, doing nothing...',idrob,taxiState);
            return;
    }
}


/*
    Taxi mission, called when the robot has no missions available (go to taxi position and wait)
    @param idrob [long] -> Id of the robot
*/
export function rob_taxi() {
    global g('simConf');

    //logInstall('[TAXI] Rob %i is available to take a mission.',robGetId());

    // get rob info
    idrob = robGetId;
    serial = number2Serial(idrob);
    flag = getPreferenceValue("navigation/pathFlag",serial);
    //cantonSetTrailerLength(idrob,0.0);

    // ** USEFUL LENGHTS **
    DIST_ROLLER_BACKREST      =  abs(getPreferenceValue("general/backrestAbscissa",serial));
    DIST_ROLLERS_ENDFRK       =  abs(getPreferenceValue("general/forksEndAbscissa",serial));
    taxiForkOffest			      =  DIST_ROLLER_BACKREST + DIST_ROLLERS_ENDFRK + 0.2;

    // mini initMissVarspc
    taxiMiss = varspc();
    taxiMiss->travelLimit = getPreferenceValue("safety/design/maxHeightOkForMovement",serial);
    taxiMiss->vmaxEvac = getPreferenceValue("trajectoryDriver/speedLimit/vmaxEvac",serial);

    //start recording taxi miss
    startTaxi(idrob);

    // check that itineray exists
    [i,j] = TRACK_FindItinerary(getRobModel(idrob),robGetZ,g->taxi.{idrob-1},flag);
    if(isempty(i) && isempty(j)) {
        displayError(sprintf("[rob_taxi] Itinerary not found! From rob %i (%z) to taxi%i (%z).",idrob,robGetZ,idrob,g->taxi.{idrob-1}));
        logError('Check your circuit.');
        return;
    }
    // navigate to taxi pnt
    gotoPnt(g->taxi.{idrob-1}, flag);

    // evacuation
    robSetvmax(taxiMiss->vmaxEvac); //set evac speed
    rpkEvac = robGetrpk() - taxiForkOffest; //compute evac dist
    if(rpkEvac > 0) robWaitrpk(rpkEvac); // wait evac dit

    robMoveForksTravel(taxiMiss,'Forward','evac_taxi');

    // set maximum forward speed
    robSetvmax(10.);

    // available to take a mission
    robDispo(1);

    // check the availability of missions
    simSchdLaunchTask();

    // end of recording taxi mission
    endTaxiMove(idrob);
}
