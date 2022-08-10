///////////////////// TRUCK TYPES ////////////////////////////////////////
/**
 * Function to know if the robot is a tugger
 *
 * @param {long} robId id of the rob investigated
 *
 * @return {long} 1 if truck is a tugger, 0 otherwise
 *
 * @public
 */
export function out = isTugger(robId)
{
    if (nargin <1)
        out = robType()->isTugger;
    else
        out = robType(robId)->isTugger;
}

/**
 * Function to get the list of all tuggers of the installation
 *
 * @return {long} list of rob Ids
 *
 * @public
 */
export function robList = getAllTuggers()
{
    robList = [];
    if (isPc)   allRobs = acall("pcGetAllRob");
    else        allRobs = lin(1, size(getPreferenceValue("general/trucksSerial", "INSTALL")));
    k = for (allRobs)
    {
        if (isTugger(allRobs.(k)))  robList.(end+1) = allRobs.(k);
    }
}

/**
 * Function to know if the robot is a pallet stacker
 *
 * @param {long} robId id of the rob investigated
 *
 * @return {long} 1 if truck is a pallet stacker, 0 otherwise
 *
 * @public
 */
export function out = isPalletStacker(robId)
{
    if (nargin <1)
        out = robType()->isPalletStacker;
    else
        out = robType(robId)->isPalletStacker;
}

/**
 * Function to get the list of all pallet stackers of the installation
 *
 * @return {long} list of rob Ids
 *
 * @public
 */
export function robList = getAllPalletStackers()
{
    robList = [];
    if (isPc)   allRobs = acall("pcGetAllRob");
    else        allRobs = lin(1, size(getPreferenceValue("general/trucksSerial", "INSTALL")));
    k = for (allRobs)
    {
        if (isPalletStacker(allRobs.(k)))  robList.(end+1) = allRobs.(k);
    }
}

/**
 * Function to know if the robot is a Pallet Truck
 *
 * @param {long} robId id of the rob investigated
 *
 * @return {long} 1 if the robot is a Pallet Truck, 0 otherwise
 *
 * @public
 */
export function out = isPalTruck(robId)
{
    if (nargin <1)
        out = robType()->isPalTruck;
    else
        out = robType(robId)->isPalTruck;
}

/**
 * Function to know if the robot is counter balanced
 *
 * @param {long} robId id of the rob investigated
 *
 * @return {long} 1 if the robot is a counter balanced, 0 otherwise
 *
 * @public
 */
export function out = isCounterBalanced(robId)
{
    if (nargin<1)
        out = strMatch(getPreferenceValue('general/truckType', mySerial()), 'counterBalanced');
    else
        out = strMatch(getPreferenceValue('general/truckType', mySerial(robId)), 'counterBalanced');
}

/**
 * Function to get the list of all pallet trucks of the installation
 *
 * @return {long} list of rob Ids
 *
 * @public
 */
export function robList = getAllPalTrucks()
{
    robList = [];
    if (isPc)   allRobs = acall("pcGetAllRob");
    else        allRobs = lin(1, size(getPreferenceValue("general/trucksSerial", "INSTALL")));
    k = for (allRobs)
    {
        if (isPalTruck(allRobs.(k)))  robList.(end+1) = allRobs.(k);
    }
}

/**
 * Function to know if the robot is a reach Truck
 *
 * @param {long} robId id of the rob investigated
 *
 * @return {long} 1 if the robot is a reach Truck, 0 otherwise
 *
 * @public
 */
export function out = isReachTruck(robId)
{
    if (nargin <1)
        out = robType()->isReachTruck;
    else
        out = robType(robId)->isReachTruck;
}

/**
 * Function to get the list of all reachTrucks of the installation
 *
 * @return {long} list of rob Ids
 *
 * @public
 */
export function robList = getAllReachTrucks()
{
    robList = [];
    if (isPc)   allRobs = acall("pcGetAllRob");
    else        allRobs = lin(1, size(getPreferenceValue("general/trucksSerial", "INSTALL")));
    k = for (allRobs)
    {
        if (isReachTruck(allRobs.(k)))  robList.(end+1) = allRobs.(k);
    }
}

/**
 * Function to know if the robot is a VNA (very Narrow Aisle) Truck
 *
 * @param {long} robId id of the rob investigated
 *
 * @return {long} 1 if the robot is a VNA Truck, 0 otherwise
 *
 * @public
 */
export function out = isVNA(robId)
{
    if (nargin <1)
        out = robType()->isVNA;
    else
        out = robType(robId)->isVNA;
}

/**
 * Function to get the list of all VNA trucks of the installation
 *
 * @return {long} list of rob Ids
 *
 * @public
 */
export function robList = getAllVNA()
{
    robList = [];
    if (isPc)   allRobs = acall("pcGetAllRob");
    else        allRobs = lin(1, size(getPreferenceValue("general/trucksSerial", "INSTALL")));
    k = for (allRobs)
    {
        if (isVNA(allRobs.(k)))  robList.(end+1) = allRobs.(k);
    }
}

/**
 * Function to return all trucks ID from a given model
 *
 * @param {name} robModel name of the model (as in pref file)
 *
 * @return {L.long} List of all robots from the installation sharing this robot model
 *
 * @public
 */
export function robList = pcGetAllRobModel(robModel)
{
    robList = [];
    if (isPc)   allRobs = acall("pcGetAllRob");
    else        allRobs = lin(1, size(getPreferenceValue("general/trucksSerial", "INSTALL")));
    k = for (allRobs)
    {
        if (!strcmp(getPreferenceValue("general/modelFileName", number2Serial(allRobs.(k))), robModel))
        {
            robList.(end+1) = allRobs.(k);
        }
    }
}

/**
 * Function to return all trucks models existing in your installation
 *
 * @return {L.name} List of all robots model names
 *
 * @public
 */
export function modelNameList = pcGetAllModels()
{
    modelNameList = [];
    if (isPc)   allRobs = acall("pcGetAllRob");
    else        allRobs = lin(1, size(getPreferenceValue("general/trucksSerial", "INSTALL")));
    k = for (allRobs)
    {
        if (!size(findElt(modelNameList, getPreferenceValue("general/modelFileName", number2Serial(allRobs.(k))))))
        {
            modelNameList.(end+1) = strn(getPreferenceValue("general/modelFileName", number2Serial(allRobs.(k))));
        }
    }
}