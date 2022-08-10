
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                              USEFUL FUNCTIONS                               //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Function to return flag model (from preferences) TODO: add pref v4.
    @param idrob [long] -> id of the robot
    @return flag [long] -> flag of the selected robot
*/
export function flag = getModelFlag(idrob) {
    serial = number2Serial(idrob); // get the serial of the truck
    try flag = getPreferenceValue("navigation/pathFlag",serial); // get the model path flag
    catch flag = getPreferenceValue("navigation/pathFlag","INSTALL");
}


/*
    Function to return if model is Kmatic
    @param idrob [long] -> id of the rob
    @return isK [bool] -> 1 if Kmatic 0 otherwise
*/
export function isK = isKmatic(idrob) {
    if(nargin<1) idrob = robGetId;
    isK = new('long',1);

    serial = number2Serial(idrob); // get the serial of the truck
    truckType = getPreferenceValue("general/truckType",serial); // get the truck type from preferences

    if(!strcmp(truckType,'VNA')) isK = 1;
}


/*
    Function to return if model is Rmatic
    @param idrob [long] -> id of the rob
    @return isR [bool] -> 1 if Rmatic 0 otherwise
*/
export function isR = isRmatic(idrob) {
    if(nargin<1) idrob = robGetId;
    isR = new('long',1);

    serial = number2Serial(idrob); // get the serial of the truck
    truckType = getPreferenceValue("general/truckType",serial); // get the truck type from prefereces

    if(!strcmp(truckType,'ReachTruck')) isR = 1;
}


/*
    Function to return if model is a tugger
    @param idrob [long] -> id of the rob
    @return isT [bool] -> 1 if tugger 0 otherwise
*/
/*
export function isT = isTugger(idrob) {
    if(nargin<1) idrob = robGetId;
    isT = new('long',1);

    serial = number2Serial(idrob); // get the serial of the truck
    truckType = getPreferenceValue("general/truckType",serial); // get the truck type from preferences

    if(!strcmp(truckType,'Tugger')) isT = 1;
}
*/

/*
    Returns 1 if the robot is at its taxi position
    @param idrob [long] -> robot id
    @return out [bool] -> 1 if robot at its taxi point, 0 otherwise
*/
export function out = isRobotAtTaxi(idrob) {
  global g('simConf');

  if(nargin<1) idrob = robGetId();

  //initialise out
  out = 0;

  // get robot and taxi position (cpx)
  robPos = robGetZ(idrob);
  taxiPos = g->taxi.{idrob-1};

  // get the current task on robot
  currentTask = robFindTask(idrob);

  if(!strcmp(currentTask, "noTask") && (abs(robGetSpeed(idrob)) < 0.05) && (abs(dist(robPos,taxiPos)) < 0.15)) out = 1;

  return;
}

/*
    Function that return if vertical forks and LI are enabled
    @param idrob [long] -> id of the rob
    @return verticalForksEnabled [bool] -> 1 if vertical forks enabled 0 otherwise
    @return initialLiftEnabled [bool] -> 1 if initial lift enabled 0 otherwise
    @return moveWithInitialLiftUp [bool] -> 1 is robot move with LI up 0 otherwise
*/
export function [verticalForksEnabled,initialLiftEnabled,moveWithInitialLiftUp] = areForksEnabled(idrob) {
  //get robot serial
  if(nargin < 1) idrob = robGetId();
  serial = number2Serial(idrob);

  //get value from preferences
  verticalForksEnabled = getPreferenceValue("forksActuators/verticalForks/enable",serial);
  initialLiftEnabled = getPreferenceValue("forksActuators/initialLift/enable",serial);
  moveWithInitialLiftUp = getPreferenceValue("forksActuators/initialLift/moveWithInitialLiftUp",serial);
}

