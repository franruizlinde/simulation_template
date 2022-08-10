/*
    fill the DB from CSV data
*/
export function initCSV() {

    id = kiwiGetGlobalId();
    global w(id);

    csvFileName = [fnameName(w->env->open),'.csv']; // name of the csv file (I suppose one file per circuit)
    csvFilePath = fnameConc(w->env->proj,'Circuits',csvFileName); // location of the csv file (YOURPROJECT/Circuit)
    // read the content of the CSV file
    if(!isfile(csvFilePath)) return;
    csvContent  = readCSV(csvFilePath,0);
    // display warn if file empty
    if(size(csvContent) < 2) {
      logWarn("[MISS-CSV] Init : Csv file is empty!");
      return;
    }
    // create mission generation DB
    createcsvDB();
    // loop over each line and store cellules values: Day | Hour | Throughput | Mission //TODO: add checks

    k=for(1,size(csvContent)-1) {

      lineContent   = csvContent.{k};

      if(1 == size(lineContent)) {  // one string is detected and has to be splitted
        lineSplit       = strSplit(lineContent.{0},','); //split line with comma
        csvDay          = lineSplit.{0}; //get the value of the first column for this row --> Day
        csvHour         = lineSplit.{1}; //get the value of the second column for this row and keep only 2 first digits -->Hour
        if(2 < size(csvHour)) csvHour = csvHour.(lin(0,1));
        csvMinute       = lineSplit.{2}; //get the value of the third column for this row --> Throughput
        csvSecond       = lineSplit.{3}; //get the value of the fourth column for this row --> Mission name
        csvMission      = lineSplit.{4}; //get the value of the sixth column for this row --> Mission name
        if(isempty(lineContent.{5})){ csvSrcPoint = '0';}
        else{csvSrcPoint = lineContent.{5};} //get the value of the seventh column for this row --> source Point}
        
        if(isempty(lineContent.{6})){csvDstPoint = '0';}
        else {csvDstPoint = lineContent.{6}; //get the value of the eighth column for this row --> destination Point}
        }
      }

      else if(6 <= size(lineContent)) { // The line detected is already splitted
        csvDay            = lineContent.{0}; //get the value of the first column for this row --> Day
        csvHour           = lineContent.{1}; //get the value of the second column for this row and keep only 2 first digits -->Hour
        csvMinute         = lineContent.{2}; //get the value of the third column for this row --> Day
        csvSecond         = lineContent.{3}; //get the value of the fourth column for this row and keep only 2 first digits -->Hour
        csvMission        = lineContent.{4}; //get the value of the sixth column for this row --> Mission name

        if(isempty(lineContent.{5})){ csvSrcPoint ='0';}
        else{csvSrcPoint = lineContent.{5};} //get the value of the seventh column for this row --> source Point}
        
        if(isempty(lineContent.{6})){csvDstPoint ='0';}
        else {csvDstPoint = lineContent.{6}; //get the value of the eighth column for this row --> destination Point}
        }
      }

      else logError("[MISS-CSV] Error in content! r=%z c=%z)",lineContent,csvContent);
      //creates varspc
      varspcDB = varspc();

      // fill varspc (id will be filled automatically)
      varspcDB->day         = str2num(csvDay);
      // varspcDB->throughput  = str2num(csvThroughput);
      varspcDB->hour        = str2num(csvHour);
      varspcDB->mission     = strn(csvMission);
      varspcDB->minute      = str2num(csvMinute);
      varspcDB->second      = str2num(csvSecond);
      varspcDB->srcPoint     = csvSrcPoint;
      varspcDB->dstPoint     = csvDstPoint;

      // If the obtained values are number, they are changed into strings
      // if(isNumeric(csvSrcPoint)){varspcDB->srcPoint     = num2str(csvSrcPoint);}
      // if(isNumeric(csvDstPoint)){varspcDB->dstPoint     = num2str(csvDstPoint);}

      // add entry to DB
      DBadd_gbl("csvDB",varspcDB);
    }
    
    logInstall("[initCSV] csvDB created and filled. end of the function");

    // Updates the ratePerHour from the Csv file by counting the missions to execute
}

