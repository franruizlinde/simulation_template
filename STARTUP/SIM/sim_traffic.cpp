
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                              TRAFFIC FUNCTIONS                              //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Initializes the canton check fake thread
*/
export function initCantonCheck() {
    global g('simConf');
    unint errorOnCantonLoop = g->errorOnCantonLoop;
    unint g->nbCantonLoop = 0;

    if (errorOnCantonLoop) setRandCbk('$checkCantonLoop','1s','0s');

    // initialize traffic var for each rob
    k = for(0,g->nRob-1) {
        unint g->trafficTime.(k) = 0;
    }
}


/*
    Fake thread to check if there is a canton loop (called every second) and display an error if so
*/
export function checkCantonLoop() {
    global g('simConf');

    cantonTab = findLoop();
    if (any(cantonTab))   {
        g->nbCantonLoop = g->nbCantonLoop + 1 ;
        // For loop to select robs which are causing DeadLock
        arrayDL = [];
        k = for (cantonTab)
        {
            if(cantonTab.(k) == 1)
            {
                arrayDL.(end+1) = k+1;
            }
        }

        displayError(sprintf('Deadlock detected !! Rob: %z', arrayDL));
    }
}


/*
    Check the canton state of the robot and count the traffic time
    @return loop [L.bool] -> is there a loop
    @return lockedByIdrob [L.long] -> array of id of robots blocked
*/
export function [loop,lockedByIdrob] = findLoop() {
    global g('simConf');

    v = cantonDbg_GetState();

    if (isempty(v)) {
        loop = -1;
        lockedByIdrob = -1;
        return;
    }

    tmp.(v->idrob) = v->lockedByIdrob;
    loop = cantonFindLoop(tmp).(v->idrob);
    lockedByIdrob = v->idrobStp;

    simuTime = simGetT();
    if(simuTime > g->startCollectData) {
        i = for(lockedByIdrob) {
            if(lockedByIdrob.(i)>0) g->trafficTime.(i) = g->trafficTime.(i) + 1;
        }
    }
}
