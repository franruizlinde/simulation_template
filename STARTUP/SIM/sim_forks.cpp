
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                               FORKS FUNCTIONS                               //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


/*
  Initialise the forks DB and fill it
*/
export function initForks() {
    global g('simConf');

    //Forks DB definition
    def = varspc('id',        0,
                'rob',        0,
                //'cmd',        strn(''),
                'name',       strn(''),
                'h',          0.0,
                'actuatorID', 0,
                /*'tolerance',  0.0,
                'maxSpeed',   0.0,
                'controller', 0,*/
                );

    DBcreate_gbl('simForks',def,'id',0);

    //Empty DB at the beginning
    allKeys = DBgetAllKeys_gbl('simForks');
    DBdel_gbl('simForks',allKeys);

    //Fill the forks DB
    i = for(1,g->nRob) {
        idrob = i;
        [frkCmds, _] = acall("projectForkDef",idrob);
        k = for(frkCmds)
        {
            v = frkCmds.{k};
            v->rob = idrob;
            DBadd_gbl('simForks', v);
        }
    }
}


/**
 * Function to define the fork configuration
 * It must contains 2 varspc :
 * List of all standard forks levels that will be used in the project
 * Can be altered by an environment callback : [frkCmd, autoMove] = projectForkDef_customization(frkCmd, autoMove)
 *
 * @param {long} (optional) id of the investigated robot
 *
 * @return {cell} frkCmd configuration. List of all standard forks levels that will be used in the project. Each line is an entry in the frkDB database
 * @return {varspc} autoMove configuration. List of parameters to tune the autoForks thread (sdk thread in charge of handling forks movement during travel)
 *
 * @public
 */
