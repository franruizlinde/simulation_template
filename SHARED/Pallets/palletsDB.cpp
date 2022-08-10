/**
 * Function to load all pallets defined in preferences into a DB to make it accessible in project code
 *
 * @public
 */
export function PALLET_init(robId)
{
    if (nargin < 1)
    {
        if (isRob())    robId = getRobId();
        else            robId = 1;
    }

    serial = mySerial(robId);

    global palletDBLoaded ("palletDBLoaded");
    unint palletDBLoaded->loaded = 0;

    // read pallet definitions
    palletDefinitionVarspc                           = varspc();
    palletDefinitionVarspc->pocketHeight             = getPreferenceValue("detection/pallet3D/pallets/centralGapHeight", "INSTALL");
    palletDefinitionVarspc->palletBoardThickness     = getPreferenceValue("detection/pallet3D/pallets/bottomDeckBoardThickness", "INSTALL");
    palletDefinitionVarspc->palletWidths             = getPreferenceValue("detection/pallet3D/pallets/palletWidth", "INSTALL");

    // read truck geometry
    truckConfVarspc                                  = varspc();
    truckConfVarspc->forksThickness                  = getPreferenceValue("forksParameters/verticalForksThickness", serial);
    truckConfVarspc->toleranceUp                     = abs(getPreferenceValue("forksControllers/verticalForks/genericForksController/parameters/toleranceUp", serial));
    truckConfVarspc->toleranceDown                   = abs(getPreferenceValue("forksControllers/verticalForks/genericForksController/parameters/toleranceDown", serial));
    truckConfVarspc->robModel                        = getPreferenceValue("general/modelFileName", serial);

    allPalletNames                                   = getPreferenceValue("detection/pallet3D/pallets/names", "INSTALL");

    //////////////////////////////////////////////////
    // Step 1 : define the DB
    indexes = lin(0, size(allPalletNames) - 1);
    palletDef = loadNewPalletEntry(strn("All"), 0, indexes, palletDefinitionVarspc, truckConfVarspc);

    DBcreate_gbl("palletDB", palletDef, "id", 0);
    DBdel_gbl("palletDB", DBgetAllKeys_gbl("palletDB"));
    if (isRob && !isMastEnabled(robId))
    {
        logInstall("[PALLET] - no need to load the palletDB as truck is not equipped with a mast.");
        return;
    }

    //////////////////////////////////////////////////
    // Step 2 : populate with individual pallets
    k = for (allPalletNames)
    {
        singlePallet = loadNewPalletEntry(allPalletNames.(k), k+1, k, palletDefinitionVarspc, truckConfVarspc);
        DBadd_gbl("palletDB", singlePallet);
    }

    //////////////////////////////////////////////////
    // Step 3 : populate with groups of pallets
    palletGroups             = getPreferenceValue("detection/pallet3D/palletGroup", "INSTALL");
    palletGroupAffiliations  = getPreferenceValue("detection/pallet3D/pallets/groupAffiliation", "INSTALL");
    k = for (palletGroups)
    {
        indexes = [];
        j = for (palletGroupAffiliations)
        {
            isInGroup = strcmp(char(palletGroupAffiliations.(j)).(k), "1") == 0 ? 1 : 0;
            if (isInGroup)  indexes.(end+1) = j;
        }

        if (size(indexes))
        {
            singleGroup = loadNewPalletEntry(palletGroups.(k), -k-1, indexes, palletDefinitionVarspc, truckConfVarspc);
            DBadd_gbl("palletDB", singleGroup);
        }
    }

    unint palletDBLoaded->loaded = robId;
}

