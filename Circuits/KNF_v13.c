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
    conf->nRob = 9;

    //enable or disable battery missions (automatically launched) 1: enabled , 0 : no battery mission
    conf->enableBattMiss = 0;

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
        {'EUR',cpx(1.2,0.8),'YELLOW'},
        {'UK',cpx(1.2,1.0),'GREEN'},
        {'custom',cpx(1.0,1.0),'WHITE'},
    };

    // start the simulation in a diferent day, hour, minute, second. THIS CAN BE MODIFIED AS NEEDED.
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
    conf->missions.{ end + 1} = varspc(
                'title', 'EP03to14000201L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '14000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to14000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to22000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '22000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to22000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12000201LtoOCSPKEP0061',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP006'),
        },
                'flows', {
                   varspc('flowName', '12000201LtoOCSPKEP006', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '15000201LtoOCSPKEP0061',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','15000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP006'),
        },
                'flows', {
                   varspc('flowName', '15000201LtoOCSPKEP006', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT004toOCSPKEP0061',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT004'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP006'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT004toOCSPKEP006', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to15000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '15000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to15000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT004toOCSPKEP0111',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT004'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP011'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT004toOCSPKEP011', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to14000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '14000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to14000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '15000201LtoOCSPKEP0131',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','15000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '15000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to15000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '15000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to15000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to10000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '10000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to10000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '19000201LtoOCSPKEP0131',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','19000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '19000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to18000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '18000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to18000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to10000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '10000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to10000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to04000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '04000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to04000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to18000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '18000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to18000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to18000201L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '18000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to18000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '11000201LtoOCSPKEP0131',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','11000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '11000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '21000201LtoOCSPKEP0081',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','21000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP008'),
        },
                'flows', {
                   varspc('flowName', '21000201LtoOCSPKEP008', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to16000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '16000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to16000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to12000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '12000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to12000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to04000201L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '04000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to04000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to18000201L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '18000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to18000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to12000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '12000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to12000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to05000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '05000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to05000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to18000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '18000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to18000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '07000201LtoOCSPKEP0131',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','07000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '07000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to12000201L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '12000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to12000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '05000201LtoOCSPKEP0131',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','05000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '05000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to22000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '22000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to22000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to10000201L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '10000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to10000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '05000201LtoOCSPKEP0111',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','05000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP011'),
        },
                'flows', {
                   varspc('flowName', '05000201LtoOCSPKEP011', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to19000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '19000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to19000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to06000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '06000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to06000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to08000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '08000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to08000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT004toOCSPKEP0131',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT004'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT004toOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to10000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '10000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to10000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to04000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '04000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to04000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to19000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '19000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to19000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '05000201LtoOCSPKEP0051',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','05000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP005'),
        },
                'flows', {
                   varspc('flowName', '05000201LtoOCSPKEP005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '09000201LtoOCSPKEP0091',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','09000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP009'),
        },
                'flows', {
                   varspc('flowName', '09000201LtoOCSPKEP009', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to20000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '20000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to20000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to08000001L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '08000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to08000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '09000201LtoOCSPKEP0101',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','09000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP010'),
        },
                'flows', {
                   varspc('flowName', '09000201LtoOCSPKEP010', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22000201LtoOCSPKEP0091',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP009'),
        },
                'flows', {
                   varspc('flowName', '22000201LtoOCSPKEP009', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '19000201LtoOCSPKEP0071',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','19000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP007'),
        },
                'flows', {
                   varspc('flowName', '19000201LtoOCSPKEP007', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to14000101L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '14000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to14000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to16000201L1',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '16000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to16000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT002toOCSPKEP0131',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [1],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT002'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT002toOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to12000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '12000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to12000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to15000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '15000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to15000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT003toOCSPKEP0092',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT003'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP009'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT003toOCSPKEP009', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT004toOCSPKEP0012',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT004'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP001'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT004toOCSPKEP001', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '15000201LtoOCSPKEP0122',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','15000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP012'),
        },
                'flows', {
                   varspc('flowName', '15000201LtoOCSPKEP012', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to21000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '21000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to21000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '19000201LtoOCSPKEP0132',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','19000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '19000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT004toOCSPKEP0102',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT004'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP010'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT004toOCSPKEP010', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '05000201LtoOCSPKEP0052',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','05000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP005'),
        },
                'flows', {
                   varspc('flowName', '05000201LtoOCSPKEP005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to12000101L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '12000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to12000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to14000201L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '14000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to14000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to08000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '08000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to08000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '15000201LtoOCSPKEP0032',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','15000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP003'),
        },
                'flows', {
                   varspc('flowName', '15000201LtoOCSPKEP003', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to04000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '04000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to04000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to15000101L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '15000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to15000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '17000201LtoOCSPKEP0062',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','17000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP006'),
        },
                'flows', {
                   varspc('flowName', '17000201LtoOCSPKEP006', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '15000201LtoOCSPKEP0132',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','15000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '15000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '09000201LtoOCSPKEP0132',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','09000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '09000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '07000201LtoOCSPKEP0132',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','07000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '07000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to10000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '10000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to10000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to06000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '06000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to06000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12000201LtoOCSPKEP0052',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP005'),
        },
                'flows', {
                   varspc('flowName', '12000201LtoOCSPKEP005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT003toOCSPKEP0132',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT003'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT003toOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22000201LtoOCSPKEP0132',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '22000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT004toOCSPKEP0132',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT004'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT004toOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12000201LtoOCSPKEP0132',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '12000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to22000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '22000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to22000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to05000101L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '05000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to05000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to22000101L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '22000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to22000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to16000101L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '16000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to16000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to06000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '06000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to06000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to10000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '10000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to10000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to16000201L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '16000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to16000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to18000201L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '18000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to18000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to15000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '15000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to15000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to16000101L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '16000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to16000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT001toUQM0012',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT001'),
                   varspc('task', 'drop', 'dst', 'UQM001'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT001toUQM001', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to12000201L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '12000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to12000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to08000201L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '08000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to08000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to20000101L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '20000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to20000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT003toOCSPKEP0012',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT003'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP001'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT003toOCSPKEP001', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '07000201LtoOCSPKEP0102',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','07000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP010'),
        },
                'flows', {
                   varspc('flowName', '07000201LtoOCSPKEP010', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to08000101L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '08000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to08000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '07000201LtoOCSPKEP0122',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','07000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP012'),
        },
                'flows', {
                   varspc('flowName', '07000201LtoOCSPKEP012', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '11000201LtoOCSPKEP0022',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','11000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP002'),
        },
                'flows', {
                   varspc('flowName', '11000201LtoOCSPKEP002', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT003toOCSPKEP0082',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT003'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP008'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT003toOCSPKEP008', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to18000201L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '18000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to18000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to19000001L2',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '19000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to19000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '11000201LtoOCSPKEP0132',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','11000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '11000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT003toOCSPKEP0112',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [2],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT003'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP011'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT003toOCSPKEP011', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to20000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '20000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to20000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to05000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '05000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to05000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '21000201LtoOCSPKEP0083',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','21000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP008'),
        },
                'flows', {
                   varspc('flowName', '21000201LtoOCSPKEP008', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '21000201LtoOCSPKEP0133',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','21000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '21000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to12000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '12000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to12000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to20000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '20000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to20000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to18000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '18000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to18000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to14000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '14000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to14000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT004toOCSPKEP0073',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT004'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP007'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT004toOCSPKEP007', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to22000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '22000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to22000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to08000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '08000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to08000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to22000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '22000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to22000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT004toOCSPKEP0053',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT004'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP005'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT004toOCSPKEP005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '05000201LtoOCSPKEP0133',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','05000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '05000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to12000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '12000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to12000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '17000201LtoOCSPKEP0133',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','17000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '17000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '15000201LtoOCSPKEP0013',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','15000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP001'),
        },
                'flows', {
                   varspc('flowName', '15000201LtoOCSPKEP001', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT004toOCSPKEP0133',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT004'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT004toOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '07000201LtoOCSPKEP0133',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','07000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', '07000201LtoOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to14000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '14000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to14000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to14000201L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '14000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to14000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to04000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '04000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to04000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to23000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '23000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to23000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to12000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '12000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to12000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to06000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '06000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to06000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to06000201L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '06000201L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to06000201L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to18000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '18000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to18000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '07000201LtoOCSPKEP0053',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','07000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP005'),
        },
                'flows', {
                   varspc('flowName', '07000201LtoOCSPKEP005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to16000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '16000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to16000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to08000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '08000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to08000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to12000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '12000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to12000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to15000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '15000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to15000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to11000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '11000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to11000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '05000201LtoOCSPKEP0103',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','05000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP010'),
        },
                'flows', {
                   varspc('flowName', '05000201LtoOCSPKEP010', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to20000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '20000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to20000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to14000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '14000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to14000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP03to04000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP03'),
                   varspc('task', 'drop', 'dst', '04000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP03to04000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to22000001L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '22000001L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to22000001L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'EP02to04000101L3',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','EP02'),
                   varspc('task', 'drop', 'dst', '04000101L'),
        },
                'flows', {
                   varspc('flowName', 'EP02to04000101L', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '05000201LtoOCSPKEP0123',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','05000201L'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP012'),
        },
                'flows', {
                   varspc('flowName', '05000201LtoOCSPKEP012', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', 'SHBTOUT001toOCSPKEP0133',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [3],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','SHBTOUT001'),
                   varspc('task', 'drop', 'dst', 'OCSPKEP013'),
        },
                'flows', {
                   varspc('flowName', 'SHBTOUT001toOCSPKEP013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240001LtoOCNPOUT0194',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT019'),
        },
                'flows', {
                   varspc('flowName', '12240001LtoOCNPOUT019', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '04240001LtoOCNPOUT0054',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','04240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT005'),
        },
                'flows', {
                   varspc('flowName', '04240001LtoOCNPOUT005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '06240001LtoOCNPOUT0024',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','06240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT002'),
        },
                'flows', {
                   varspc('flowName', '06240001LtoOCNPOUT002', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '06240101LtoOCNPOUT0194',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','06240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT019'),
        },
                'flows', {
                   varspc('flowName', '06240101LtoOCNPOUT019', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '04240101LtoOCNPOUT0034',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','04240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT003'),
        },
                'flows', {
                   varspc('flowName', '04240101LtoOCNPOUT003', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240001LtoOCNPOUT0104',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT010'),
        },
                'flows', {
                   varspc('flowName', '22240001LtoOCNPOUT010', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240001LtoOCNPOUT0094',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT009'),
        },
                'flows', {
                   varspc('flowName', '12240001LtoOCNPOUT009', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240001LtoOCNPOUT0014',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT001'),
        },
                'flows', {
                   varspc('flowName', '18240001LtoOCNPOUT001', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240001LtoOCNPOUT0154',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT015'),
        },
                'flows', {
                   varspc('flowName', '22240001LtoOCNPOUT015', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240101LtoOCNPOUT0114',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT011'),
        },
                'flows', {
                   varspc('flowName', '22240101LtoOCNPOUT011', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240101LtoOCNPOUT0244',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT024'),
        },
                'flows', {
                   varspc('flowName', '18240101LtoOCNPOUT024', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '16240001LtoOCNPOUT0044',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','16240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT004'),
        },
                'flows', {
                   varspc('flowName', '16240001LtoOCNPOUT004', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '10240201LtoOCNPOUT0064',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','10240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT006'),
        },
                'flows', {
                   varspc('flowName', '10240201LtoOCNPOUT006', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '16240101LtoOCNPOUT0194',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','16240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT019'),
        },
                'flows', {
                   varspc('flowName', '16240101LtoOCNPOUT019', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240101LtoOCNPOUT0164',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT016'),
        },
                'flows', {
                   varspc('flowName', '18240101LtoOCNPOUT016', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '05240101LtoOCNPOUT0044',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','05240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT004'),
        },
                'flows', {
                   varspc('flowName', '05240101LtoOCNPOUT004', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240201LtoOCNPOUT0044',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [4],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT004'),
        },
                'flows', {
                   varspc('flowName', '18240201LtoOCNPOUT004', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '08240101LtoOCNPOUT0175',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','08240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT017'),
        },
                'flows', {
                   varspc('flowName', '08240101LtoOCNPOUT017', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '14240101LtoOCNPOUT0135',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','14240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT013'),
        },
                'flows', {
                   varspc('flowName', '14240101LtoOCNPOUT013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '06240101LtoOCNPOUT0165',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','06240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT016'),
        },
                'flows', {
                   varspc('flowName', '06240101LtoOCNPOUT016', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '14240201LtoOCNPOUT0105',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','14240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT010'),
        },
                'flows', {
                   varspc('flowName', '14240201LtoOCNPOUT010', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '15240001LtoOCNPOUT0035',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','15240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT003'),
        },
                'flows', {
                   varspc('flowName', '15240001LtoOCNPOUT003', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '06240001LtoOCNPOUT0025',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','06240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT002'),
        },
                'flows', {
                   varspc('flowName', '06240001LtoOCNPOUT002', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240001LtoOCNPOUT0145',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT014'),
        },
                'flows', {
                   varspc('flowName', '22240001LtoOCNPOUT014', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240001LtoOCNPOUT0045',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT004'),
        },
                'flows', {
                   varspc('flowName', '12240001LtoOCNPOUT004', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '08240001LtoOCNPOUT0185',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','08240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT018'),
        },
                'flows', {
                   varspc('flowName', '08240001LtoOCNPOUT018', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '15240101LtoOCNPOUT0195',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','15240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT019'),
        },
                'flows', {
                   varspc('flowName', '15240101LtoOCNPOUT019', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '06240201LtoOCNPOUT0205',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','06240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT020'),
        },
                'flows', {
                   varspc('flowName', '06240201LtoOCNPOUT020', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240101LtoOCNPOUT0165',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT016'),
        },
                'flows', {
                   varspc('flowName', '12240101LtoOCNPOUT016', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240101LtoOCNPOUT0175',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT017'),
        },
                'flows', {
                   varspc('flowName', '22240101LtoOCNPOUT017', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240201LtoOCNPOUT0055',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT005'),
        },
                'flows', {
                   varspc('flowName', '22240201LtoOCNPOUT005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240201LtoOCNPOUT0245',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT024'),
        },
                'flows', {
                   varspc('flowName', '12240201LtoOCNPOUT024', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240001LtoOCNPOUT0085',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT008'),
        },
                'flows', {
                   varspc('flowName', '18240001LtoOCNPOUT008', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '13240001LtoOCNPOUT0125',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','13240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT012'),
        },
                'flows', {
                   varspc('flowName', '13240001LtoOCNPOUT012', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '08240101LtoOCNPOUT0225',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','08240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT022'),
        },
                'flows', {
                   varspc('flowName', '08240101LtoOCNPOUT022', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240101LtoOCNPOUT0075',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT007'),
        },
                'flows', {
                   varspc('flowName', '18240101LtoOCNPOUT007', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240001LtoOCNPOUT0095',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT009'),
        },
                'flows', {
                   varspc('flowName', '22240001LtoOCNPOUT009', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '20240001LtoOCNPOUT0105',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','20240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT010'),
        },
                'flows', {
                   varspc('flowName', '20240001LtoOCNPOUT010', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '14240001LtoOCNPOUT0185',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','14240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT018'),
        },
                'flows', {
                   varspc('flowName', '14240001LtoOCNPOUT018', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '04240001LtoOCNPOUT0045',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','04240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT004'),
        },
                'flows', {
                   varspc('flowName', '04240001LtoOCNPOUT004', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '20240101LtoOCNPOUT0115',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','20240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT011'),
        },
                'flows', {
                   varspc('flowName', '20240101LtoOCNPOUT011', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '14240101LtoOCNPOUT0175',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','14240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT017'),
        },
                'flows', {
                   varspc('flowName', '14240101LtoOCNPOUT017', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240101LtoOCNPOUT0085',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT008'),
        },
                'flows', {
                   varspc('flowName', '22240101LtoOCNPOUT008', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '10240001LtoOCNPOUT0145',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','10240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT014'),
        },
                'flows', {
                   varspc('flowName', '10240001LtoOCNPOUT014', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '10240101LtoOCNPOUT0125',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','10240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT012'),
        },
                'flows', {
                   varspc('flowName', '10240101LtoOCNPOUT012', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240201LtoOCNPOUT0015',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT001'),
        },
                'flows', {
                   varspc('flowName', '22240201LtoOCNPOUT001', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '20240201LtoOCNPOUT0135',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','20240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT013'),
        },
                'flows', {
                   varspc('flowName', '20240201LtoOCNPOUT013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '14240201LtoOCNPOUT0225',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','14240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT022'),
        },
                'flows', {
                   varspc('flowName', '14240201LtoOCNPOUT022', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240001LtoOCNPOUT0045',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT004'),
        },
                'flows', {
                   varspc('flowName', '18240001LtoOCNPOUT004', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240001LtoOCNPOUT0075',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT007'),
        },
                'flows', {
                   varspc('flowName', '12240001LtoOCNPOUT007', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '10240201LtoOCNPOUT0115',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','10240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT011'),
        },
                'flows', {
                   varspc('flowName', '10240201LtoOCNPOUT011', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '11240001LtoOCNPOUT0055',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','11240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT005'),
        },
                'flows', {
                   varspc('flowName', '11240001LtoOCNPOUT005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240101LtoOCNPOUT0095',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT009'),
        },
                'flows', {
                   varspc('flowName', '18240101LtoOCNPOUT009', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240101LtoOCNPOUT0065',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT006'),
        },
                'flows', {
                   varspc('flowName', '12240101LtoOCNPOUT006', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '16240001LtoOCNPOUT0185',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','16240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT018'),
        },
                'flows', {
                   varspc('flowName', '16240001LtoOCNPOUT018', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '10240001LtoOCNPOUT0205',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','10240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT020'),
        },
                'flows', {
                   varspc('flowName', '10240001LtoOCNPOUT020', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '16240101LtoOCNPOUT0095',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','16240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT009'),
        },
                'flows', {
                   varspc('flowName', '16240101LtoOCNPOUT009', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '20240001LtoOCNPOUT0035',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','20240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT003'),
        },
                'flows', {
                   varspc('flowName', '20240001LtoOCNPOUT003', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240101LtoOCNPOUT0135',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT013'),
        },
                'flows', {
                   varspc('flowName', '12240101LtoOCNPOUT013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240201LtoOCNPOUT0105',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT010'),
        },
                'flows', {
                   varspc('flowName', '12240201LtoOCNPOUT010', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '20240101LtoOCNPOUT0155',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','20240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT015'),
        },
                'flows', {
                   varspc('flowName', '20240101LtoOCNPOUT015', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '14240001LtoOCNPOUT0035',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','14240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT003'),
        },
                'flows', {
                   varspc('flowName', '14240001LtoOCNPOUT003', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '22240101LtoOCNPOUT0025',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','22240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT002'),
        },
                'flows', {
                   varspc('flowName', '22240101LtoOCNPOUT002', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240001LtoOCNPOUT0015',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT001'),
        },
                'flows', {
                   varspc('flowName', '12240001LtoOCNPOUT001', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '06240101LtoOCNPOUT0075',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','06240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT007'),
        },
                'flows', {
                   varspc('flowName', '06240101LtoOCNPOUT007', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240101LtoOCNPOUT0115',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT011'),
        },
                'flows', {
                   varspc('flowName', '18240101LtoOCNPOUT011', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '06240001LtoOCNPOUT0125',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','06240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT012'),
        },
                'flows', {
                   varspc('flowName', '06240001LtoOCNPOUT012', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240201LtoOCNPOUT0135',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT013'),
        },
                'flows', {
                   varspc('flowName', '18240201LtoOCNPOUT013', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '19240001LtoOCNPOUT0095',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','19240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT009'),
        },
                'flows', {
                   varspc('flowName', '19240001LtoOCNPOUT009', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '10240001LtoOCNPOUT0165',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','10240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT016'),
        },
                'flows', {
                   varspc('flowName', '10240001LtoOCNPOUT016', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '06240101LtoOCNPOUT0185',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','06240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT018'),
        },
                'flows', {
                   varspc('flowName', '06240101LtoOCNPOUT018', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '18240001LtoOCNPOUT0215',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','18240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT021'),
        },
                'flows', {
                   varspc('flowName', '18240001LtoOCNPOUT021', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240001LtoOCNPOUT0065',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT006'),
        },
                'flows', {
                   varspc('flowName', '12240001LtoOCNPOUT006', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '10240001LtoOCNPOUT0155',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','10240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT015'),
        },
                'flows', {
                   varspc('flowName', '10240001LtoOCNPOUT015', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '17240101LtoOCNPOUT0055',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','17240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT005'),
        },
                'flows', {
                   varspc('flowName', '17240101LtoOCNPOUT005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '08240001LtoOCNPOUT0065',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','08240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT006'),
        },
                'flows', {
                   varspc('flowName', '08240001LtoOCNPOUT006', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '20240001LtoOCNPOUT0065',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','20240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT006'),
        },
                'flows', {
                   varspc('flowName', '20240001LtoOCNPOUT006', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '19240101LtoOCNPOUT0145',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','19240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT014'),
        },
                'flows', {
                   varspc('flowName', '19240101LtoOCNPOUT014', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '13240001LtoOCNPOUT0035',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','13240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT003'),
        },
                'flows', {
                   varspc('flowName', '13240001LtoOCNPOUT003', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '11240101LtoOCNPOUT0195',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','11240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT019'),
        },
                'flows', {
                   varspc('flowName', '11240101LtoOCNPOUT019', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '12240201LtoOCNPOUT0175',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','12240201L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT017'),
        },
                'flows', {
                   varspc('flowName', '12240201LtoOCNPOUT017', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '20240101LtoOCNPOUT0025',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','20240101L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT002'),
        },
                'flows', {
                   varspc('flowName', '20240101LtoOCNPOUT002', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

            conf->missions.{ end + 1} = varspc(
                'title', '14240001LtoOCNPOUT0055',
                'enable', 1,
                'ratePerHour', 0, 
                'group', [5],
                'missAuto', 0,
                'objectives', {
                   varspc('task', 'pick', 'dst','14240001L'),
                   varspc('task', 'drop', 'dst', 'OCNPOUT005'),
        },
                'flows', {
                   varspc('flowName', '14240001LtoOCNPOUT005', 'numberOfStep', 2, 'flowDisplay', 1),
        }
    , 'trigger' , 'csv' );

}