export function [frkCmd, autoMove] = projectForkDef(idrob) {
    serial = mySerial(idrob);

    frkCmd = {};
    vertical    = varspc();
    initialLift = varspc();
    //rotation    = varspc();
    //sideShift   = varspc();
    //reach       = varspc();

    // get forks pref
    [verticalForksEnabled, initialLiftEnabled] = areForksEnabled(idrob);

    ///////////////////////////////////////////////////////////////////
    // 1. Setting default heights according to general safety standards
    // 1.1 Tugger case
    if(isTugger(idrob) && initialLiftEnabled) {
        initialLift->hitch      = forkDefNew("hitch",   1,    INITIALLIFT(), idrob);
        initialLift->unhitch    = forkDefNew("unhitch", 0,   INITIALLIFT(), idrob);
    }
    else {
        //1.2 Vertical forks case
        if(verticalForksEnabled)  {
            verticalForksThickness          = getPreferenceValue("forksParameters/verticalForksThickness", serial);
            minimumForksHeight              = getPreferenceValue("sensors/encoderVerticalForks/physicalMeasureLowStop", serial);
            bottomDeckBoardThickness        = getPreferenceValue("detection/pallet3D/pallets/bottomDeckBoardThickness", serial);
            centralGapHeight                = getPreferenceValue("detection/pallet3D/pallets/centralGapHeight", serial);
            palletHeight                    = min(bottomDeckBoardThickness + centralGapHeight);
            frkHeightPickDropGround         = max((verticalForksThickness + palletHeight) / 2, minimumForksHeight) ;

            vertical->emptyForward      = forkDefNew("emptyForward",   FORKS_getTravelHeight("empty", "forward", idrob),   VERTICALFORKS(), idrob);
            vertical->emptyBackward     = forkDefNew("emptyBackward",  FORKS_getTravelHeight("empty", "backward", idrob),  VERTICALFORKS(), idrob);
            vertical->carryForward      = forkDefNew("carryForward",   FORKS_getTravelHeight("carry", "forward", idrob),   VERTICALFORKS(), idrob);
            vertical->carryBackward     = forkDefNew("carryBackward",  FORKS_getTravelHeight("carry", "backward", idrob),  VERTICALFORKS(), idrob);
            vertical->emptyScanning     = forkDefNew("emptyScanning",  FORKS_getTravelHeight("empty", "scan",idrob),      VERTICALFORKS(), idrob);
            vertical->carryScanning     = forkDefNew("carryScanning",  FORKS_getTravelHeight("carry", "scan",idrob),      VERTICALFORKS(), idrob);
            vertical->taxi              = forkDefNew("taxi",           minimumForksHeight,                          VERTICALFORKS(), idrob);
            vertical->pickDropGround    = forkDefNew("pickDropGround", frkHeightPickDropGround,                     VERTICALFORKS(), idrob);
            vertical->palletLifting     = forkDefNew("palletLifting",  FORKS_getTravelHeight("carry", "forward",idrob),   VERTICALFORKS(), idrob);
        }

        // 1.3 LI case
        if(initialLiftEnabled) {
            initialLift->up             = forkDefNew("LI_up",   1,   INITIALLIFT(), idrob);
            initialLift->dw             = forkDefNew("LI_dw",   0,  INITIALLIFT(), idrob);

            // duplicate up and dw setup onto other names
            initialLift->taxi                   = initialLift->dw;
            initialLift->taxi->name             = "taxi";
            initialLift->pickDropGround         = initialLift->dw;
            initialLift->pickDropGround->name   = "pickDropGround";

            if (LI_getTravelHeight("empty", "forward",idrob) > 0)
                initialLift->emptyForward = initialLift->up;
            else
                initialLift->emptyForward = initialLift->dw;
            initialLift->emptyForward->name = "emptyForward";

            if (LI_getTravelHeight("empty", "backward",idrob) > 0)
                initialLift->emptyBackward = initialLift->up;
            else
                initialLift->emptyBackward = initialLift->dw;
            initialLift->emptyBackward->name = "emptyBackward";

            if (LI_getTravelHeight("carry", "forward",idrob) > 0)
                initialLift->carryForward = initialLift->up;
            else
                initialLift->carryForward = initialLift->dw;
            initialLift->carryForward->name = "carryForward";

            if (LI_getTravelHeight("carry", "backward",idrob) > 0)
                initialLift->carryBackward = initialLift->up;
            else
                initialLift->carryBackward = initialLift->dw;
            initialLift->carryBackward->name = "carryBackward";

            if (!verticalForksEnabled) {
                initialLift->palletLifting = initialLift->up;
                initialLift->palletLifting->name = "palletLifting";
            }
        }
    }

    // 2. Adding heights for special behaviours
    if(!isTugger(idrob)) {
        if(verticalForksEnabled) {
            // High speed forks height
            if(getPreferenceValue("forksActuators/verticalForks/highSpeed/enabled", serial)) {
                forksLevelAtHighSpeed = getPreferenceValue("forksActuators/verticalForks/highSpeed/forksLevelAtHighSpeed", serial);
                vertical->highSpeed   = forkDefNew("highSpeed", forksLevelAtHighSpeed, VERTICALFORKS(), idrob);
            }
        }
    }

    // 3. Merging varspc onto frkCmd
    frkCmd = {};
    k = for(vertical) {
        forkMovementVarspc = getVar(vertical, vertical.name(k));
        if (!isnull(forkMovementVarspc->h) && !isnan(forkMovementVarspc->h))
            frkCmd.{end+1}  = forkMovementVarspc;
    }
    k = for(initialLift) {
        forkMovementVarspc = getVar(initialLift, initialLift.name(k));
        if (!isnull(forkMovementVarspc->h) && !isnan(forkMovementVarspc->h))
            frkCmd.{end+1}  = forkMovementVarspc;
    }

    // 4. Defining AutoMoveVarspc
    distFromReverseToAllowFrkMvt    = getPreferenceValue("forksAnticipation/distFromReverseToAllowFrkMvt", serial);     //See automove varspc below
    distFromReverseToPreventFrkMvt  = getPreferenceValue("forksAnticipation/distFromReverseToPreventFrkMvt", serial);   //See automove varspc below

    autoMove =  varspc(
        'rpkFrkBeforeAR',       distFromReverseToAllowFrkMvt,
        'rpkNoFrkBeforeAr',     distFromReverseToPreventFrkMvt,
        'rpkFrkBeforeScanChoose', distFromReverseToAllowFrkMvt,
        'frkArEmpty',           'emptyBackward',
        'frkAvEmpty',           'emptyForward',
        'frkArCarry',           'carryBackward',
        'frkAvCarry',           'carryForward',
        'frkScanningEmpty',     'emptyScanning',
        'frkScanningCarry',     'carryScanning',
    );

}


/**
 * Functions used to build a forksDB entry out of all information
 *
 * @param {name}   height name
 * @param {double} forks height associated to the name
 * @param {long}   ID of the actuator
 * @param {long}   (optional) controller ID associated to this name
 * @param {double} (optional) tolerance associated to this name
 * @param {double} (optional) maximum speed associated to this name
 *
 * @return {varspc} varspc ready to be implemented in forksDB / projectForkDef
 *
 * @public
 */
