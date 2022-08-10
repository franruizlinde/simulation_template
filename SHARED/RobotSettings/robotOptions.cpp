/**
 * Function to get the battery type
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {string} LTO or Ld depending on the robot serial
 *
 * @public
 */
export function out = getBatteryType(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->batteryType;
    }
    else
    {
        out  = robOptions(robId)->batteryType;
    }
}

/**
 * Function to get the battery mission type
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {string} manualCharge or autoCharge depending on the robot model
 *
 * @public
 */
export function out = getBatteryMissionType(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->batteryMissionType;
    }
    else
    {
        out  = robOptions(robId)->batteryMissionType;
    }
}

/**
 * Function to know if the standard battery is used
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} -1 if LTO trucks only, 1 if lead trucks only, 0 if mixt
 *
 * @public
 */
export function out = isStandardBatt(robId)
{
    if (isRob)
    {
        robId = getRobId();
        out = same(getBatteryType(robId), 'Ld') ? 1 : -1;
    }
    else if (nargin == 1)
    {
        out = same(getBatteryType(robId), 'Ld') ? 1 : -1;
    }
    else
    {
        isLeadTruck = 0;
        isLTOTruck = 0;

        // Evaluate all trucks in the installation to find if there is at least one LTO truck / at least one Lead truck
        allRobotIds = lin(1,size(getPreferenceValue("general/trucksSerial")));
        k = for (allRobotIds)
        {
            if (same(getBatteryType(allRobotIds.(k)), "Ld"))
            {
                isLeadTruck = 1;
            }
            else
            {
                isLTOTruck = 1;
            }
        }

        if (!isLeadTruck && isLTOTruck)       out = -1;
        else if (isLeadTruck && !isLTOTruck)  out = 1;
        else                                  out = 0;
    }
}

/**
 * Function to compute the ID of the destination of the taxi mission
 *
 * @param {long] (optional) robot Id
 *
 * @return {long} ID in stkDB of the selected taxi point
 *
 * @public
 */
export function idstk = TAXI_chooseDestination(robId)
{
    if (nargin<1 && isRob)
        robId = getRobId();
    if (isFunc("TAXI_chooseDestination_cbk"))
    {
        idstk = acall("TAXI_chooseDestination_cbk", robId);
    }
    else
    {
        idstk = name2id(sprintf('taxi%i',robId));
    }
}

/**
 * Function to return the battery destination for given robot
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} id of point on the circuit
 *
 * @public
 */
export function dst = getBatteryDest(robId)
{
    if(nargin<1 && isRob())
    {
        robId = getRobId();
    }

    waitForInitialisationOfStkDB();

    defaultBattName = sprintf('batt%i',robId);
    battPoints = setInter(DBfind_gbl("stkDB", "type", "==", "batt"), DBfind_gbl("stkDB", "layerName", "==", myLayer()));

    if (name2id(defaultBattName) != -1)
    {
        dst = name2id(defaultBattName);
    }
    else if(size(battPoints))
    {
        dst = battPoints.(0);
    }
    else
    {
        dst = acall("TAXI_chooseDestination", robId);      // function always exist but is not available on kiwi
    }
}

/**
 * Function to know if the robot is equipped with a bar code reader
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a bar code reader, 0 otherwise
 *
 * @public
*/
export function out = isBarCode(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isBarCode;
    }
    else
    {
        out  = robOptions(robId)->isBarCode;
    }
}

/**
 * Function to know if the robot has a 3D camera curtain safety
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a 3D camera
 *
 * @public
 */
export function out = isCurtain3D(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isCurtain3D;
    }
    else
    {
        out  = robOptions(robId)->isCurtain3D;
    }
}

/**
 * Function to know if the robot has a fixed rear Laser
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a fixed rear Laser
 *
 * @public
 */
export function out = isFixedRearLaser(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isRearLaser;
    }
    else
    {
        out  = robOptions(robId)->isRearLaser;
    }
}

/**
 * Function to know if the robot has at least one laser below the initial lift
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if at least of of the rear laser is hidden when LI is down
 *
 * @public
 */
