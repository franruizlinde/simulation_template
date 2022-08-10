/* SIMULATION */

/*****************************************/
/** Balyo S.A. / Balyo Inc              **/
/** Project: base code for Simulation   **/
/** Last modification: 2020/06/22       **/
/*****************************************/

export function conf = customConfig() {
    // Create config varspc
    conf = varspc();

    // Format for CSV files export
    // values : 'EU', 'US'
    conf->format = 'EU';

    // Number of agvs to use in the Simulation
    // Agvs will always be picked in ordrer following the install.xml preference file
    conf->nRob = 2;

    //enable or disable battery missions (automatically launched) 1: enabled , 0 : no battery mission
    conf->enableBattMiss = 1;

    // used in rob_battery
    // collective : the battery points are shared between all the robots
    // individual : one battery point per robot, named battx for rob x
    conf->battPntStrategy = 'individual';

    // scheduler strategy
    // "closest" - optimze missions so rob take closest mission
    // "FIFO" - robot take first mission
    // "LIFO" - robot take last available mission
    conf->scheduler = 'closest';

    // startCollectData represent the simulation time (in hours) the agvs will be running before collecting cadences data
    // This is done to let the agv fleet spread around the facility
    conf->startCollectData = 1;

    // simulationRunTime represent the simulation time (in hours) for which who are collecting cadences data
    // As soon as we have all data for this period, the simulation will stop automatically (up to 48h)
    conf->simulationRunTime = 8;

    // Definition of pallets used in the differents flows : {palName, cpx(LENGHT,WIDTH), color}
    // Color has to be one of the following: BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, YELLOW, WHITE
    conf->pallets = {
        {'EUR',cpx(1.2,0.8),'YELLOW'},
    };

	conf->StartDay = 0;
	conf->StartHour = 0;
	conf->StartMinute = 0;
	conf->StartSecond = 0;

	conf->StartTime = conf->StartSecond + conf->StartMinute*60 + conf->StartHour*3600 + conf->StartDay*(24*3600); // Starting time in seconds. This value is added to the current time variable in mission_csv.cpp

    // Multiplying factor that will be applied to 'ratePerHour' for all of the missions
    conf->factor = 1.0;

    // Task List to be executed by the agvs
    conf->missions = {}; // do no touch this :)

    // * * * Insert your missions below : * * * * * 

conf->missions.{end+1} = varspc(
		'title','Mission_1',
		'enable',1,
		'ratePerHour',1,
		'group',[1],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '135'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow1','numberOfStep',2,'flowDisplay',1),
		}
  
    );
    
    
    	conf->missions.{end+1} = varspc(
		'title','Mission_2',
		'enable',1,
		'ratePerHour',1,
		'group',[2],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '136'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow2','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_3',
		'enable',1,
		'ratePerHour',1,
		'group',[1],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '107'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow3','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_4',
		'enable',1,
		'ratePerHour',1,
		'group',[2],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '111'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow4','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_5',
		'enable',1,
		'ratePerHour',3,
		'group',[1],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '175'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'grp', 'stock2'),
		},
		'flows', {
			varspc('flowName','flow5','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_6',
		'enable',1,
		'ratePerHour',3,
		'group',[2],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '175'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'grp', 'stock1'),
		},
		'flows', {
			varspc('flowName','flow6','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_7',
		'enable',1,
		'ratePerHour',1,
		'group',[1],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '127'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow7','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_8',
		'enable',1,
		'ratePerHour',1,
		'group',[2],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '126'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow8','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_9',
		'enable',1,
		'ratePerHour',1,
		'group',[1],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '145'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow9','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_10',
		'enable',1,
		'ratePerHour',1,
		'group',[2],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '144'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow10','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_11',
		'enable',1,
		'ratePerHour',1,
		'group',[1],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '163'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow11','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_12',
		'enable',1,
		'ratePerHour',1,
		'group',[2],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '162'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow12','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
  conf->missions.{end+1} = varspc(
		'title','Mission_13',
		'enable',1,
		'ratePerHour',1,
		'group',[1],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '153'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow13','numberOfStep',2,'flowDisplay',1),
		}
  
	);
  conf->missions.{end+1} = varspc(
		'title','Mission_14',
		'enable',1,
		'ratePerHour',1,
		'group',[2],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '154'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'dst', '170'),
		},
		'flows', {
			varspc('flowName','flow14','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
	conf->missions.{end+1} = varspc(
		'title','Mission_15',
		'enable',1,
		'ratePerHour',6,
		'group',[1],
		'missAuto',1,
		'objectives', {
			//varspc('task', 'pick', 'dst', '200'),
            varspc('task', 'pick', 'dst', '175'),
			//varspc('task', 'drop', 'dst', '184'),
            varspc('task', 'drop', 'grp', 'rack'),
		},
		'flows', {
			varspc('flowName','flow15','numberOfStep',2,'flowDisplay',1),
		}
  
	);
	
}