export function forksDef = forkDefNew(name, height, actuatorID, idrob/*, controller, tolerance, maxSpeed*/) {
    serial = mySerial(idrob);

    // Special case for reachTrucks : forks have a blind zone where it's impossible to control height precisely
    forksHeightEndOfBlindZone = getPreferenceValue("forksParameters/verticalForksBlindThreshold",serial);

    if (actuatorID == VERTICALFORKS() && forksHeightEndOfBlindZone !=0 && height < forksHeightEndOfBlindZone) {
        height = 0.0;
        // controller = CONTROLLER_BLIND(); // is this needed?
    }

    forksDef = varspc('name', name,'actuatorID', actuatorID, 'h', height/*, 'controller', controller, 'tolerance', tolerance, 'maxSpeed', maxSpeed*/);

}


/**
 * Function that returns the adequate forks height for travel depending on the travel direction and presence of load, based on the truck design. (Does not take high speed mode into account)
 *
 * @param {string} mode carry or empty
 * @param {string} direction forward or backward
 *
 * @return {double} adequate height
 *
 * @public
 */
export function height = FORKS_getTravelHeight(mode, direction, idrob) {
    serial = mySerial(idrob);
    height          = nan();
      minHeightMargin  = 0.1; // 10cm margin to travel a bit higher than the minimum value allowed

    // Default arguments
    if(nargin < 1) mode        = "carry";
    if(nargin < 2) direction   = "forward";

    // Check arguments are correct
    if(strcmp(mode, "carry") && strcmp(mode, "empty")) {
        logError("[FORKS_getTravelHeight] - Incorrect mode %z", mode);
        return;
    }
    else if(strcmp(direction, "forward") && strcmp(direction, "backward") && strcmp(direction, "scan")) {
        logError("[FORKS_getTravelHeight] - Incorrect direction %z", direction);
        return;
    }

    // Check we are not completely out of subject
    if(isTugger(idrob) || !getPreferenceValue("forksActuators/verticalForks/enable",serial)) {
        return;
    }

    //Retrieve forks thickness and pallet height
    bottomDeckBoardThickness        = getPreferenceValue("detection/pallet3D/pallets/bottomDeckBoardThickness", serial);
    centralGapHeight                = getPreferenceValue("detection/pallet3D/pallets/centralGapHeight", serial);
    palletHeight                    = min(bottomDeckBoardThickness + centralGapHeight);
    forksThickness                = getPreferenceValue("forksParameters/verticalForksThickness",serial);

    // Check if there are inductive sensors mounted on the mast, which will impose certain forks height
    inductiveLowHeight  = getPreferenceValue("safety/forksLowInductiveHeight",serial);
    inductiveHighHeight = getPreferenceValue("safety/forksHighInductiveHeight",serial);

    // If there are inductive sensors on the mast, forks must be raised between the 2 sensors (and the truck design will be done so that the inductives are at the correct height)
    if(inductiveLowHeight != 0 && inductiveHighHeight != 0) {
        height  = (inductiveLowHeight + inductiveHighHeight) / 2;
    }
    else {
        // by default, there is no need to lift the forks, but we have to evaluate every possible configuration thay may require so.

        // test 1
        // if truck is equipped with a single inductive sensor, travel height is defined above this height
        if (inductiveLowHeight != 0) {
            height = max(height, inductiveLowHeight + minHeightMargin);
        }

        // test 2
        // if an active laser is blind if forks are too low, then we need to lift forks higher than this
        [isBlindWhenDrivingForward, isBlindWhenDrivingBackward, isBlindWhenScanning] = isSoftSafetyBlindWhenForksDown(idrob);
        if(strcmp(direction, "forward") == 0 && isBlindWhenDrivingForward) {
            height = max(height, getPreferenceValue("safety/design/minHeightRearSafetyEnabled", serial));
        }
        if (strcmp(direction, "backward") == 0 && isBlindWhenDrivingBackward) {
            height = max(height, getPreferenceValue("safety/design/minHeightRearSafetyEnabled", serial));
        }
        if (strcmp(direction, "scan") == 0 && isBlindWhenScanning) {
            height = max(height, getPreferenceValue("safety/design/minHeightRearSafetyEnabled", serial));
        }

        // test 3
        // if there is a risk of the pallet or the forks dragging on the floor
        if(strcmp(mode, "carry") == 0) {
            height = max(height, palletHeight + minHeightMargin);
        }

        // test 3.1
        if (isCounterBalanced(idrob)) {
            height = max(height, forksThickness + minHeightMargin);
        }

        // test 5
        // The travel forks height cannot be lower than the lowStop of the sensor (mechanical stop)
        height = max(height, getPreferenceValue("sensors/encoderVerticalForks/physicalMeasureLowStop", serial));
    }
}



