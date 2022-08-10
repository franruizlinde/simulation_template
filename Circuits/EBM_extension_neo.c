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
    conf->nRob = 11;

	phase = 2; 

    //enable or disable battery missions (automatically launched) 1: enabled , 0 : no battery mission
    conf->enableBattMiss = 1;

    // used in rob_battery
    // collective : the battery points are shared between all the robots
    // individual : one battery point per robot, named battx for rob x
    conf->battPntStrategy = 'collective';

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
    conf->simulationRunTime = 4;

    // Definition of pallets used in the differents flows : {palName, cpx(LENGHT,WIDTH), color}
    // Color has to be one of the following: BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, YELLOW, WHITE
    conf->pallets = {
        {'EUR',cpx(1.4,1.4),'YELLOW'},
    };

    // start the simulation in a diferent day, hour, minute, second. THIS CAN BE MODIFIED AS NEEDED.
    // Simulation Day: 2022-07-11
    conf->StartDay = 0;
    conf->StartHour = 15;
    conf->StartMinute = 0;
    conf->StartSecond = 0;

    conf->StartTime = conf->StartSecond + conf->StartMinute*60 + conf->StartHour*3600 + conf->StartDay*(24*3600); // Starting time in seconds. This value is added to the current time variable in mission_csv.cpp

    // Multiplying factor that will be applied to 'ratePerHour' for all of the missions
    conf->factor = 1.0;

    // Task List to be executed by the agvs
    conf->missions = {}; // do no touch this :)

   // * * * Insert your missions below : * * * * *

     conf->missions.{end+1} = varspc(
        'title','missEBM',
        'enable',1,
        'ratePerHour',0,
        'group',[],
        'missAuto',0,
        'objectives', {
            varspc('task', 'pick', 'grp', 'K03I_1'),
            varspc('task', 'drop', 'grp', 'K03I_1'),
        },
        'flows', {
            varspc('flowName','missEBM','numberOfStep',2,'flowDisplay',1),
        }, 'trigger', 'csv'
    );

}