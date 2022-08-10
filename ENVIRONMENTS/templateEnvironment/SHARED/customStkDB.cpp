/**
 * Function to edit the list of fields in stkDB
 *
 * @return {varspc} list of extra fields with their default values
 *
 * @public
 */
export function stkDBCustomFields = stkDB_customDef()
{
    stkDBCustomFields = varspc();
    /*
        // Here is an example of how adding fields to the stkDB
        stkDBProjectSpecificFields->customField1 = strn('');      // Char
        stkDBProjectSpecificFields->customField2 = 0;             // Int
        stkDBProjectSpecificFields->customField1 = {[]};          // Array of values
    */
}

/**
 * Function to edit the stkDB before it is fully loaded for custom project code
 *
 * @param {varspc} pointData pointData before edit
 *
 * @return {varspc} pointData after edit
 *
 * @public
 */
export function pointData = stkDB_alter(pointData)
{
    pointID = pointData->id;
    pointName = char(pointData->name);

    // Modify the type of point for unhitch
    if (!strcmp(pointData->type, 'move') && strMatch(pointName, 'unhitch'))
    {
        pointData->actionEndTask = 5;
    }

    if (pointData->npalsx > 6)
    {
        pointData->activateScanInReverse = 1;
    }
}

/**
 * Function used to complete the filling of stkDB by adding virtual points
 * Can be used to duplicate points with new names, turn stockzones or destination points into rackscells or so...
 *
 * @public
 */
export function addVirtualPoints() {}