/**
 * Function that returns the adequate Initial Lift height for travel depending on the travel direction and presence of load, based on the truck design. (Does not take high speed mode into account)
 *
 * @param {string} mode carry or empty
 * @param {string} direction forward or backward
 *
 * @return {double} adequate height
 *
 * @public
 */
export function height = LI_getTravelHeight(mode, direction, idrob) {
    serial = mySerial(idrob);
    height          = [];

    // Default arguments
    if(nargin < 1)  mode        = "carry";
    if(nargin < 2)  direction   = "forward";

    //get forks pref
    [verticalForksEnabled, initialLiftEnabled, moveWithInitialLiftUp] = areForksEnabled(idrob);

    // Check arguments are correct
    if(strcmp(mode, "carry") && strcmp(mode, "empty")) {
        logError("[LI_getTravelHeight] - Incorrect mode %z", mode);
        return;
    }
    else if(strcmp(direction, "forward") && strcmp(direction, "backward"))  {
        logError("[LI_getTravelHeight] - Incorrect direction %z", direction);
        return;
    }

    // Check we are not completely out of subject
    if(isTugger(idrob) || !initialLiftEnabled) {
        return;
    }

    //Special case to respect US standards
    LIupWhenTravellingEmpty = getPreferenceValue("safety/design/LIupWhenTravellingEmpty", serial);
    isSpecialCaseUS = (isPalTruck(idrob) && !LIupWhenTravellingEmpty && strcmp(direction, "forward") == 0 && strcmp(mode, "empty") == 0);

    //If there is at least one laser under Initial Lift, Initial Lift must be up during travel
    if((isSafetyLaserUnderInitialLift(idrob) && !isSpecialCaseUS) || moveWithInitialLiftUp) {
        height  = 1;
    }
    else {
        height  = 0;
    }
}


/*
    Compute time usesd by VNA to move its forks according to point configuration
    @param miss [varspc] -> mission varspc
    @return frkTime [float] -> time to simulate forks mouvement
*/
export function frkTime = getFrkMvtVNA(miss) {
    //detectionTime = miss->detectionTime;

    if(!strcmp(miss->task,'pick')) frkTime = 8 + 6 + 14 + 35; //frk position + lateral freespace + frk mvt + pick and retract
    else frkTime = 8 + 4 + 6 + 8 + 14 + 35; //frk position + beam detection + lateral freespace + free slot detection  + frk mvt + drop and retract

    logDebug('[frkTimeVNA] : %z s (%s %z)',frkTime,miss->pointData->type,miss->destination);
}


/*
 *  Function to move vertical forks and initial lift for travel
 *  @param miss [varspc] -> mission varspace
 *  @param direction [L.char] -> direction of the truck : 'Forward' or 'Backward'
 *  @param tag [L.char] -> tag to display in logs (optional)
*/
export function robMoveForksTravel(miss,direction,tag) {
    serial = mySerial(robGetId);

    if(nargin < 3) tag = '';

    //rob get state
    if(robGetCarryingState) state = 'carry';
    else state = 'empty';

    //find forks movements
    LI_h = robGetForksHeightFromName(robGetId,INITIALLIFT(),[state,direction]);
    frk_h = robGetForksHeightFromName(robGetId,VERTICALFORKS(),[state,direction]);

    //LI variable
    LI_endtime = 0;
    LI_allowedDuringTravel = getPreferenceValue("forksActuators/initialLift/allowedDuringTravel",serial);
    //do LI movements
    if(!isempty(LI_h)) {

      // move the initial lift
      robMoveInitialLift(LI_h);
      logInstall('[FORKS-%s] Move LI to %0.1f for rob %i',tag,LI_h,robGetId);

      // stop the truck if needed (from pref)
      if(0 == LI_allowedDuringTravel) robSetvmax(0.0); //stop the truck

      // wait for initial lift end movement
      robWaitForForks();

      // reset speed
      robSetvmax(miss->vmaxEvac);
    }

    //do vertical forks movements
    if(!isempty(frk_h)) {
      // get forks data
      frk_h_current = robGetForksHeight();
      frk_h_limit = miss->travelLimit;

      // if forks are too height lower them until the limit
      if(frk_h_current > frk_h_limit) {
        robSetvmax(0.0); //stop the robot
        robMoveForksToHeight(frk_h_limit); //lower the forks until the limit
        logInstall('[FORKS-%s] Forks too high (%0.2f > %0.2f) for rob %i. Stop the robot and lower the forks.',tag,frk_h_current,frk_h_limit,robGetId);
        robWaitForForks();
      }

      // move the vertical forks
      robMoveForksToHeight(frk_h);
      logInstall('[FORKS-%s] Move FRK to %0.2f for rob %i',tag,frk_h,robGetId);
      // reset evac speed
      robSetvmax(miss->vmaxEvac);
      robWaitForForks();

    }

}

