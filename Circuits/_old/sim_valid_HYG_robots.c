export function conf = customConfig() {
    // Create config varspc
    conf = varspc();

    // Format for CSV files export
    // values : 'EU', 'US'
    conf->format = 'US';

    // Number of agvs to use in the Simulation
    // Agvs will always be picked in ordrer following the install.xml preference file
    conf->nRob = 14;

    //enable or disable battery missions (automatically launched) 1: enabled , 0 : no battery mission
    conf->enableBattMiss = 1;

    // battery point strategy
    // collective : the battery points are shared between all the robots
    // individual : one battery point per robot, named battx for rob x (ex batt1, batt2)
    // taxi :       the taxi points are used as battery points (taxi1 for rob 1, taxi2 for rob 2, ..)
    // taxiOpti :   the taxi points are used as battery points + the robots charge when staying at their taxi position (without mission)
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
        {'EUR',cpx(1.2,0.8),'YELLOW'},
    };

    // Multiplying factor that will be applied to 'ratePerHour' for all of the missions
    conf->factor = 10.0;

    // Task List to be executed by the agvs
    conf->missions = {}; // do no touch this :)

    robs = {'MC10C','MC10T','MC10TU','MC12C','MC12T','MC12TU','MC15C','MC15T','MC15TU','MO50','MO70F','MO70S','MPE080D','MPE080S'};
    idLines = {'243','309','779','817','855','893','931','969','1007','0','0','0','1121','1159'} ;

    // WRITE HERE THE NAMES OF THE ROBOTS YOU WANT TO TEST
    robsToTest = {'MC10C'} ;

    rt = for(robsToTest) {
        r = for(robs) {
            if (!strcmp(robsToTest.{rt},robs.{r})) {
                if (!strMatch(robs.{r}, 'MO50') && !strMatch(robs.{r}, 'MO70F') && !strMatch(robs.{r}, 'MO70S')) {
                    conf->missions.{end+1} = varspc(
                        'title',sprintf('%s',robs.{r}),
                        'enable',1,
                        'ratePerHour',1.,
                        'group',r+1,
                        'missAuto',1,
                        'objectives', {
                            varspc('task', 'pick', 'dst', idLines.{r}),
                            varspc('task', 'move', 'dst', sprintf('move_%s',robs.{r}), 'destinationTime', 5.),
                            varspc('task', 'drop', 'dst', sprintf('drop_%s',robs.{r})),
                        },
                        'flows', {
                            varspc('flowName',sprintf('%s',robs.{r}),'numberOfStep',3,'flowDisplay',1),
                        }
                    );
                } else {
                    conf->missions.{end+1} = varspc(
                        'title',sprintf('%s',robs.{r}),
                        'enable',1,
                        'ratePerHour',1.,
                        'group',r+1,
                        'missAuto',1,
                        'objectives', {
                            varspc('task', 'move', 'dst', sprintf('move_%s_1',robs.{r}), 'destinationTime', 5.),
                            varspc('task', 'move', 'dst', sprintf('move_%s_2',robs.{r}), 'destinationTime', 5.),
                        },
                        'flows', {
                            varspc('flowName',sprintf('%s',robs.{r}),'numberOfStep',2,'flowDisplay',1),
                        }
                    );
                }
            }
        }
    }
}