export function out = isSafetyLaserUnderInitialLift(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isSafetyLaserUnderInitialLift;
    }
    else
    {
        out  = robOptions(robId)->isSafetyLaserUnderInitialLift;
    }
}

/**
 * Function to know if the robot has at least one laser is forcing the forks to lift to free its field of vision
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if at least of of the rear laser is risking to see the backrest/load/forks if forks are too low and the truck is driving forward
 * @return {long} 1 if at least of of the rear laser is risking to see the backrest/load/forks if forks are too low and the truck is driving backward
 *
 * @public
 */
export function [whileDrivingForward, whileDrivingBackward, whileScanning] = isSoftSafetyBlindWhenForksDown(robId)
{
    if (nargin<1)
    {
        robOptionsVarspc = robOptions();
    }
    else
    {
        robOptionsVarspc = robOptions(robId);
    }

    whileDrivingForward  = robOptionsVarspc->isSoftSafetyBlindWhenForksDown->whileDrivingForward;
    whileDrivingBackward = robOptionsVarspc->isSoftSafetyBlindWhenForksDown->whileDrivingBackward;
    whileScanning        = robOptionsVarspc->isSoftSafetyBlindWhenForksDown->whileScanning;
}

/**
 * Function to know if the robot has a mobile Laser
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a mobile Laser
 *
 * @public
 */
export function out = isMobileLaser(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isMobileLaser;
    }
    else
    {
        out  = robOptions(robId)->isMobileLaser;
    }
}

/**
 * Function to know if the robot has an initial lift option
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is an initial lift, 0 otherwise
 *
 * @public
 */
export function out = isLIEnabled(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isLIEnabled;
    }
    else
    {
        out  = robOptions(robId)->isLIEnabled;
    }
}

/**
 * Function that returns 1 id the robot is equiped with a pair of inductive sensor to detect the initial lift state
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @retun {int} out 1 if there inductive sensors to detect initial lift state
 *
 * @public
 */
export function out = isInductiveSum(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isInductiveSum;
    }
    else
    {
        out  = robOptions(robId)->isInductiveSum;
    }
}

/**
 * Function that returns 1 id the robot is equiped with a pair of inductive sensor to detect the initial lift state
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @retun {int} out 1 if there inductive sensors to detect initial lift state
 *
 * @public
 */
export function out = isInitialLiftEstimator(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isInitialLiftEstimator;
    }
    else
    {
        out  = robOptions(robId)->isInitialLiftEstimator;
    }
}

/**
 * Function to know if the robot has a mast
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a mast actuator, 0 otherwise
 *
 * @public
 */
export function out = isMastEnabled(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isMastEnabled;
    }
    else
    {
        out  = robOptions(robId)->isMastEnabled;
    }
}

/**
 * Function to know if the robot has a side shift actuator
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a sideshift actuator, 0 otherwise
 *
 * @public
 */
export function out = isSideShiftEnabled(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isSideShiftEnabled;
    }
    else
    {
        out  = robOptions(robId)->isSideShiftEnabled;
    }
}

/**
 * Function to know if the robot has a side shift actuator that is inverted
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is an inverted sideshift actuator, 0 otherwise
 *
 * @public
 */
export function out = isSideShiftInverted(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isSideShiftInverted;
    }
    else
    {
        out  = robOptions(robId)->isSideShiftInverted;
    }
}

/**
 * Function to know if the robot has a rotating pair of forks
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a rotation actuator, 0 otherwise
 *
 * @public
 */
export function out = isRotationSideEnabled(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isRotationEnabled;
    }
    else
    {
        out  = robOptions(robId)->isRotationEnabled;
    }
}

/**
 * Function to know if the robot has a rotating pair of forks
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a rotation actuator, 0 otherwise
 *
 * @public
 */
export function out = isRotationFrontEnabled(robId)
{
    if (nargin<1)
    {
        out  = getPreferenceValue("forksActuators/rotationFront/enable", mySerial());
    }
    else
    {
        out  = getPreferenceValue("forksActuators/rotationFront/enable", mySerial(robId));
    }
}