/*
    Registration of all points and groups
    @return pntLs [L.char] -> list of all points from circuit
    @return grpLs [L.char] -> list of all group points from circuit
    @return nbPerGrp [long] -> number of points for each group
*/
export function [pntLs,grpLs,nbPerGrp] = cirGetAllPnt() {


    pntLs = {}; // list of the points of the circuit (registered by name)
    grpLs = {}; // list of the groups of points (registered by name)
    nbPerGrp = new('long',0); // Number of points per group
    /*
    allPnt =  cirFindAllPnt();

    p = for(allPnt) { // all the points are reviewed
        pntName = cirGetPntName(p);
        pntLs.{end+1} = pntName; // add the point name in list pntLs

        if(strMatch(pntName,'TOPO(KERN)')) continue; // skip point if TOPO(KERN)

        // search for last underscore in point name
        underIdx = strfind(pntName,'_');
        if(!isempty(underIdx)) {
            prefix = pntName.(lin(0,(underIdx.(end)-1)));
            suffix = pntName.(lin(underIdx.(end)+1,size(pntName)-1));

            if (isNumeric(str2num(suffix))) { // if there are only numerals after the underscore (= what we are searching for)
                // add the group name to grpLs if not registered yet (and update nbPerGroup)
                grpIdx = findElt(grpLs,prefix);
                if (isempty(grpIdx)) {
                    grpLs.{end+1} = prefix;
                    nbPerGrp.(end+1) = 1;
                }
                else {
                    nbPerGrp.(grpIdx) = nbPerGrp.(grpIdx) + 1;
                }
            }
            else nbPerGrp.(end+1) = 1;
        }
        else nbPerGrp.(end+1) = 1;
    }
    */
    logError('[cirGetAllPnt] not developped yet.');
}

/*
  function to find if a pnt in the track
  @param layer [string] -> name of the layer
  @param pnt [string] -> name of the point
  @return out [boolean] -> 1 : point exist / 0 : point doesnt exist
*/
export function out = findPntName(layer,pnt) {
    out = 0;
    allPnt = TRACK_GetAllDestinationPoints(layer);
    i = for(allPnt) {
        if(!strcmp(allPnt.(i).name{0},pnt)) {
            out = 1;
            break;
        }
    }
}

/* function to get all points of a particular type from a layer
  @param type [string] -> type of point (either "normal", "init", "taxi" or "charger")
  @param layer [string] -> name of the layer
  @return typePnts [L.string] -> list of points names
*/
export function typePnts = getDestinationTypePoints(type,layer) {
    typePnts = {};

    //get all desti points from layer
    allPnts = TRACK_GetAllDestinationPoints(layer);
    //loop over all points
    i = for(allPnts) {
        //retrieve data from a point
        point = allPnts.(i);
        //check type
        switch(type) {
            case('normal');
                if(point.type_normal == 1) typePnts.{end+1} = point.name{0};
            case('init');
                if(point.type_init == 1) typePnts.{end+1} = point.name{0};
            case('taxi');
                if(point.type_taxi == 1) typePnts.{end+1} = point.name{0};
            case('charger');
                if(point.type_charger == 1) typePnts.{end+1} = point.name{0};
        }
    }
}


/*
  function to return all available points for a robot
  @param layer [string] -> layer of the robot
  @return ids [L.long] -> list of all ids
*/
export function ids = getLayerAllIds(layer) {
    // init ids list
    ids = [];

    // get rack pallet ids
    rackPalNames = TRACK_GetAllRackPalletNames();
    p = for(rackPalNames) ids.(end+1) = name2id(rackPalNames.{p});

    // get destination point ids (from layer)
    destiPntIds = TRACK_GetAllDestinationPoints(layer).id;

    // get stock line ids
    stockZoneIds = TRACK_GetAllStockZone().Id;
    stockLineIds = TRACK_GetAllStockLine().Id;

    // return all ids
    ids = [ids, destiPntIds, stockZoneIds, stockLineIds];
}


/*
  function to get the type of a point from its id
  @param id [long] -> id of the destination point
  @return typePnt [string] -> type of the point
*/
export function getPointTypeFromId(id) {

}


/*
    Return the strategy name associated to its id (useless ?)
    @param num [long] -> id of the strategy
    @return str [char] -> name of the strategy
*/
export function str = num2strategy(num) {
    switch(num) {
        case(1);
            str = "furthest";
        case(2);
            str = "nearest";
        case(3);
            str = "equality";
        case(4);
            str = "random";
        default;
            str = "UNK";
    }
}