export function palletDef = loadNewPalletEntry(name, id, indexes, dimensionVarspc, truckConfVarspc)
{
    palletDef   = varspc();
    palletDef->id                   = id;   // groups are using negatives keys
    palletDef->name                 = name;

    if (size(indexes))
    {
        topOfPocketHeight               = min(dimensionVarspc->palletBoardThickness.(indexes) + dimensionVarspc->pocketHeight.(indexes));
        bottomOfPocketHeight            = max(dimensionVarspc->palletBoardThickness.(indexes));

        palletDef->lowestPocketHeight   = bottomOfPocketHeight;
        palletDef->highestPocketHeight  = topOfPocketHeight;

        palletDef->palletHeightOffset   = bottomOfPocketHeight + (topOfPocketHeight - bottomOfPocketHeight)/2 + truckConfVarspc->forksThickness/2;
        palletDef->width                = max(dimensionVarspc->palletWidths.(indexes));
        palletDef->palletTotalHeight    = max(dimensionVarspc->pocketHeight.(indexes) + dimensionVarspc->palletBoardThickness.(indexes) + max(0.002, dimensionVarspc->palletBoardThickness.(indexes))); // top deackboard thickness is not known : we assume it's = to bootom one or 2cm

        checkValidity(palletDef, truckConfVarspc);
    }
    else
    {
        topOfPocketHeight   = 0.1;
        bottomOfPocketHeight  = 0.0;

        palletDef->lowestPocketHeight   = bottomOfPocketHeight;
        palletDef->highestPocketHeight  = topOfPocketHeight;

        palletDef->palletHeightOffset   = bottomOfPocketHeight + (topOfPocketHeight - bottomOfPocketHeight)/2 + truckConfVarspc->forksThickness/2;
        palletDef->width                = 0.8;
        palletDef->palletTotalHeight    = topOfPocketHeight + 0.04;
    }
}

/**
 * Triggers a warning if a truck cannot handle a special pallet or group of pallet.
 *
 * @public
 */
export function [isValid, reason] = checkValidity(palletDef, truckConfVarspc)
{
    isValid = 1;
    reason = 0;

    availableSpaceH = max(palletDef->highestPocketHeight - palletDef->lowestPocketHeight, 0);
    requiredSpaceH  = truckConfVarspc->forksThickness + truckConfVarspc->toleranceDown + truckConfVarspc->toleranceUp;
    if (availableSpaceH < requiredSpaceH)
    {
        isValid = 0;
        reason = 1;
    }

//        if (widthIssue)

//        if (inBlindZone)

    palletDef->isValid = isValid;
}

/**
 * Convert the name of a group into an ID (to be sent in radio events)
 *
 * @param {string} name of a group of pallet (must be found in pallets/palletGroup)
 *
 * @return {long} ID corresponding to the group
 *
 * @public
 */
export function palletID = PALLET_name2id(palletOrGroupName)
{
    palletID = 0;
    if (size(palletOrGroupName))
    {
        keys = DBfind_gbl("palletDB", "name", "==", palletOrGroupName);
        if (size(keys))
        {
            palletID = keys.(0);
        }
    }
}

/**
 * Convert the ID of a group into its name (to understand a radio events)
 *
 * @param {long} ID corresponding to the group
 *
 * @return {string} name of a group of pallet (must be found in pallets/palletGroup)
 *
 * @public
 */
export function palletOrGroupName = PALLET_id2name(palletID)
{
    if (size(find(DBgetAllKeys_gbl("palletDB") == palletID)))
    {
        palletOrGroupName = DBget_gbl("palletDB", palletID, "name");
    }
    else
    {
        palletOrGroupName = DBget_gbl("palletDB", 0, "name");
    }
}

/**
 * return the pallet design if provided its id
 *
 * @param {long} pallet Id
 *
 * @return {varspc} pallet design (if id was found, else, default pallet design)
 *
 * @public
 */
export function pal = PALLET_getPal(palletIdOrName, fieldName)
{
    if (isNumeric(palletIdOrName))
    {
        palletID = palletIdOrName;
    }
    else
    {
        palletID = PALLET_name2id(palletIdOrName);
    }

    if (nargin < 2)
        pal = DBget_gbl("palletDB", palletID);
    else
        pal = DBget_gbl("palletDB", palletID, fieldName);
}
