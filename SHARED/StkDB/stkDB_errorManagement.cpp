/**
 * Check the consistency of the defined mission
 *
 * @param {varspc} missVarspc Mission varspc
 *
 * @public
 */
export function checkCoherencyMission(missVarspc)
{
    // make sure the robot can perform the mission
    try
    {
        checkConsistencyMissVsRobot(missVarspc);
    }
    catch
    {
        if (strMatch(lastShortError(), "caseUnknown"))
        {
            translatedError = sprintf("%s", translate("caseUnknown"));
        }
        else
        {
            translatedError = sprintf("%s - Inconsistency between mission and robot\n%s", translate("genericCheckCoherencyError"), lastShortError());
        }
        if (isRob())
        {
            call("missSetManuErrMiss", {translatedError});
        }
        else
        {
            logError(translatedError);
        }
    }

    // make sure the mission can be executed on that destination point
    try
    {
        checkConsistencyMissVsDest(missVarspc);
    }
    catch
    {
        if (strMatch(lastShortError(), "caseUnknown"))
        {
            translatedError = sprintf("%s", translate("caseUnknown"));
        }
        else
        {
            translatedError = sprintf("%s - Inconsistency between mission and destination\n%s", translate("genericCheckCoherencyError"), lastShortError());
        }
        if (isRob())
        {
            call("missSetManuErrMiss", {translatedError});
        }
        else
        {
            logError(translatedError);
        }
    }
}

/**
 * Check consistency between mission type and destination where it's going to be performed
 *
 * @param {varspc} missVarspc Mission varspc
 *
 * @public
 */
export function checkConsistencyMissVsDest(missVarspc)
{
    typeMission = missVarspc->typeMission;
    missionName = missVarspc->missionName;
    pointData = missVarspc->pointData;

    // each mission has a type (or several) authorized
    switch(typeMission)
    {
        case("move");
            allowedPointTypes = strn("init", "taxi", "batt", "move");

        case("batt");
            switch(getPreferenceValue("battery/missionType", mySerial()))
            {
                case("manualCharge");
                    allowedPointTypes = strn("init", "taxi", "batt", "move");
                case("autoCharge");
                    allowedPointTypes = strn("batt");
            }

        case("taxi");
            allowedPointTypes = strn("init", "taxi", "move");

        case("pick", "drop");
            if (isVNA())
            {
                allowedPointTypes = strn("elevated");
            }
            else
            {
                allowedPointTypes = strn("line", "lineZone", "slot", "slotZone", "elevated");
            }

        case("pickFront", "dropFront");
            allowedPointTypes = strn("elevated");

        default;
            error("caseUnknown");

    }

    // if the type of point of the destination is not allowed prevent the mission to be run
    if (!size(findElt(allowedPointTypes, pointData->type)))
    {
        error(sprintf("mission %s cannot by executed at point %s because of wrong type (%s)", missionName, id2name(missVarspc->idstk), pointData->type));
    }

    // Ensure the right length of slot
    if (strMatch(missionName, "slot") && !strMatch(pointData->type, "slot"))
    {
        // last chance for allowing the mission : on elevated slots, we consider we can run the mission if at ground level
        if (pointData->heightTable.{0} == 0.0 && strMatch(pointData->type, "elevated"))
        {
        }
        else
        {
            error(sprintf("mission %s is not adapted to be run on destination %s (%s instead of slot)", missionName, id2name(missVarspc->idstk), pointData->type));
        }
    }
    if (strMatch(missionName, "line") && !strMatch(pointData->type, "line"))
    {
        error(sprintf("mission %s is not adapted to be run on destination %s (%s instead of line)", missionName, id2name(missVarspc->idstk), pointData->type));
    }

    // Ensure the VNA mission are only applied onto racks
    if (strMatch(missionName, "VNA") && !strMatch(pointData->type, "elevated"))
    {
        error(sprintf("mission %s is not adapted to be run on destination %s (%s instead of elevated)", missionName, id2name(missVarspc->idstk), pointData->type));
    }

    // ensure no permission mission is run on a slot without a sensor
    if (strMatch(missionName, "permission") && pointData->conveyorDeviceId == 0 && pointData->conveyorDevicePin == 0)
    {
        error(sprintf("mission %s is not adapted to be run on destination %s (no pallet sensor on this point)", missionName, id2name(missVarspc->idstk)));
    }
    // ensure no safetyFence mission is run on a slot without a safety fence
    if (strMatch(missionName, "safetyFence") && ((pointData->safetyFenceDeviceId == 0 && pointData->safetyFenceDevicePin == 0) || (pointData->safetyFenceEmergencyDeviceId == 0 && pointData->safetyFenceEmergencyDevicePin == 0)))
    {
        error(sprintf("mission %s is not adapted to be run on destination %s (no safety fence on this point)", missionName, id2name(missVarspc->idstk)));
    }

    // ensure the right size of scanlist
    if (strMatch(missionName, "multi") && size(missVarspc->scanList) < 2)
    {
        error(sprintf("mission %s is not adapted to be run on destination %s (scanlist too small for multi mission)", missionName, id2name(missVarspc->idstk)));
    }
    // Running a single line or single slot mission on a multi line group or multi slot group is fine
}

