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
    conf->nRob = 1;

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
    conf->simulationRunTime = 2;

    // Definition of pallets used in the differents flows : {palName, cpx(LENGHT,WIDTH), color}
    // Color has to be one of the following: BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, YELLOW, WHITE
    conf->pallets = {
        {'EUR',cpx(1.2,0.8),'YELLOW'},
    };

    // Multiplying factor that will be applied to 'ratePerHour' for all of the missions
    conf->factor = 1.0;

    // Task List to be executed by the agvs
    conf->missions = {}; // do no touch this :)

    // * * * Insert your missions below : * * * * *

    conf->missions.{end+1} = varspc(
        'title','Mission_1',
        'enable',1,
        'ratePerHour',5,
        'group',[],
        'missAuto',1,
        'objectives', {
            varspc('task', 'pick', 'grp', 'rack_north'),
            varspc('task', 'drop', 'grp', 'stock_south'),
            varspc('task', 'move', 'dst', 'move_1', 'nbPal',0, 'destinationTime',15),
        },
        'flows', {
            varspc('flowName','Flow_1','numberOfStep',3,'flowDisplay',1),
        }
    );

    conf->missions.{end+1} = varspc(
    		'title','Mission_2',
    		'enable',1,
    		'ratePerHour',5,
    		'group',[],
    		'missAuto',1,
    		'objectives', {
    			varspc('task', 'pick', 'dst', '312','slot',3),
    			varspc('task', 'drop', 'dst', '308','slot',0),
    		},
    		'flows', {
    			varspc('flowName','Flow_2','numberOfStep',2,'flowDisplay',1),
    		}
    );


//

}