// Function to change the rate per hour if there is data in the csv file. It is called from thesim_main file, just after the initCSV function
export function ratePerHourFromCSV()
{
  global g('simConf');
  // unint missions = g->missions;
  unint missions = g->mission;
  loopSize = size(missions);
  sh = g->StartHour + (g->startCollectData /3600);
  eh = g->StartHour + (g->simulationRunTime / 3600);

  // it there is data in the csvData base
  if(!isempty(DBget_gbl("csvDB")))
  {
    k = for(g->mission)
    {
      ids_missions = DBfind_gbl("csvDB", "mission", "==", g->mission.{k}->title);
      ids_day = DBfind_gbl("csvDB","day",">=",(g->StartDay));
      ids_hour = setInter( DBfind_gbl("csvDB","hour",">=",sh), DBfind_gbl("csvDB","hour","<=",eh) );
      ids_time = setInter(ids_day, ids_hour);
      ids = setInter(ids_missions, ids_time);
      
      // logDebug("%z executed %z times.", g->mission.{k}->title, size(ids));
      g->mission.{k}->ratePerHour = (size(ids) *1.0)/((g->simulationRunTime)/3600);
      // logDebug("new ratePerHour: %z", g->mission.{k}->ratePerHour);
    }
  }
}

// create mission generation DB
export function createcsvDB() {
    logInfo('Creating mission generation DB');

    // Get DB definition
    dbDef = csvDBDef();

    // Create DB from definition
    DBcreate_gbl('csvDB',dbDef,'id',0);

    // If keys, delete them all to re-start fresh
    keys = DBgetAllKeys_gbl('csvDB');
    if (size(keys)) {
        //logDebug('[SCHD] Deleting old keys %z',keys);
        DBdel_gbl('csvDB',keys);
    }
}

// definition varspace of the mission generation DB
export function varspcDB = csvDBDef() {
    //creates definition varspc
    varspcDB = varspc(  "id",          0, //Is a number
                        "mission",     strn(""), //Is a string
                        "day",         0, //Is a number
                        "minute",     0, // Is a number
                        "second",      0, // Is a number
                        "throughput",  0, //Is a number
                        "hour"   ,     0, //Is a number
                        "currentNb",   0, // track number of missions
                        "srcPoint",   strn(""), // Is a number
                        "dstPoint",   strn("") // Is a number
                        );
}


