export function conf = customConfig() {
    // Create config varspc
    conf = varspc();

    // Format for CSV files export
    // values : 'EU', 'US'
    conf->format = 'US';

    // Number of agvs to use in the Simulation
    // Agvs will always be picked in ordrer following the install.xml preference file
    conf->nRob = 22;

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

    robs = {'K1','K2','L12ACHL','L12ACLL','L12HPA','L12HPB','L12HPC','L16ACHL','L16ACLL','L16HD3','L16HD4','L20','P50A','P50FV','R164','R165','R165HL','T30','RG24','RG24S','RG25E','RG25SE'};
    idLines = {'0','0','779','817','855','893','931','969','1007','1235','1045','1083','0','0','2283','2321','2359','3366','4297','4383','4454','4625'};

    // WRITE HERE THE NAMES OF THE ROBOTS YOU WANT TO TEST
    robsToTest = {'RG24S'} ;

    rt = for(robsToTest) {
        r = for(robs) {
            if (!strcmp(robsToTest.{rt},robs.{r})) {
                switch(robs.{r}) {
                    case('K1','K2');
                        conf->missions.{end+1} = varspc(
                            'title',sprintf('%s',robs.{r}),
                            'enable',1,
                            'ratePerHour',1.,
                            'group',r+1,
                            'missAuto',1,
                            'objectives', {
                                varspc('task', 'pick', 'dst', sprintf('pick_%s',robs.{r})),
                                varspc('task', 'move', 'dst', sprintf('move_%s',robs.{r}), 'destinationTime', 5.),
                                varspc('task', 'drop', 'dst', sprintf('drop_%s',robs.{r})),
                            },
                            'flows', {
                                varspc('flowName',sprintf('%s',robs.{r}),'numberOfStep',3,'flowDisplay',1),
                            }
                        );

                    case('P50A','P50FV');
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

                    case('T30');
                        conf->missions.{end+1} = varspc(
                            'title',sprintf('%s',robs.{r}),
                            'enable',1,
                            'ratePerHour',1.,
                            'group',r+1,
                            'missAuto',1,
                            'objectives', {
                                varspc('task', 'pick', 'dst', idLines.{r}),
                                varspc('task', 'move', 'dst', sprintf('move_%s',robs.{r}), 'destinationTime', 5.),
                                varspc('task', 'drop', 'grp', sprintf('%s',robs.{r})),
                            },
                            'flows', {
                                varspc('flowName',sprintf('%s',robs.{r}),'numberOfStep',3,'flowDisplay',1),
                            }
                        );

                    default;
                        conf->missions.{end+1} = varspc(
                            'title',sprintf('%s',robs.{r}),
                            'enable',1,
                            'ratePerHour',1.,
                            'group',r+1,
                            'missAuto',1,
                            'objectives', {
                                varspc('task', 'pick', 'dst', idLines.{r}, 'nbPal',1),
                                varspc('task', 'move', 'dst', sprintf('move_%s',robs.{r}), 'destinationTime', 5.),
                                varspc('task', 'drop', 'dst', sprintf('drop_%s',robs.{r})),
                            },
                            'flows', {
                                varspc('flowName',sprintf('%s',robs.{r}),'numberOfStep',3,'flowDisplay',1),
                            }
                        );
                }
            }
        }
    }
}