/**
 * Check consistency between mission type and capacity of the robot
 *
 * @param {varspc} missVarspc Mission varspc
 *
 * @public
 */
export function checkConsistencyMissVsRobot(missVarspc)
{
    serial = mySerial();

    truckType = getPreferenceValue("general/truckType");
    missionName = missVarspc->missionName;
    switch(truckType)
    {
        case("Tugger");
            switch(missVarspc->typeMission)
            {
                case("pick", "drop");
                    error(sprintf("mission %s cannot by executed on tugger", missionName));
                default;
                    // other missions (move, taxi, battery) are OK
            }

            // Ensure VNA missions are not executed by any other robot
            if (strMatch(missionName, "VNA"))
            {
                error(sprintf("mission %s (VNA) cannot by executed by a %s", missionName, truckType));
            }

        case("PalletTruck", "LongPalletTruck");
            if (strMatch(missionName, "elevated"))
            {
                error(sprintf("mission %s (elevated) cannot by executed by a %s", missionName, truckType));
            }

            // Ensure VNA missions are not executed by any other robot
            if (strMatch(missionName, "VNA"))
            {
                error(sprintf("mission %s (VNA) cannot by executed by a %s", missionName, truckType));
            }

        case("ReachTruck", "PalletStacker", "CounterbalancedPalletStacker");
            // Ensure VNA missions are not executed by any other robot
            if (strMatch(missionName, "VNA"))
            {
                error(sprintf("mission %s (VNA) cannot by executed by a %s", missionName, truckType));
            }

        case("VNA");
            if ((strMatch(missionName, "pick") || strMatch(missionName, "drop")) && !strMatch(missionName, "VNA") && !strMatch(missionName, "move"))
            {
                error(sprintf("mission %s cannot by executed by a %s", missionName, truckType));
            }

        default;
            error("caseUnknown");
    }

    // in case of battery mission, make sure it is consistent with the battery in the truck
    if (strMatch(missionName, "batt"))
    {
        expectedMissionType = getBatteryMissionType();

        if (!strMatch(missionName, expectedMissionType))
        {
            error(sprintf("Battery in truck is expecting battery mission to be %sd", expectedMissionType));
        }
    }

    // make sure the right equipments are present on the truck
    if (strMatch(missionName, "scan"))
    {
        // truck cannot be sent on a 3D mission if not equipped with a 3D camera
        if (strMatch(missionName, "3D"))
        {
            if (!isMastCamera() && !isForksCamera())
            {
                error(sprintf("Truck is not equipped with any 3D camera : cannot perform mission %s", missionName));
            }
        }
        // truck cannot be sent on a scan mission if no laser is declared as such
        else
        {
            if (!size(getPreferenceValue("detection/module", serial)))
            {
                error(sprintf("Truck has no scanning laser declared : cannot perform mission %s", missionName));
            }
        }
    }

    if (strMatch(missionName, '3D') && isPerceptionV2Enabled())
    {
        if(!acall("checkRoSeConfFiles")) //Checking if the robot is ready to use perception in a mission
        {
            error(sprintf("ERROR : Can not use perception (a file is missing)."));
        }
    }
}