/*
    get ids from Database of a specific mission, day, hour, minute, second
    @param mission [char] -> mission name (optional)
    @param day     [long] -> day number (optional)
    @param hour    [long] -> hour number (optional)
    @param day     [long] -> day number (optional)
    @param second    [long] -> second number (optional)
    @return ids   [L.long] -> list of ids

*/
export function ids = csvDB_getEntryId(mission, day, hour, minute, second) {

    // if no argument = full data
  	if(0 == nargin) {
  		ids = DBgetAllKeys_gbl("csvDB"); //get all data if number of arguments (nargin) is 0
  	}
    // data from specific mission
  	else {
  		ids = DBfind_gbl("csvDB","mission","==",mission); // filter ids with mission name

      // filter ids with day number (optional)
      if(nargin > 1) {
        ids = setInter(ids, DBfind_gbl("csvDB","day","==",day));
        // logInstall("nargins > 1, the ids are: %z", ids);
      }

      // filter ids with hour number (optional)
      if(nargin > 2) {
        ids = setInter(ids,DBfind_gbl("csvDB","hour","==",hour));
        // logInstall("nargins > 2, the ids are: %z", ids);
        // if(size(ids) > 1) {
        //   displayError(sprintf("[csvDB] Error in DB, id should be unique (s=%i)",size(ids)));
        // }
      }

      // filter ids with minutes number (optional)
      if(nargin > 3) {
        ids = setInter(ids,DBfind_gbl("csvDB","minute","==", minute));
        // logInstall("nargins > 3, the ids are: %z", ids);
        // if(size(ids) > 1) {
        //   displayError(sprintf("[csvDB] Error in DB, id should be unique (s=%i)",size(ids)));
        // }
      }

      // filter ids with seconds number (optional)
      if(nargin > 4) {
        ids = setInter(ids,DBfind_gbl("csvDB","second","==", second));
        // logInstall("nargins > 4, the ids are: %z", ids);
        // if(size(ids) > 1) {
        //   displayError(sprintf("[csvDB] Error in DB, id should be unique (s=%i)",size(ids)));
        // }
      }
          if(size(ids) > 1) {
            // This means there are 2 same missions at the same time in the csv file
            logWarn("There are %z missions of %z, at the same moment. Both of them are launched.",size(ids), mission );
          // displayError(sprintf("[csvDB] Error in DB, id should be unique (s=%i)",size(ids)));
          // logInstall("The ids are: [%z], there should be just one", ids);
        }
  	}
    // return error if no entry found
    if(isempty(ids)) {
      //displayError(sprintf("[csvDB] No entry found for miss %s",mission));
      // logWarn("[csvDB_getEntryId] Mission: %z not founded at time (d%z,h%z,m%z,s%z)", mission, day, hour, minute, second);
    }
}
/*
    CBK launched every second to enable custom mission profile (DEV)
    @param taskName [char] -> name of the mission
*/
export function customMissProfileCbk(taskName) {
    global g('simConf');
    global csv("csvVar");

    idMiss = new('long',0); //Create a new list of type type that contains n elements initialized at 0 and preallocates nmax elements.
    unint missions = g->mission; //Get all missions data from configuration file

    // find the id of the mission from the mission list /*-- I think it is from the .c file where the missions and flows are defined*/
    i = for(missions) {
        if(!strcmp(missions.{i}->title, taskName)) {
            idMiss = i;
            // logInstall("IDMISS ASIGNADAS: %z a %z",idMiss, taskName);
            break;
        }
    }
    
    // return if mission not found
    if(isempty(idMiss)) {
      displayError(sprintf("[customMissProfile] Mission %s not found in config file.",taskName));
      return;
    }

    //get current time
      current_time          = simGetT() + g->StartTime; //simGetT: Time of simulation since beginning (in seconds) addind the offset time set in config
      current_minute        = long(current_time/60);  // minute number of the specific day
      current_day           = long(current_time/(3600*24)); // day number (starting from 0)
      current_hour2         = long((current_time - (current_day*3600*24))/3600); // hour number of the specific day (starting from 0)
      // current_hour    =   long(current_time/10); // hour number of the specific day (starting from 0) 
      current_seconds       = long(current_time - (current_day*3600*24) - (current_hour2*3600)); // number of seconds of the specific hour
      current_minutes       = long(current_time/60 - (current_day*60*24) - (current_hour2*60)); // number of minutes of the specific hour
      current_seconds_min   = long(current_time - (current_day*3600*24) - (current_hour2*3600) - current_minutes*60); // number of seconds of the specific minute

/*     // Get DB ids of this day
    ids = csvDB_getEntryId(taskName,(current_day + 1));

    // get throughput of the day from DB
    day_throughput = DBget_gbl("csvDB",ids,strn("throughput"))->throughput; */

    // compute current throughput
    //current_hour_modulo = current_hour%size(day_throughput); // in case there is less hours than 24	
    id = csvDB_getEntryId(taskName,(current_day),current_hour2, current_minutes, current_seconds_min); // get DB id of this second

    // number of miss already launched this hour
    day_nbmissions = DBget_gbl("csvDB",id,strn("currentNb"))->currentNb;

    if(!isempty(id)) {  // If an id is assigned

    // There is a possibility of having the same mission at the same time more than once
    if(size(id) > 1)
    {
      k = for(0, (size(id)-1)){
        
        // number of miss already launched this hour
        day_nbmissions = DBget_gbl("csvDB",id.(k),strn("currentNb"))->currentNb;
        // add the src and dst points to the schedDB
        vv = varspc();
        vv->srcPoint = DBget_gbl("csvDB",id.(k),strn("srcPoint"))->srcPoint;
        vv->dstPoint = DBget_gbl("csvDB",id.(k),strn("dstPoint"))->dstPoint;
        vv->idCSV = id.(k);
        // key = DBadd_gbl("simSchdDB", vv);
        logDebug("Src Point: %z and dst Point %z added to the simSchdDB database",vv->srcPoint, vv->dstPoint );

        launchMiss(taskName, vv);

        //update number of miss already launched this hour
        DBset_gbl("csvDB",id.(k),varspc("currentNb",(day_nbmissions + 1)));
      }

    }

    else{
      // add the src and dst points to the schedDB
      vv = varspc();
      vv->srcPoint = DBget_gbl("csvDB",id,strn("srcPoint"))->srcPoint;
      vv->dstPoint = DBget_gbl("csvDB",id,strn("dstPoint"))->dstPoint;
      vv->idCSV = id;
      // key = DBadd_gbl("simSchdDB", vv);
      logDebug("[mission_csv] Src Point: %z and dst Point %z added to the simSchdDB database",vv->srcPoint, vv->dstPoint );

      launchMiss(taskName, vv);

      //update number of miss already launched this hour
      DBset_gbl("csvDB",id,varspc("currentNb",(day_nbmissions + 1)));
    }
    

  }
      // else logWarn("No mission assigned to this second");
      // checks is the simulation time is reached
      isSimEndTime();

}