/**
 * Function to know if the robot is equipped with a rangefinder pointing at the pallet feet
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a rangefinder capable of detecting the pallet with a pallet feet rangefinder, 0 otherwise
 *
 * @public
 */
export function out = isRangefinder(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isAnyRangefinder;
    }
    else
    {
        out  = robOptions(robId)->isAnyRangefinder;
    }
}

/**
 * Function to know if the robot is equipped with side lasers made to ensure the load is correctly located on the forks
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if truck has the correct side sensors, 0 otherwise
 *
 * @public
 */
export function out = isOverhang(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isOverhang;
    }
    else
    {
        out  = robOptions(robId)->isOverhang;
    }
}

/**
 * Function to know if the robot is equipped with side lasers made to prevent intrusion when the forks are down (rear laser is blind)
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if truck has the correct side sensors, 0 otherwise
 *
 * @public
 */
export function out = isAntiIntrusion(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isAntiIntrusion;
    }
    else
    {
        out  = robOptions(robId)->isAntiIntrusion;
    }
}

/**
 * Function to know if the robot is equipped with a pallet sensor against the backrest
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a pallet sensor, 0 otherwise
 *
 * @public
 */
export function out = isPalletSensor(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isPalletSensor;
    }
    else
    {
        out  = robOptions(robId)->isPalletSensor;
    }
}

/**
 * Function to know if the robot is equipped with a rangefinder sensor in between the forks, aiming at the pallet central feet
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a pallet sensor, 0 otherwise
 *
 * @public
 */
export function out = isCenterRangefinder(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isCenterRangefinder;
    }
    else
    {
        out  = robOptions(robId)->isCenterRangefinder;
    }
}

/**
 * Function to know if the robot is equipped with a pallet sensor on the right fork
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a pallet sensor, 0 otherwise
 *
 * @public
 */
export function out = isForkPalletSensorR(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isRCenterRangefinder;
    }
    else
    {
        out  = robOptions(robId)->isRCenterRangefinder;
    }
}

/**
 * Function to know if the robot is equipped with a pallet sensor on the left fork
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a pallet sensor, 0 otherwise
 *
 * @public
 */
export function out = isForkPalletSensorL(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isLCenterRangefinder;
    }
    else
    {
        out  = robOptions(robId)->isLCenterRangefinder;
    }
}

/**
 * Function to know if the robot is equipped with a mast camera
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a mast camera, 0 otherwise
 *
 * @public
 */
export function out = isMastCamera(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isMastCamera;
    }
    else
    {
        out  = robOptions(robId)->isMastCamera;
    }
}

/**
 * Function to know if the robot is equipped with a forks camera
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a forks camera, 0 otherwise
 *
 * @public
 */
export function out = isForksCamera(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isForksCamera;
    }
    else
    {
        out  = robOptions(robId)->isForksCamera;
    }
}

/**
 * Function to know if the robot is equipped with a safety laser on the forks
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if there is a safety laser on the forks, 0 otherwise
 *
 * @public
 */
export function out = isRearSafetyLaserOnTheForks(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isRearSafetyLaserOnTheForks;
    }
    else
    {
        out  = robOptions(robId)->isRearSafetyLaserOnTheForks;
    }
}

/**
 * Function to know if the robot uses the perception V2
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if the perception V2 is enabled, 0 otherwise
 *
 * @public
 */
export function out = isPerceptionV2Enabled(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isPerceptionV2Enabled;
    }
    else
    {
        out  = robOptions(robId)->isPerceptionV2Enabled;
    }
}

/**
 * Function to know if the robot is equiped with a RVB (Remote Vision Box) for Mowo features
 *
 * @param {long} (optional on robot side) robId id of the rob investigated
 *
 * @return {long} 1 if the RVB is equiped, 0 otherwise
 *
 * @public
 */
export function out = isEquipedWithRVB(robId)
{
    if (nargin<1)
    {
        out  = robOptions()->isEquipedWithRVB;
    }
    else
    {
        out  = robOptions(robId)->isEquipedWithRVB;
    }
}