/*
    Set vertical forks and initial lift height for tasks (pick and drop)
    @param height [double] ->  target height
    @param initialLift [integer] -> -1 : no movement needed | 0 : LI_down | 1 : LI_up
    @param waitForForks [bool] -> 1 : wait for end of forks movement | 0 : no need to wait (continue)
    @param tag [L.char] -> tag to display in logs (optional)
*/
export function robMoveForksTask(frk_h,LI_h,waitForForks,tag) {
    if(nargin<4) tag = '';
    if(nargin<3) waitForForks = 1;
    if(nargin<2) initialLift = -1;

    // start timer
    t_start = simGetT();

    // get forks pref
    [verticalForksEnabled, initialLiftEnabled] = areForksEnabled();

    // set initial lift
    if(initialLiftEnabled && (LI_h > -1)) {
      robMoveInitialLift(LI_h);
      robWaitForForks(); //always wait for LI before vertical forks

      logInstall('[FORKS-%s] Initial lift done (LI = %.0f, %.1fs) rob %i',tag,robGetInitialLift(),(simGetT - t_start),robGetId());
    }

    // set vertical forks height target
    if(verticalForksEnabled) {
      robMoveForksToHeight(frk_h);
      //logInstall('[FORKS-%s] Set frk height to %.2fm (h = %.2fm) #%i [%s]',tag,frk_h,robGetForksHeight(),robGetId());

      //wait for forks if needed
      if(waitForForks) robWaitForForks();

      // value to simulate delay of forks (beginning and end)
      waitTime = 1.0; //TODO changer ça et comprendre pourquoi il attends moins qu'il ne devrait ..
      simWait(waitTime);

      logInstall('[FORKS-%s] Frk mvt done (h = %.2fm, %.1fs) rob %i',tag,robGetForksHeight(),(simGetT - t_start),robGetId());
    }
}

/*
    Function to simulate forks rotation (VNA)
    @param miss [varspc] -> mission varspc
*/
export function rotateForksVNA(miss) {
    global g('simConf');
    idrob = robGetId;

    //return if model is not a VNA
    if(strcmp(miss->truckType,'VNA') || !strcmp(miss->task,'move')) return;

    //if forks are not in the correct side //TODO ugly change that
    if(strcmp(g->VNA_frkSide.{idrob-1},miss->palletSide)) {
        logInstall("[palletSide] : Pallet is at %s (need to move frks)",miss->palletSide);
        // stop the robot
        robSetvmax(0.0);
        // make sure robot is stopped before moving the forks
        if(abs(robGetSpeed) > 0.2) {
            logDebug('[rotateForksVNA] Speed = %z - Need to stop (rob %i)',robGetSpeed(),robGetId());
            while(abs(robGetSpeed()) > 0.2) simWait(0.1);
        }
        //TODO clean this function
        simWait(g->VNA_frkRotationTime); // time to move frk to correct side
        g->VNA_frkSide.{idrob-1} = miss->palletSide;
    }

    //reset speed
    robSetvmax(miss->vmax);
}