/**
 * Check consistency between mission type and capacity of the robot
 *
 * @public
 */
export function checkConsistencyStkDB()
{
    allKeys = DBgetAllKeys_gbl("stkDB");

    //check the error mode in case of full slot
    allErrorModeScanFull = unique(DBget_gbl("stkDB", allKeys, "errorModeScanFull"));
    if( min(allErrorModeScanFull) <= 0 || max(allErrorModeScanFull) > 4)
    {
        error("Error mode in case of full slot is not cover in the missions ( Value = %d )", allErrorModeScanFull);
    }

    //check the error mode in case of incorrect scan status for slot
    allErrorModeScanPickFail = unique(DBget_gbl("stkDB", allKeys, "errorModeScanNotPossiblePick"));
    if( min(allErrorModeScanPickFail) <= 0 || max(allErrorModeScanPickFail) > 5)
    {
        error("Error mode in case of incorrect scan status is not cover in the missions ( Value = %d )", allErrorModeScanPickFail);
    }

    //check the error mode in case of incorrect scan status for slot
    allErrorModeScanDropFail = unique(DBget_gbl("stkDB", allKeys, "errorModeScanNotPossibleDrop"));
    if( min(allErrorModeScanDropFail) <= 0 || max(allErrorModeScanDropFail) > 5)
    {
        error("Error mode in case of incorrect scan status is not cover in the missions ( Value = %d )", allErrorModeScanDropFail);
    }

    //check the error mode in case of empty slot
    allErrorModeScanEmpty = unique(DBget_gbl("stkDB", allKeys, "errorModeScanEmpty"));
    if( min(allErrorModeScanEmpty) <= 0 || max(allErrorModeScanEmpty) > 5)
    {
        error("Error mode in case of empty slot is not cover in the missions ( Value = %d )", allErrorModeScanEmpty);
    }

    //check the error mode in case of empty slot
    allErrorModeScanRevEmpty = unique(DBget_gbl("stkDB", allKeys, "errorModeReverseScanEmpty"));
    if( min(allErrorModeScanRevEmpty) <= 0 || max(allErrorModeScanRevEmpty) > 3)
    {
        error("Error mode in case of empty slot is not cover in the missions ( Value = %d )", allErrorModeScanRevEmpty);
    }

    //check the error mode in case of abscence of barcode
    allErrorModeBarcodeNotFound = unique(DBget_gbl("stkDB", allKeys, "errorModeBarcodeNotFound"));
    if( min(allErrorModeBarcodeNotFound) <= 0 || max(allErrorModeBarcodeNotFound) > 3)
    {
        error("Error mode in case of barcode absence is not cover in the missions ( Value = %d )", allErrorModeBarcodeNotFound);
    }

    //check the error mode in case of barcode wrong
    allErrorModeBarcodeWrong = unique(DBget_gbl("stkDB", allKeys, "errorModeBarcodeWrong"));
    if( min(allErrorModeBarcodeWrong) <= 0 || max(allErrorModeBarcodeWrong) > 3)
    {
        error("Error mode in case of wrong barcode is not cover in the missions ( Value = %d )", allErrorModeBarcodeWrong);
    }

    //check the error mode in case of obstacle at rpkApproach
    allObstacleErrorMode = unique(DBget_gbl("stkDB", allKeys, "obstacleErrorMode"));
    if( min(allObstacleErrorMode) <= 0 || max(allObstacleErrorMode) > 3)
    {
        error("Error mode in case of obstacle at rpkApproach is not cover in the missions ( Value = %d )", allObstacleErrorMode);
    }

    //check the error mode in case no pallet is detected
    allErrorModeNoPalOnForks = unique(DBget_gbl("stkDB", allKeys, "errorModeNoPalOnForks"));
    if( min(allErrorModeNoPalOnForks) <= 0 || max(allErrorModeNoPalOnForks) > 2)
    {
        error("Error mode in case no pallet is detected is not cover in the missions ( Value = %d )", allErrorModeNoPalOnForks);
    }

    //check the error mode in case of traction blocked (pick)
    allErrorModeTBPick = unique(DBget_gbl("stkDB", allKeys, "errorModeTractionBlockedPick"));
    if( min(allErrorModeTBPick) <= 0 || max(allErrorModeTBPick) > 4)
    {
        error("Error mode in case traction blocked (pick) is not cover in the missions ( Value = %d )", allErrorModeTBPick);
    }
    //check the error mode in case of traction blocked (drop)
    allErrorModeTBDrop = unique(DBget_gbl("stkDB", allKeys, "errorModeTractionBlockedDrop"));
    if( min(allErrorModeTBDrop) <= 0 || max(allErrorModeTBDrop) > 4)
    {
       error("Error mode in case traction blocked (drop) is not cover in the missions ( Value = %d )", allErrorModeTBDrop);
    }

    //check the error mode in case of slot occupied
    allErrorModeConveyorNotFree = unique(DBget_gbl("stkDB", allKeys, "errorModeConveyorNotFree"));
    if( min(allErrorModeConveyorNotFree) <= 0 || max(allErrorModeConveyorNotFree) > 5)
    {
        error("Error mode in case slot occupied is not cover in the missions ( Value = %d )", allErrorModeConveyorNotFree);
    }

    //check the error mode in case of slot occupied
    allErrorModeElevatedControl = unique(DBget_gbl("stkDB", allKeys, "errorModeElevatedControl"));
    if( min(allErrorModeElevatedControl) <= 0 || max(allErrorModeElevatedControl) > 5)
    {
        error("Error mode in case slot occupied is not cover in the missions ( Value = %d )", allErrorModeElevatedControl);
    }

    //check the error mode in case of empty conveyor
    allErrorModeConveyorEmpty = unique(DBget_gbl("stkDB", allKeys, "errorModeConveyorEmpty"));
    if( min(allErrorModeConveyorEmpty) <= 0 || max(allErrorModeConveyorEmpty) > 5)
    {
        error("Error mode in case empty conveyor is not cover in the missions ( Value = %d )", allErrorModeConveyorEmpty);
    }

    //check the error mode in case of safety fence timeout
    allErrorsModeSafetyFence1 = unique(DBget_gbl("stkDB", allKeys, "errorModeTMOOpeningSafetyFence"));
    if( min(allErrorsModeSafetyFence1) <= 0 || max(allErrorsModeSafetyFence1) > 2)
    {
        error("Error mode in case safety fence timeout is not cover in the missions ( Value = %d )", allErrorsModeSafetyFence1);
    }

    //check the error mode in case of safety fence triggered
    allErrorsModeSafetyFence2 = unique(DBget_gbl("stkDB", allKeys, "errorModeTriggerSafetyFence"));
    if( min(allErrorsModeSafetyFence2) <= 0 || max(allErrorsModeSafetyFence2) > 1)
    {
        error("Error mode in case safety fence triggered is not cover in the missions ( Value = %d )", allErrorsModeSafetyFence2);
    }

    //check the existence of the fillStrategy
    allFillStrategies = unique(DBget_gbl("stkDB", allKeys, "fillStrategy"));
    if( min(allFillStrategies) <= 0 || max(allFillStrategies) > 6)
    {
        error("Wrong fillStrategy ( Value = %d )", allFillStrategies);
    }
}
