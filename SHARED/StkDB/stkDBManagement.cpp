/**
 * Function called ont he robot and the robot manager to load stkDB
 *
 * @return {long} 1 if the stkDB has been loaded
 *
 * @public
 */
export function res = loadStkDB()
{
    PALLET_init();

    res = 1;
    // Add all standard data
    addAllDestinationPoints();
    logInstall('[loadStkDB] Finished filling destination points');
    addAllStockZones();
    logInstall('[loadStkDB] Finished filling stock zones');
    addAllRackPallet();
    logInstall('[loadStkDB] Finished filling racks');
    addVirtualPoints();
    logInstall('[loadStkDB] Finished filling stkDB');
}

/**
 * Function to get the mission option from point ID
 *
 * @param {long} pointId ID of the point
 *
 * @return {string} missionOption Mission option
 *
 * @public
 */
export function missionOption = getMissionOptionFromPointId(pointId, robId)
{
    if (nargin<2 && isRob())
        robId = getRobId();

    pointData = missPntGetInfo(pointId);
    detectionUsed = pointData->rackCell.detectionUsed;

    // Category VNA
    if (isVNA(robId))
    {
        if (strMatch(pointData->type, "elevated"))
        {
            if (isPerceptionV2Enabled(robId) || isForksCamera(robId) || isMastCamera(robId))
            {
                missionOption = "VNALateral3D";
            }
            else
            {
                missionOption = "VNALateral";
            }
        }
        else
        {
            if (isPerceptionV2Enabled(robId) || isForksCamera(robId) || isMastCamera(robId))
            {
                missionOption = "VNAFrontal3D";
            }
            else
            {
                missionOption = "VNAFrontal";
            }
        }
    }

    // Category Elevated (camera must be used on circuit editor and truck is capable)
    else if ((detectionUsed.pallet3D == 1 || detectionUsed.beam3D == 1 || detectionUsed.freeSpace3D == 1)
             && (isForksCamera(robId) || isMastCamera(robId)) )
    {
        missionOption = "Elevated3D";
    }

    else if (strcmp(pointData->type, strn('elevated')) == 0 || pointData->heightTable.{0} != 0)
    {
        if (pointData->safetyFenceDevicePin != 0)
        {
            missionOption = "ElevatedSafetyFence";
        }
        else if (pointData->conveyorDevicePin != 0)
        {
            missionOption = "ElevatedPermission";
        }
        else
        {
            missionOption = "Elevated";
        }
    }

    // Category multiple line
    else if (size(pointData->scanList.{0}) > 1)
    {
        if (size(pointData->slotEmplacement.{0}) > 1)
        {
            missionOption = "MultiLineScan";
        }
        else
        {
            missionOption = "MultiSlotScan";
        }
    }

    // Category single line / slot
    else
    {
        if (size(pointData->slotEmplacement.{0}) > 1)
        {
            if (pointData->scan_sick == 1)
            {
                missionOption = "SingleLineScan";
            }
            else
            {
                missionOption = "SingleLine";
            }
        }
        else
        {
            if (pointData->scan_sick == 1)
            {
                missionOption = "SingleSlotScan";
            }
            else
            {
                missionOption = "SingleSlot";
            }
        }
    }
}