export function remainingTime = moveForksVNA(currentHeight, targetHeight, idrob) {

    // get serial
    if(nargin < 3) idrob = robGetId;
    serial = number2Serial(idrob);

    if(!isKmatic(idrob)) displayError(sprintf("[moveForksVNA] Robot %i is not a VNA!",idrob));

    // get preferences
    PARAM_upperForksMaxSpeed =  getPreferenceValue("forksControllers/verticalForks/genericForksController/upperForks/maxVelocity", serial);
    PARAM_upperForksMaxAccel =  getPreferenceValue("forksControllers/verticalForks/genericForksController/upperForks/maxAccel", serial);
    PARAM_upperForksTau1 =      getPreferenceValue("forksControllers/verticalForks/genericForksController/upperForks/Tau1", serial);
    PARAM_lowerForksMaxSpeed =  getPreferenceValue("forksControllers/verticalForks/genericForksController/lowerForks/maxVelocity", serial);
    PARAM_lowerForksMaxAccel =  getPreferenceValue("forksControllers/verticalForks/genericForksController/lowerForks/maxAccel", serial);
    PARAM_lowerForksTau1 =      getPreferenceValue("forksControllers/verticalForks/genericForksController/lowerForks/Tau1", serial);

    PARAM_FirstMast =           getPreferenceValue("forksControllers/verticalForks/genericForksController/parameters/firstMast", serial);
    PARAM_SecondMast =          getPreferenceValue("forksControllers/verticalForks/genericForksController/parameters/secondMast", serial);
    //PARAM_auxMastIdleHeight =   getPreferenceValue("forksControllers/verticalForks/genericForksController/parameters/mastSwitchLength", serial); //check real pref
    PARAM_auxMastIdleHeight =   0.45; //check real pref

    // fixed parameters
    PARAM_ActivationTime = 2.;
    PARAM_mastChangeTime = 6;

    // SELECT FORKS MOVEMENT SENS
    if (targetHeight >= currentHeight) {
        maxSpeed = PARAM_upperForksMaxSpeed;
        maxAccel = PARAM_upperForksMaxAccel;
        tau1 =     PARAM_upperForksTau1;
    }
    else {
        maxSpeed = PARAM_lowerForksMaxSpeed;
        maxAccel = PARAM_lowerForksMaxAccel;
        tau1 = PARAM_lowerForksTau1;
    }

    // CALCULATE LINEAR MOUVEMENT DISTANCES
    remainingDist = abs(targetHeight  - currentHeight);

    limitedMaxSpeed = max(1e-3,min(2*maxAccel*remainingDist/2,maxSpeed));
    accelerationDistance = (pow(limitedMaxSpeed,2))/2/maxAccel;
    constantSpeedDistance = max(0,remainingDist - accelerationDistance*2);

    // CALCULATE LINEAR MOUVEMENT TIMES
    accelerationTime = limitedMaxSpeed/maxAccel;
    constantSpeedTime = constantSpeedDistance/limitedMaxSpeed;
    decelerationTime = accelerationTime + 3*tau1;

    // ANALYSE IF MAST CHANGE WILL HAVE PLACE
    firstMastTotalHeight = PARAM_FirstMast + PARAM_auxMastIdleHeight;
    secondMastTotalHeight = PARAM_SecondMast + PARAM_auxMastIdleHeight;
    firstMastChangeNeeded = (((targetHeight >= firstMastTotalHeight)>=0 && (currentHeight - firstMastTotalHeight)<=0) || ((targetHeight - firstMastTotalHeight)<=0 && (currentHeight - firstMastTotalHeight)>=0));
    secondMastChangeNeeded = (((targetHeight - secondMastTotalHeight)>=0 && (currentHeight - secondMastTotalHeight)<=0) ||  ((targetHeight - secondMastTotalHeight)<=0 && (currentHeight - secondMastTotalHeight)>=0));

    // CALCULATE TIME NEEDED FOR MAST CHANGE
    if (firstMastChangeNeeded) {
        timeForMastChange = PARAM_mastChangeTime;
    }
    else {
        timeForMastChange = 0;
    }
    if (secondMastChangeNeeded) {
        timeForMastChange = timeForMastChange + PARAM_mastChangeTime;
    }

    // CALCULATE TOTAL TIME NEEDED
    remainingTime = accelerationTime + constantSpeedTime + decelerationTime + timeForMastChange + PARAM_ActivationTime;
}

/*
   Function to get height from forks DB
*/
export function height = robGetForksHeightFromName(idrob, actuatorID,name) {
    id1 = setInter(DBfind_gbl('simForks','rob','==',idrob), DBfind_gbl('simForks','actuatorID','==',actuatorID));
    id2 = setInter(id1, DBfind_gbl('simForks','name','==',name));
    height = DBget_gbl('simForks',id2)->h;
}
