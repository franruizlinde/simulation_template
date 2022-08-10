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

    // battery point strategy
    // collective : the battery points are shared between all the robots
    // individual : one battery point per robot, named battx for rob x (ex batt1, batt2)
    // taxi :       the taxi points are used as battery points (taxi1 for rob 1, taxi2 for rob 2, ..)
    // taxiOpti :   the taxi points are used as battery points + the robots charge when staying at their taxi position (without mission)
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
    conf->simulationRunTime = 8;

    // Definition of pallets used in the differents flows : {palName, cpx(LENGHT,WIDTH), color}
    // Color has to be one of the following: BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, YELLOW, WHITE
    conf->pallets = {
        {'EUR',cpx(1.2,0.8),'YELLOW'},
        {'UK',cpx(1.2,1.0),'GREEN'},
        {'custom',cpx(1.5,1.5),'WHITE'},
    };

    // Multiplying factor that will be applied to 'ratePerHour' for all of the missions
    conf->factor = 1.0;

    // Task List to be executed by the agvs
    conf->missions = {}; // do no touch this :)

    conf->missions.{end+1} = varspc(
        'title','Flow_1',
        'enable',1,
        'ratePerHour',5,
        'group',[],
        'missAuto',1,
        'objectives', {
            varspc('task', 'pick', 'dst', '3300', 'nbPal',1),
            varspc('task', 'drop', 'dst', '3396', 'nbPal',1),
        },
        'flows', {
            varspc('flowName','Flow_1','numberOfStep',2,'flowDisplay',1),
        }
    );

    conf->missions.{end+1} = varspc(
        'title','Flow_2',
        'enable',1,
        'ratePerHour',5,
        'group',[1,2],
        'missAuto',1,
        'objectives', {
            varspc('task', 'pick', 'dst', '3393', 'nbPal',1),
            varspc('task', 'drop', 'grp', 'stock_1', 'nbPal',1),
        },
        'flows', {
            varspc('flowName','Flow_2','numberOfStep',2,'flowDisplay',1),
        }
    );

    groupTest = {'stock_1','rack_1'};

    conf->missions.{end+1} = varspc(
        'title','Flow_3_1',
        'enable',1,
        'ratePerHour',2,
        'group',1,
        'missAuto',1,
        'objectives', {
            varspc('task', 'pick', 'dst', '3392', 'nbPal',1),
            varspc('task', 'move', 'dst', '3546', 'nbPal',1, 'destinationTime', 10.),
            varspc('task', 'drop', 'grp', groupTest, 'nbPal',1),
        },
        'flows', {
            varspc('flowName','Flow_3_1','numberOfStep',3,'flowDisplay',1),
        }
    );

    conf->missions.{end+1} = varspc(
        'title','Flow_3_2',
        'enable',1,
        'ratePerHour',2,
        'group',2,
        'missAuto',1,
        'objectives', {
            varspc('task', 'pick', 'dst', '3392', 'nbPal',1),
            varspc('task', 'move', 'dst', 'move1', 'nbPal',1, 'destinationTime', 10),
            varspc('task', 'drop', 'dst', '3471', 'nbPal',1),
        },
        'flows', {
            varspc('flowName','Flow_3_2','numberOfStep',3,'flowDisplay',1),
        }
    );

}