/*
    Check if the simulation needs to stop
*/
export function isSimEndTime() {
    global g('simConf');

    simT = simGetT(); // get simulation time simT
    // logInstall("Checking the if the simulation time is overpassed...");
    if (simT >= g->simulationRunTime  + g->startCollectData) { // stop if simT is higher than the wanted simulation run time
        logDebug("Simulation time is over");
        printf('[%s] Simulation end time reached, stopping...\n',sprintHMS(simGetT));
        g->simulationEndTime = simT; // store simulation end time
        stopSim();
        writeSimResults(); // writes results in console
        // launches callback if the function exists
        if (isFunc('simEndCbk')) {
            acall('simEndCbk');
        }
    }
}
// global g('simConf');
// if (simGetT() >= g->simulationRunTime  + g->startCollectData){
//     logDebug("Se ha pasado el tiempo de simulación");
// } else{logDebug("La simulación continua");}


/*
    Function to stop the simulation
*/
export function stopSim() {
    // kiwi global
    global w(getSimxGlobalId());

    // sim global (windows)
    global g('sim');

    if(isvar(g,'run') && g->run)
    {
        g->run=0;
        if(strMatch(w->edapp,'kiwiEditor'))
            set(w->hplay,'bitmap',acall('kiwiEditorBmp','play'));
        else
            set(g->hgo,'bitmap',acall('simxBmp','play'));
    }
}

/*
    Function to return the duration of the simulation
    @return duration [long] -> simulation duration (seconds)
*/
export function duration = simGetDuration() {
  global g('simConf');

  if(g->simulationStartTime < 0) {
    logWarn("[simGetDuration] Simulation didnt reach startCollectData yet !");
    duration = simGetT;
  }
  else if(g->simulationEndTime < 0) {
    logWarn("[simGetDuration] Simulation didnt reach simulationRunTime yet !");
    duration = simGetT - g->simulationStartTime;
  }
  else {
    duration = g->simulationEndTime - g->simulationStartTime;
  }

  //logInfo("[simGetDuration] The simulation record lasted %s", sprintHMS(duration));
}


/*
    Transform a string into a cell of float
    @param str [char] -> string to transfomr (ex: str = "[1.25,1.5,2]" )
    @return cell [L.float] -> cell of float (ex: cell = [1.25,1.5,2] )
*/
export function cell = str2cellNum(str) {
    cell = new('double',0);
    commas = strfind(str,',');
    if(!isempty(commas)) {
        if(size(commas)==1) {
            cell.(end+1) = str2num(str.(lin(1,commas-1)));
            cell.(end+1) = str2num(str.(lin(commas+1,(size(str)-2))));
        }
        else {
            c = for(commas) {
                if (c==0) cell.(end+1) = str2num(str.(lin(1,commas.(c)-1)));
                else if (c==(size(commas)-1))  {
                    cell.(end+1) = str2num(str.(lin(commas.(c-1)+1,commas.(c)-1)));
                    cell.(end+1) = str2num(str.(lin(commas.(c)+1,(size(str)-2))));
                }
                else cell.(end+1) = str2num(str.(lin(commas.(c-1)+1,commas.(c)-1)));
            }
        }
    }
    else cell = str2num(str.(lin(1,size(str)-2)));
}


/*
  Format a string with a separator
  @param string [char] -> original string
  @param length [long] -> length of the desired string
  @param sep [char] -> separator
*/
export function str = formatString(string, length, sep) {
    str = string;

    if (size(str) < length) {
        k = for(0, length - size(str) - 1)
            str = [str,sep];
    }
    else if (size(str) > length) {
        str = str.(lin(0, length -3));
        str= [str,'..'];
    }
}

/*
 * Cast numeric tab into a str
 *
 */
export function str = tab2str(tab) {
 str = '';
 // return if tab empty or size == 1
 if(isempty(tab)) return;

 // loop over all values
 i = for(tab) {
   if(0 == i) str = '[';
   else str = [str,', '];
   str = [str,num2str(tab.(i))];
 }

 str = [str,' ]'];

}

