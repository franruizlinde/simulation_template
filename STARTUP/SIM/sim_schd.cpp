
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                           SCHEDULER INITIALIZATION                          //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Initialize the scheduler DB
*/
export function initSimSchd() {
    logInfo('[SCHD] Initializing custom simSchdDB');

    // Get DB definition
    dbDef = simSchdDBDef();

    // Create DB from definition
    DBcreate_gbl('simSchdDB',dbDef,'id',0);

    // If keys, delete them all to re-start fresh
    keys = DBgetAllKeys_gbl('simSchdDB');
    if (size(keys)) {
        //logDebug('[SCHD] Deleting old keys %z',keys);
        DBdel_gbl('simSchdDB',keys);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                             SCHEDULER DEFINITION                            //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    definition of the scheduler DB
    @return def [varspc] -> definition of simSchdDB
*/
export function def = simSchdDBDef() {
    def = varspc;
    def->id = 0;                  // DB key, unique id
    def->taskName = strn('');     // Name of the task
    def->missId = -1;             // Mission id (=/= taskId) from conf
    def->pgm = strn('');          // pgm of the mission
    def->addDate = 0;             // timestamp for add date
    def->startDate = 0;           // timestamp for start date
    def->endDate = 0;             // timestamp for end date
    def->state = 0;               // 0: waiting, 1:sending 2:running, 3:done
    def->robAuth = {[]};          // allows to specify a list of rob for the task
    def->rob = 0;             	  // rob running the task
    def->priority = 1;            // task priority
    def-> srcPoint = strn('');            // source point from the csv file
    def-> dstPoint = strn('');            // dest point from the csv file
    def-> idCSV = 0;              // Id of the mission in the csv file

}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                             SCHEDULER FUNCTIONS                             //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Add a task in the pcsol
    @param miss [char] -> mission Name
    @param id [long] -> mission index (from g->missions)
    @param priority [long] -> mission priority
*/
export function addTaskInPcsol(miss, id, priority, varsCSV) {
    global g('simConf');
    logDebug('[ADD TASK][sim_schd] varsCSV = %z',varsCSV);
    // task data
    v = varspc();
    v->taskName = miss;
    v->priority = str2num(priority);
    v->missId = str2num(id);
    v->pgm = 'robMiss3';
    v->dstPoint = varsCSV->dstPoint;
    v->srcPoint = varsCSV->srcPoint;
    v->idCSV = varsCSV->idCSV;
    // add task to sim DB
    taskId = simSchdTaskAdd(v);

    //check if there is available missions and rob => trigger them
    simSchdLaunchTask();
}

/*
    Function that launches tasks if there are available robots and available missions
    @param idrob [long] -> id of a particular robot (optional)
*/
export function isLaunched = simSchdLaunchTask(idrob) {
    global g('simConf');
    //boolean to return if the function successfully launched a mission or not
    isLaunched = 0;

    //find available robs if none in argument
    freeRob = (nargin<1)? simGetAvailableRob() : idrob;

    //for each rob check if there are mission waiting available
    if (!isempty(freeRob)) {
        //logInstall('[SCHD] Rob %z available',freeRob);
        k = for(0,size(freeRob)-1) {
            // find waiting task authorized for each rob
            authMissIds = simSchdFindTaskAuth(freeRob.(k),'waiting');

            if(!isempty(authMissIds)) {
                // find highest priority amongs authorized miss
                prioIds = simSchdFindTaskPrio(authMissIds);

                // choice of mission then depend of scheduler
                switch(g->scheduler) {
                    case('FIFO'); // * * * * * * * * * FIFO * * * * * * * * * * * * *
                        chosenId = prioIds.(0); //choose first created mission
                        break;

                    case('LIFO'); // * * * * * * * * * LIFO * * * * * * * * * * * * *
                        chosenId = prioIds.(end); //choose last created mission
                        break;

                    case('closest'); // * * * * * * closest * * * * * * * * * * * * *
                        // The priority ID is the id from the csv file
                        //chosenId = prioIds.()
                        chosenId = simSchdFindTaskClosest(freeRob.(k),prioIds); //choose closest mission
                        break;

                    default;
                        displayError('[SCHD] No custom behavior defined ! Please choose "FIFO", "LIFO" or "closest"');
                }
                //if no id chosen return
                if(isempty(chosenId)) return;

                //get data of the chosen mission with highest priority
                [taskName,missId,missPgm] = simSchdGetTaskData(chosenId);

                // Create callback for each mission
                pgm  = sprintf('$%s %i %i',strn(missPgm),missId,chosenId);
                chooseCbk = 'iRobot';

                // task definition
                newTaskDef(taskName,'pgm',pgm,'chooseCbk',chooseCbk);

                // launch task on pcsol to particular rob
                pcsolNewTask(taskName,num2str(freeRob.(k)));
                isLaunched = 1;

                // call create callback
                taskCreateCbk(chosenId,taskName,freeRob.(k));
            }
            //else logInstall('[SCHD] No task for rob %i (%s)',freeRob.(k),g->scheduler);
        }
    }
    else logInstall('[SCHD] No rob available');

}


/*
    Add a mission to the scheduler DB
    @param v [varspc] -> mission definition varspc
    @return key [long] -> unique DB key of the mission
*/
export function key = simSchdTaskAdd(v) {
    global g('simConf');

    logDebug("[SCHD] init of simSchdTaskAdd");

    // Initialization
    vv = varspc();
    vv = v;
    missions = g->missions;
    found = 0;
    missId = 0;

    // Check that required fields are set
    if (!isvar(v,'taskName')) {
        logWarn('[SCHD] Field taskName is not set, skipping...');
        return;
    }
    // Check if mission is defined in global varspace //TODO DOESNT WORK IF A MISSION IS DISABLED !
    id = for(missions) {
        if (!strcmp(missions.{id}->title,char(v->taskName))) {
            found = 1;
            missId = id;
        }
    }
     
    if (!found) {
        logWarn('[SCHD] TaskName %z doesn t exists, skipping...',v->taskName);
        return;
    }
    // Set missId
    if (v->missId!=missId) {
        logWarn('[SCHD] Miss id (%i), is different from global varspc (%i) ==> take %i',v->missId,missId,missId);
        vv->missId = missId;
    }

    // Set taskName
    vv->taskName = strn(char(v->taskName));

    // Set robAuth //choose rob group from .c function
    if(isempty(missions.{missId}->group)) vv->robAuth = {robFindAll()};
    else vv->robAuth = {missions.{missId}->group};

    // Set addDate
    vv->addDate = simGetT();

    // Add task in simSchdDB
    key = DBadd_gbl('simSchdDB',vv);
    logDebug('[SCHD] Task %i (%s) added in simSchdDB',key,char(v->taskName));
}


/*
    Function that selects the closest (in distance) mission from a id list
    @param idrob [long] -> id of the robot
    @param  ids [long_cell] -> list of ids (simSchdDB)
    @return id [long] -> Id of the selected task
*/
export function id = simSchdFindTaskClosest(idrob,ids) {
    global g('simConf');
    // logInstall("The missions IDs to choose from are: %z", ids);

    // ids is the variable with the csv id

    // Initialization
    id = new('long',0);
    taskName = new('char',0);
    missId = new('long',0);
    missPgm = new('char',0);

    r = varspc();
    r->rpk = new('double',0);
    r->id  = new('long',0);

    unint missions = g->missions;

    // Get rob position
    robPos = robGetZ(idrob);
    serial = number2Serial(idrob);
    robModel = getRobModel(idrob);
    flag = getPreferenceValue("navigation/pathFlag",serial);

    //iterate for all ids
    // logInstall("[sim_schd] size of ids %z", size(ids));
    i = for(0,size(ids)-1) {

        // Get data of the mission
        v = DBget_gbl('simSchdDB',ids.(i),strn('id','missId'));

        // Check if mission battery
        if(v->missId == -2) { //-2 is reserved for battery missions !
            id = ids.(i); //thus take this as next mission
            return;
        }
        else if(v->missId == -1) displayError(sprintf('[SCHD] Task %i do not have a missId defined (-1) !',id.(i-1)));

        // else mission is stored in missions varspc
        task = missions.{v->missId};
        // Get the pos of the first objective destination

        // <-------------- NOT NECESARY FOR THE CSV PART --------->
        // if(isvar(task->objectives.{0},'grp')) {
        //     if(istype(task->objectives.{0}->grp,'obj'))
        //     {
        //         grpName = task->objectives.{0}->grp.{0};
        //     }
            
        //   else
        //   {
        //     grpName = task->objectives.{0}->grp;
        //   // get dst (random from grp)
        //   logConsoleLevel(4); // to avoid spam (ugly i know flemme de recoder une fonction)
        //   dst = chooseRealDestination(grpName,'random', task);
        //   logConsoleLevel(g->logConsoleLevel);
        //   }
        // }
        // else if(isvar(task->objectives.{0},'dst')) {
        //     dst = task->objectives.{0}->dst;
        // }

        // Dst is the src point for that mission taken from the csv file
        

        dst = DBget_gbl('simSchdDB',ids.(i),'srcPoint');


        // calc rpk
        if(!isNumeric(dst)) pointData = missPntGetInfo(name2id(dst));
        else pointData = missPntGetInfo(dst);
        zendCpx = cpx(pointData->zend.xposition,pointData->zend.yposition);
        [arg1,arg2,arg3,arg4,arg5] = TRACK_FindItinerary(robModel,robPos,zendCpx,flag);
        rpk = arg4;

        if (isnan(rpk)) continue;

        //store rpk found
        r->id.(end+1) = ids.(i);
        r->rpk.(end+1) = rpk;
    }

    //get the closest rpk
    [rpkMin,rpkId] = min(r->rpk);

    // Assign results
    if(isempty(rpkId)) id = ids.(0);
    else id = r->id.(rpkId.(0)); //chose first minimal rpk

}


/*
    function to get all mission authorized for a rob at a particular state (all,waiting,sending,running,done);
    @param idrob [long] -> id of the robot
    @param state [string] -> state of the wanted tasks
    @return ids [long_cell] -> Ids of the tasks found
*/
export function ids = simSchdFindTaskAuth(idrob,state) {
    global g('simConf');

    if(nargin<2) state = 'all';
    ids = new('long',0);

    // Get all tasks according to state
    taskIds = simSchdGetTask(state);

    if(!isempty(taskIds)) {
        // Retrieve corresponding data
        v = DBget_gbl('simSchdDB',taskIds,strn('id','robAuth'));
        i = for(0,size(taskIds)-1) {
            //find if idrob is authorized for each task
            if(!isempty(findElt(v->robAuth.(i).{0},idrob))) ids.(end+1) = taskIds.(i);
        }
    }
    //    else logDebug('[simSchdFindTasksForRob] No %s tasks for rob %i',state,idrob);
}


/*
    function to get all highest priority missions from an id list
    @param ids [long_cell] -> list of ids (simSchdDB)
    @return prioIds [long_cell] -> Ids of the tasks found
*/
export function maxPrioIds = simSchdFindTaskPrio(ids) {
    global g('simConf');

    // Initialization
    prioLs = new('long',0);
    prioIds = new('long',0);
    maxPrioIds = new('long',0);

    // Retrieve corresponding data
    v = DBget_gbl('simSchdDB',ids,strn('id','priority'));

    if(!isempty(ids)) {
        // Take only missions with greater or equal priority
        i = for(ids) {
            prioLs.(end+1) = v->priority.(i);
            prioIds.(end+1) = ids.(i);
        }
    }
    else logError('[simSchdFindTaskPrio] Argument "ids" is empty !');

    //get max priority
    [maxPrio,maxIds] = max(prioLs);
    maxPrioIds = prioIds.(maxIds);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                         SCHEDULER RELATED CALLBACKS                         //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Called at the creation of a task
    @param taskId [long] -> simSchdDB key of the task
    @param miss [char] -> task name
    @param idrob [long] -> id of the robot
*/
export function taskCreateCbk(taskId, taskName, idrob) {
    if(nargin<3) idrob = Null();

    //if idrob is empty it means the schd is in FIFO (we don't know which robot will take the mission)
    if(isempty(idrob)) {
        DBset_gbl('simSchdDB',taskId,varspc('state',1));
    }
    else {
        // Set the rob reserved for the task
        DBset_gbl('simSchdDB',taskId,varspc('rob',idrob,'state',1));
    }
}


/*
    Called at the start of a task
    @param taskId [long] -> simSchdDB key of the task
    @param miss [char] -> task name
    @param idrob [long] -> id of the robot
*/
export function taskStartCbk(taskId, taskName, idrob) {

    // Set task as running
    DBset_gbl('simSchdDB',taskId,varspc('state',2,'startDate',simGetT()));


    //EUSKIRCHEN DEBUG
    //check the existence of other "sending" task
    ids_sending = setInter(DBfind_gbl('simSchdDB','state','==',1), DBfind_gbl('simSchdDB','rob','==',idrob));
    if(!isempty(ids_sending)) {
      data_sending = DBget_gbl('simSchdDB',ids_sending);
      i = for(ids_sending) {
        taskName = char(data_sending->taskName.(i));
        DBset_gbl('simSchdDB',ids_sending.(i),varspc('state',0)); // set sending task as waiting (back in the pool)
        // logWarn("[taskStartCbk] rob %i has a sending task : relaunch id %i (state = 0)",idrob,ids_sending.(i));
      }
    }
}


/*
    Called at the end of a task
    @param taskId [long] -> simSchdDB key of the task
    @param miss [char] -> task name
    @param idrob [long] -> id of the robot
*/
export function taskDoneCbk(taskId, taskName, idrob) {
    global g('simConf');

    // Set task as done
    DBset_gbl('simSchdDB',taskId,varspc('state',3,'endDate',simGetT()));

    //check if simulation has to stop
    isSimEndTime();

    //check if there is available missions and rob => trigger them
    isLaunched = simSchdLaunchTask(idrob);

    //launch rob to taxi if no mission found
    if(!isLaunched) rob_taxi();
}


/*
    Choose_cbk to chose only selected robot
    @param lrob [L.char?] -> list of all robs (called by SDK)
    @param idstr [char] -> char id of the wanted rob
    @return id [long] -> id of the wanted rob
*/
export function id = iRobot(lrob,idstr) {
    id = [str2num(idstr)];
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                          SCHEDULER USEFUL FUNCTIONS                         //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Returns all the points with the group name "pointname_xxx" (if they exists), otherwise returns itself
    @param grpName [char] -> a group of point
    @return pntLs [char_cell] -> the list of all the points in the group
*/
export function pntLs = getGroupPnt(grpName) {
    pntLs = {};

    /*allPnt = cirFindAllPnt();
    p = for(allPnt) {
        pntName = cirGetPntName(p);
        //find the last underscore idx
        underIdx = strfind(pntName,'_');
        if(!isempty(underIdx)) {
            // compare the pnt names
            newName = pntName.(lin(0,(underIdx.(end)-1)));
            // if same : add to the list
            if(!strcmp(newName,grpName)) pntLs.{end+1} = pntName;
        }
    }*/
    //if no points found --> itself
    if(isempty(pntLs)) pntLs = {grpName};
}


/*
  Returns the group name of a specific point (pointname_xxx) or an empty string
  @param pntName [char] -> a point name
  @return grpName [char] -> a group name
*/
export function grpName = getGroupName(pntName) {
    grpName = new('char',0);

    //find the last underscore idx
    underIdx = strfind(pntName,'_');
    if(!isempty(underIdx)) grpName = pntName.(lin(0,(underIdx.(end)-1)));
    else logWarn('[getGroupName] %s doesnt have the "pointname_xxx" structure. GroupName is an empty string',pntName);
}


/*
    Function to get all available rob
    @return idrob [L.long] -> Array of available rob
*/
export function idrob = simGetAvailableRob() {
    global g('simConf');

    // Initialization
    idrob = new('long',0);

    // Find all robots
    unint robs = robFindAll();

    // Add available rob to the list
    k = for(robs) {
        robTask = robFindTask(robs.(k));
        if (!strcmp(robTask,'noTask'))
            idrob.(end+1) = robs.(k);
    }
}


/*
    Function to query simSchdDB to get all tasks with a defined state
    @param  state [char] -> State of the tasks to get: waiting | running | done
    @return ids [L.long] -> Array of DB keys matching the state
*/
export function ids = simSchdGetTask(state) {
    // Initialization
    ids = new('long',0);

    // Return query from DB
    switch(tolower(state)) {
        case('waiting');
            ids = DBfind_gbl('simSchdDB','state','==',0);
        case('sending');
            ids = DBfind_gbl('simSchdDB','state','==',1);
        case('running');
            ids = DBfind_gbl('simSchdDB','state','==',2);
        case('done');
            ids = DBfind_gbl('simSchdDB','state','==',3);
        case('all');
            ids = DBgetAllKeys_gbl('simSchdDB');
        default;
            logWarn('[SCHD] Task state %z doesn t exists, skipping...',state);
            return;
    }
}


/*
     Export tasks for scheduler windows
*/
export function tasks = simSchdGetTasks()
{
    tasks = [];
    nTask = 0;

    // print running tasks
    ids = simSchdGetTask('running');
    taskData = DBget_gbl('simSchdDB',ids);

    k = for(taskData->id)
    {
        if (nTask > 15)
        {
            break;
        }
        tasks = [tasks, '    ', num2str(taskData->id.(k)), '|',taskData->taskName.(k),'|running|',sprintHMS(taskData->startDate.(k) - taskData->addDate.(k)),'|',sprintHMS(simGetT - taskData->startDate.(k)),'|',num2str(taskData->rob.(k)),'\n'];
        nTask = nTask + 1;
    }

    tasks = [tasks,'\n'];

    // print waiting tasks
    ids = simSchdGetTask('waiting');
    taskData = DBget_gbl('simSchdDB',ids);

    if (nTask < 15)
    {
        k = for(taskData->id)
        {
            if (nTask > 15)
            {
                break;
            }
            tasks = [tasks, '    ', num2str(taskData->id.(k)), '|',taskData->taskName.(k),'|waiting|',sprintHMS(simGetT - taskData->addDate.(k)),'|00:00:00\n'];
            nTask = nTask + 1;
        }
    }

    tasks = [tasks,'\n'];

    // print sending tasks
    ids = simSchdGetTask('sending');
    taskData = DBget_gbl('simSchdDB',ids);

    if (nTask < 15)
    {
        k = for(taskData->id)
        {
            if (nTask > 15)
            {
                break;
            }
            tasks = [tasks, '    ', num2str(taskData->id.(k)), '|',taskData->taskName.(k),'|sending|',sprintHMS(simGetT - taskData->addDate.(k)),'|00:00:00\n'];
            nTask = nTask + 1;
        }
    }

    tasks = [' Mission Waiting: ',num2str(size(ids)),'\n\n',tasks];
}


/*
    Function to return useful data of a unique task
    @param taskId [long] -> key of the task (simSchdDB)
    @return taskName [strn] -> name of the task
    @return missId [long] -> id of the mission (from g->missions)
    @return missPgm [strn] -> name of the program associated
    @return robAuth [long] -> robot associated
*/
export function [taskName, missId, missPgm, robAuth] = simSchdGetTaskData(taskId) {
    global g('simConf');

    if(size(taskId)!=1) displayError(sprintf('[simSchdGetTaskData] Size of taskId is superior to 1 ! (%l)',size(taskId)));

    // Retrieve corresponding data
    v = DBget_gbl('simSchdDB',taskId,strn('id','taskName','missId','pgm','robAuth'));
    taskName = v->taskName;
    missId = v->missId;
    missPgm = v->pgm;
    robAuth = v->robAuth.{0};

}
