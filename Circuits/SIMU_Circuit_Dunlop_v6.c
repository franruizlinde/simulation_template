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
    conf->scheduler = 'FIFO';

    // startCollectData represent the simulation time (in hours) the agvs will be running before collecting cadences data
    // This is done to let the agv fleet spread around the facility
    conf->startCollectData = 1;

    // simulationRunTime represent the simulation time (in hours) for which who are collecting cadences data
    // As soon as we have all data for this period, the simulation will stop automatically (up to 48h)
    conf->simulationRunTime = 8;

    // Definition of pallets used in the differents flows : {palName, cpx(LENGHT,WIDTH), color}
    // Color has to be one of the following: BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, YELLOW, WHITE
    conf->pallets = {
        {'EUR',cpx(1.5,1.115),'YELLOW'},
    };

    // Multiplying factor that will be applied to 'ratePerHour' for all of the missions
    conf->factor = 1.0;

    // Task List to be executed by the agvs
    conf->missions = {}; // do no touch this :)

    // start the simulation in a diferent day, hour, minute, second. THIS CAN BE MODIFIED AS NEEDED.
    conf->StartDay = 0;
    conf->StartHour = 12;
    conf->StartMinute = 0;
    conf->StartSecond = 0;

    conf->StartTime = conf->StartSecond + conf->StartMinute*60 + conf->StartHour*3600 + conf->StartDay*(24*3600); // Starting time in seconds. This value is added to the current time variable in mission_csv.cpp

    // * * * Insert your missions below : * * * * * 

/*     //Rack Y3 -> DLR Y3
    firstRack1 = 1;
    lastRack1 = 12;   
    rate1 = 0; //8.800;

    numOfRack1 = lastRack1 - firstRack1 + 1;
    myRate1 = rate1 / numOfRack1,
    rack1 = for(firstRack1,lastRack1)
    {

 	conf->missions.{end+1} = varspc(
		'title',sprintf('Rack%z_to_DLR_Y3',rack1),
		'enable',1,
		'ratePerHour',myRate1,
		'group',[],
		'missAuto',1,
		'objectives', {

            varspc('task', 'pick', 'grp', sprintf('rack%z',rack1),  'nbPal',1, 'destinationTime',15),
            varspc('task', 'drop', 'grp', "DLR_Y3", 'nbPal',1, 'destinationTime',15),

		},
		'flows', {
			varspc('flowName',sprintf('Rack%z_to_DLR_Y3',rack1),'numberOfStep',2,'flowDisplay',1),
		}, 'trigger' , 'csv'
    ); 
    }

    //DLRY3->rack 1 - 12
    firstRack2 = 1;
    lastRack2 = 12;
    rate2 = 0; //10.1800;

    numOfRack2 = lastRack2 - firstRack2 + 1;
    myRate2 = rate2 / numOfRack2,
    rack2 = for(firstRack2,lastRack2)
    {

 	conf->missions.{end+1} = varspc(
		'title',sprintf('DLR_Y3_to_Rack%z',rack2),
		'enable',1,
		'ratePerHour',myRate2,
		'group',[],
		'missAuto',1,
		'objectives', {

            varspc('task', 'pick', 'grp', "DLR_Y3", 'nbPal',1, 'destinationTime',15),
            varspc('task', 'drop', 'grp', sprintf('rack%z',rack2), 'nbPal',1, 'destinationTime',15),

		},
		'flows', {
			varspc('flowName',sprintf('DLR_Y3_to_Rack%z',rack2),'numberOfStep',2,'flowDisplay',1),
		}, 'trigger' , 'csv'
    ); 
    }
    
  //DLR_Y2 -> BSP
     	conf->missions.{end+1} = varspc(
		'title','DLR_Y2_to_BSP_Y3',
		'enable',1,
		'ratePerHour',0,
		'group',[],
		'missAuto',1,
		'objectives', {

            varspc('task', 'pick', 'grp', 'DLR_Y2', 'nbPal',1, 'destinationTime',15),
            varspc('task', 'drop', 'grp', 'BSP_Y3', 'nbPal',1, 'destinationTime',15),

		},
		'flows', {
			varspc('flowName','DLR_Y2_to_BSP_Y3','numberOfStep',2,'flowDisplay',1),
		}, 'trigger' , 'csv'
    ); 

  //DLR_Y1 -> BSP
//      	conf->missions.{end+1} = varspc(
// 		'title','DLR_Y1_to_BSP_Y3',
// 		'enable',1,
// 		'ratePerHour',0.650,
// 		'group',[],
// 		'missAuto',1,
// 		'objectives', {

//             varspc('task', 'pick', 'grp', 'DLR_Y1', 'nbPal',1, 'destinationTime',15),
//             varspc('task', 'drop', 'grp', 'BSP_Y3', 'nbPal',1, 'destinationTime',15),

// 		},
// 		'flows', {
// 			varspc('flowName','DLR_Y1_to_BSP_Y3','numberOfStep',2,'flowDisplay',1),
// 		}
//     ); 

   //rack 16-20 -> ubergabey2 0.9pal/h
//     firstRack3 = 16;
//     lastRack3 = 20;
//     rate3 = 0.47;

//     numOfRack3 = lastRack3 - firstRack3 + 1;
//     myRate3 = rate3 / numOfRack3,
//     rack3 = for(firstRack3,lastRack3)
//     {

//  	conf->missions.{end+1} = varspc(
// 		'title',sprintf('Rack%z_to_BSP_Y3',rack3),
// 		'enable',1,
// 		'ratePerHour',myRate3,
// 		'group',[],
// 		'missAuto',1,
// 		'objectives', {

//             varspc('task', 'pick', 'grp', sprintf('rack%z',rack3), 'nbPal',1, 'destinationTime',15),
//             varspc('task', 'drop', 'grp', 'BSP_Y3',            'nbPal',1, 'destinationTime',15),

// 		},
// 		'flows', {
// 			varspc('flowName',sprintf('Rack%z_to_BSP_Y3',rack3),'numberOfStep',2,'flowDisplay',1),
// 		}
//     ); 
//     }

    //ubergabey2 -> rack 16-15 0.9pal/h
    firstRack4 = 16;
    lastRack4 = 20;
    rate4 = 0; //0.47;

    numOfRack4 = lastRack4 - firstRack4 + 1;
    myRate4 = rate4 / numOfRack4,
    rack4 = for(firstRack4,lastRack4)
    {

 	conf->missions.{end+1} = varspc(
		'title',sprintf('WE_Y1_to_Rack%z',rack4),
		'enable',1,
		'ratePerHour',myRate4,
		'group',[],
		'missAuto',1,
		'objectives', {

            varspc('task', 'pick', 'grp', 'WE_Y1',            'nbPal',1, 'destinationTime',15),
            varspc('task', 'drop', 'grp', sprintf('rack%z',rack4), 'nbPal',1, 'destinationTime',15),

		},
		'flows', {
			varspc('flowName',sprintf('WE_Y1_to_Rack%z',rack4),'numberOfStep',2,'flowDisplay',1),
		}, 'trigger' , 'csv'
    ); 
    }
       */

      conf->missions.{end+1} = varspc(
		'title','transport',
		'enable',1,
		'ratePerHour',0,
		'group',[],
		'missAuto',1,
		'objectives', {

            varspc('task', 'pick', 'grp', '29874'),
            varspc('task', 'drop', 'grp', '29874'),

		},
		'flows', {
			varspc('flowName','transport','numberOfStep',2,'flowDisplay',1),
		}, 'trigger' , 'csv'
    ); 
    





}