/*
  Displays the distances to travel for all the flows
*/
export function dispFlowDistances() { // TODO - Finish this function (problem with K)
    global g('simConf');

    i = for(g->missions) { // for all the missions that exist
        v = g->missions.{i};
        j = for(0,size(v->flows)-1) { // for all the flows of these missions
            if (!isempty(v->group)) {
                idrob = v->group.(0); // if there are defined authorized robots, we consider the first authorized one
            }
            else {
                idrob = 1; // if all robots are authorized, we consider the first robot
            }
            robModel = getRobModel(idrob); // 1. Robot model to consider
            serial = number2Serial(idrob);
            flag = getPreferenceValue("navigation/pathFlag", serial); // 2. Path flag to consider
            d = 0. ; // distance to travel for this flow (initialization)
            k = 1;

            while (k < v->flows.{j}->numberOfStep) { // we want to calculate all the distance to travel, whatever the number of steps
                // Start point
                if (isvar(v->objectives.{k-1}, "dst")) { // if the point is a unique destination
                    startPointData = missPntGetInfo(name2id(v->objectives.{k-1}->dst));
                }
                else if (isvar(v->objectives.{k-1}, "grp"))  { // if the point is a group
                    dst = chooseRealDestination(v->objectives.{k-1}->grp, 'random'); // we take one randomly /!\ MIGHT BE MISLEADING
                    startPointData = missPntGetInfo(name2id(dst));
                }
                zStartCpx = cpx(startPointData->zend.xposition, startPointData->zend.yposition); // 3. Start point
                // End point
                if (isvar(v->objectives.{k}, "dst")) { // if the point is a unique destination
                    endPointData = missPntGetInfo(name2id(v->objectives.{k}->dst));
                }
                else if (isvar(v->objectives.{k}, "grp"))  { // if the point is a group
                    dst = chooseRealDestination(v->objectives.{k}->grp, 'random'); // we take one randomly /!\ MIGHT BE MISLEADING
                    endPointData = missPntGetInfo(name2id(dst));
                }
                zEndCpx = cpx(endPointData->zend.xposition, endPointData->zend.yposition); // 4. End point

                [_,_,_,stepDist] = TRACK_FindItinerary(robModel, zStartCpx, zEndCpx, flag);
                d = d + stepDist;
                k = k + 1 ;
            }
            logInfo('Distance to travel for flow %s with robot %s (path flag %i) is %f meters', v->flows.{j}->flowName, robModel, flag, d);
        }
    }
}


/*
    Special logs for simulation replays
    @param color [string] -> type of the log : 'ERROR', 'WARN', 'DEBUG', 'INSTALL', 'INFO', 'CUSTOM'
                            or color : 'BLACK', 'BLUE', 'GREEN', 'CYAN', 'RED', 'MAGENTA', 'BROWN', 'LIGHTGRAY', 'DARKGRAY', 'LIGHTBLUE', 'LIGHTGREEN', 'LIGHTCYAN', 'LIGHTRED', 'LIGHTMAGENTA', 'YELLOW', 'WHITE', 'DEFAULT'
    @param content [string] -> content to display
*/
/*
export function logSim(color, content) {
  global g('simConf');

  // select correct color
  switch (toupper(color)) {
    case('ERROR');
        color = 'RED';
        type  = 'ERR-';
    case('WARN');
        color = 'LIGHTMAGENTA';
        type  = 'WARN';
    case('DEBUG');
        color = 'BLUE';
        type  = 'DBG-';
    case('INSTALL');
        color = 'GREEN';
        type  = 'INST';
    case('INFO');
        color = 'BLACK';
        type  = 'INFO';
    case('CUSTOM');
        color = 'MAGENTA';
        type  = 'CSTM';
    default;
        color = toupper(color);
        type  = formatString(color,4,'-');
  }

  // build log string
  logdate = sprintDate('%Y-%m-%d', time());
  logtime = sprintHMS(simGetT());
  logstrconsole = [logtime,'|',type,'| ',content,'\n']; // log to display
  logstrsave    = [logdate,'|',logtime,'|',type,'| ',content,'\n'];// log to keep

  // display log in console
  printcf(color,logstrconsole);

  // write log in replay file (TODO)
  if(g->enableLogReplay) logSimWriteReplay(logstrsave);
}*/
