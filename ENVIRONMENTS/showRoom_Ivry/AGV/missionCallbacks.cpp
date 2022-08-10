/** 
===========================================
    Robot main callbacks
===========================================
*/
export function runCustomFeatures_cbk() {}

/** 
===========================================
    initMissVarspc callbacks
===========================================
*/
export function initMissVarspc_cbk(missVarspc)
{
}
export function updateMissVarspc_cbk(missVarspc)
{
}

/** 
===========================================
    startMissionProtocol callbacks
===========================================
*/
export function startMissionProtocol_cbk(missVarspc) {}

/** 
===========================================
    navigateToLastReverse callbacks
===========================================
*/
export function navigateToLastReverse_startCbk(missVarspc,localVarspc) {}
export function navigateToLastReverse_whileCbk(missVarspc,localVarspc)
{
    /*
    // Exemple of how to define a variable into this function    
    if(!isvar(localVarspc,'counter'))
    {
        localVarspc->counter = 0;
    }
    else
    {
        localVarspc->counter = localVarspc->counter + 1;
    }
    */
}
export function navigateToLastReverse_endCbk(missVarspc,localVarspc) {}

/**
===========================================
    stopAtNewRpkBeforeReverse callbacks
===========================================
*/
export function stopAtNewRpkBeforeRev_whileCbk(missVarspc,localVarspc) {}

/**
===========================================
    navigateToRpkApproach callbacks
===========================================
*/
export function navigateToRpkApproach_startCbk(missVarspc,localVarspc) {}
export function navigateToRpkApproach_whileCbk(missVarspc,localVarspc) {}
export function navigateToRpkApproach_endCbk(missVarspc,localVarspc) {}

/**
===========================================
    navigateToRpkSafetyFence callbacks
===========================================
*/
export function navigateSafetyFence_startCbk(missVarspc,localVarspc) {}
export function navigateSafetyFence_whileCbk(missVarspc,localVarspc) {}
export function navigateSafetyFence_endCbk(missVarspc,localVarspc) {}

/**
===========================================
    PICK_navigateToRpkDestination callbacks
===========================================
*/
export function PICK_navigateRpkDest_startCbk(missVarspc,localVarspc) {}
export function PICK_navigateRpkDest_whileCbk(missVarspc,localVarspc) {}
export function PICK_navigateRpkDest_endCbk(missVarspc,localVarspc) {}

/**
===========================================
    DROP_navigateToRpkDestination callbacks
===========================================
*/
export function DROP_navigateRpkDest_startCbk(missVarspc,localVarspc) {}
export function DROP_navigateRpkDest_whileCbk(missVarspc,localVarspc) {}
export function DROP_navigateRpkDest_endCbk(missVarspc,localVarspc) {}

/**
===========================================
    BATT_navigateToRpkDestination callbacks
===========================================
*/
export function BATT_navigateRpkDest_startCbk(missVarspc,localVarspc) {}
export function BATT_navigateRpkDest_whileCbk(missVarspc,localVarspc) {}
export function BATT_navigateRpkDest_endCbk(missVarspc,localVarspc) {}

/**
===========================================
    PICK_finishMission callbacks
===========================================
*/

export function PICK_finishMission_startCbk(missVarspc, localVarspc) {}
export function PICK_finishMission_endCbk(missVarspc, localVarspc) {}

/**
===========================================
    DROP_finishMission callbacks
===========================================
*/

export function DROP_finishMission_startCbk(missVarspc, localVarspc) {}
export function DROP_finishMission_endCbk(missVarspc, localVarspc) {}

/**
===========================================
    MOVE_finishMission callbacks
===========================================
*/

export function MOVE_finishMission_startCbk(missVarspc, localVarspc) {}
export function MOVE_finishMission_endCbk(missVarspc, localVarspc) {}

/**
===========================================
    BATT_finishMission callbacks
===========================================
*/

export function BATT_finishMission_startCbk(missVarspc, localVarspc) {}
export function BATT_finishMission_endCbk(missVarspc, localVarspc) {}

/**
===========================================
    TAXI_finishMission callbacks
===========================================
*/

export function TAXI_finishMission_startCbk(missVarspc, localVarspc) {}
export function TAXI_finishMission_endCbk(missVarspc, localVarspc) {}

/**
===========================================
    Scan error cases callbacks
===========================================
*/
export function customEmptyScanErrCase_cbk(missVarspc) {}
export function customEmptyRevScanErrCase_cbk(missVarspc) {}
export function customObstacleScanErrCase_cbk(missVarspc) {}
export function customFullScanErrCase_cbk(missVarspc) {}

/**
===========================================
    Fill Strategy callbacks
===========================================
*/
export function idxRes	= customDropFillStrategy_cbk(miss, lPointFinalStatus, lPointEmptyStatus, withDisplay) 
{
	idxRes	= -1;
}
export function idxRes	= customPickEmptyStrategy_cbk(miss, lPointFinalStatus, lPointEmptyStatus, withDisplay) 
{
	idxRes	= -1;
}
export function rpkScanChoice	= customRpkFillStrategy_cbk(miss) 
{
	rpkScanChoice	= -1;
}

/**
===========================================
    Allow rob to auto callbacks
===========================================
*/
export function [ok, st]	= customAllowRobToAutoCheck_cbk()
{
	//Add here additional conditions to authorize the robot to switch back to auto mode
	ok	= 1;
	st	= "";
}