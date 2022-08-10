/**
 * Permet d'ajouter des champs Ã  la stkDB
 *
 * @return {varspc} stkDB Definition (all fields at default values)
 *
 * @public
 */
export function v = stkDBDef()
{
    evacMargin = 0.2;

    errorModeCam3DVar = varspc(
        // drop only errors                     // 1 : AGV goes into error
        "beamNotSeen",                      1,  // 2 : AGV goes to anomaly location
        "freeSpaceTooMuchSideShift",        1,
        "slotNotFree",                      1,
        "obstacleDetected",                 1,
        "slotNotEnoughSpace",               1,
        "freeSpaceNotComplete",             1,

        // pick only errors                     // 1 : AGV goes into error
        "palletTooMuchSideShift",           1,  // 2 : mission is canceled
        "cannotDetectPallet",               1,
        "palletOrientationNOK",             1,
        "secondPalletDetected",             1,

        // common errors                        // 1 : Agv goes into error
        "unknownDetectionAlgo",             1,  // 2 : in pick missions : cancel
        "cameraNotStarted",                 1,  // 2 : in drop missions : go to anomaly
        "noResultsTMO",                     1,
        "diffCircuit3D",                    1,
        "retractNotIn",                     1,

        // switch case which isn't an error     // 1 : AGV goes into error
        "wrongPalletDetected",              3,  // 2 : cancel mission
                                                // 3 : Trust camera results over expected results
                                                // 4 : trust expected results over camera results
    );

    v = varspc(
        "id",                       0,
        "name",                     strn(""),
        "zstart",                   new('point', 1),
        "zend",                     new('point', 1),
        "portionId",                0,
        "npalsx",                   0,

        "pal",                      {[]},

        "type",                     strn(""),
        "actionEndTask",            0,
        "actionEndTaskParam",       0,
        "heightTable",              {[0.0]},
        "VMAX_REVERSE",             getPreferenceValue("/trajectoryDriver/speedLimit/maxBackwardSpeed", mySerial(), 0),
        "VMAX_REVERSE_SLOW",        0.25,
        "VMAX_NORMAL",              getPreferenceValue("/trajectoryDriver/speedLimit/maxForwardSpeed", mySerial(), 0),
        "VMAX_NORMAL_SLOW",         getPreferenceValue("/trajectoryDriver/speedLimit/vmaxEvac", mySerial(), 0),

        "scan_sick",                1,
        "scanList",                 {[]},
        "evacDst",                  abs(DIST_BACKREST_CARRYFIELD()) + evacMargin,

        "layerName",                strn("Common"),
        "groupName",                strn(""),

        'barcodeUsedForMiss'                ,0,                  // Utility: Use or not barcode for miss  |  Value: 0 or 1
        'adaptPickRpkDestWithPalSensor'     ,0,                  // Are we allowed to correct rpkDestination using the pallet sensor
        'maxDistanceObjDetect'              ,0.2,                // distance to the original rpkDestination where truck is allowed to stop early (with pallet sensor or traction blocked)

        'safetyPointMargin'                 ,0.12,               // Utility: Distance between the end of safety field and the pallet to cut the safety  | Value: Distance in meter
        'distEndSafetyDuringDrop'           ,0.,                 // how far behind the pallet is the safety
        'liftHeightOffset'                  ,getPreferenceValue("palletParameters/palletPickupHeight", mySerial(), 0),
                                                                 // offset to add to the position pallet for elevated dropOff/pick mission --> in meter

        'errorModeBarcodeWrong'             ,1,                  // defined behavior if barcode read on the pallet is not the expected one
                                                                 // possible values : 1 --> AGV in error mode
                                                                 //                   2 --> cancel mission
                                                                 //                   3 --> pick the pallet and bring it to anomaly
                                                                 //                   4 --> retry reading
        'errorModeBarcodeNotFound'          ,1,                  // defined behavior if no barcode can be found on the pallet
                                                                 // possible values : 1 --> AGV in error mode
                                                                 //                   2 --> cancel mission
                                                                 //                   3 --> pick the pallet and bring it to anomaly
                                                                 //                   4 --> retry reading
        'obstacleErrorMode'                 ,1,                  // for elevated drop if there is an obstacle that need the backward security to be disabled
                                                                 // possible values : 1 -> AGV goes in error
                                                                 //                   2 -> AGV goes to anomaly (if drop) / cancel the mission (if pick)
                                                                 //                   3 -> AGV waits until the obstacle is removed
        'errorModeScanFull'                 ,1,                  // defined behavior in case of line/slot scan full during dropOff/pick mission
                                                                 // possible values : 1 --> AGV in error mode if scan full
                                                                 //                   2 --> go to anomaly
                                                                 //                   3 --> Re-run the same mission
                                                                 //                   4 --> Custom Case (to be defined in missionCallbacks)
        'errorModeScanNotPossiblePick'      ,1,                  // defined behavior in case of bad result for scan during pick mission
                                                                 // possible values : 1 --> AGV in error
                                                                 //                   2 --> Cancel the mission
                                                                 //                   3 --> Continue the mission anyway
                                                                 //                   4 --> Re-run the same mission
                                                                 //                   5 --> Custom Case (to be defined in missionCallbacks)
        'errorModeScanNotPossibleDrop'      ,1,                  // defined behavior in case of bad result for scan during dropOff mission
                                                                 // possible values : 1 --> AGV in error
                                                                 //                   2 --> go to anomaly
                                                                 //                   3 --> continue anyway
                                                                 //                   4 --> Re-run the same mission
                                                                 //                   5 --> Custom Case (to be defined in missionCallbacks)
        'errorModeScanEmpty'                ,1,                  // defined behavior in case of line/slot scan empty during pick mission
                                                                 // possible values : 1 --> AGV in error mode if scan empty
                                                                 //                   2 --> Mission is canceled
                                                                 //                   3 --> Continue anyway
                                                                 //                   4 --> Re-run the same mission
                                                                 //                   5 --> Custom Case (to be defined in missionCallbacks)
        'errorModeReverseScanEmpty'         ,1,                  // defined behavior in case of line/slot reverse scan empty during pick mission
                                                                 // possible values : 1 --> AGV in error mode if scan empty
                                                                 //                   2 --> Mission is canceled
                                                                 //                   3 --> Custom Case (to be defined in missionCallbacks)
        'errorModeNoPalOnForks'             ,1,                  // defined behavior if the pallet is not detected by the presence sensor
                                                                 // possible values : 1 --> AGV in error mode
                                                                 //                   2 --> Mission is canceled
        'errorModeTractionBlockedPick'      ,1,                  // defined behavior if traction blocked near pickup position
                                                                 // possible values : 1 --> do nothing
                                                                 //                   2 --> end the mission there (change rpkDest)
                                                                 //                   3 --> AGV in error mode (not the same threshold as regular traction blocked)
                                                                 //                   4 --> Mission is canceled
        'errorModeTractionBlockedDrop'      ,1,                  // defined behavior if traction blocked near dropoff position
                                                                 // possible values : 1 --> do nothing
                                                                 //                   2 --> end the mission there (change rpkDest)
                                                                 //                   3 --> AGV in error mode (not the same threshold as regular traction blocked)
                                                                 //                   4 --> Go to anomaly
        'errorModeConveyorNotFree'          ,1,                  // defined behavior if sensor connected to the combox does not return the expected value
                                                                 // possible values : 1 --> AGV in error mode
                                                                 //                   2 --> AGV goes to anomaly
                                                                 //                   3 --> AGV waits until conveyor is free
                                                                 //                   4 --> same as case 3 with TMO into error
                                                                 //                   5 --> same as case 3 with TMO into anomaly
        'errorModeElevatedControl'          ,1,                  // defined behavior if elevated position happens to be not free
                                                                 // possible values : 1 --> AGV in error mode
                                                                 //                   2 --> AGV goes to anomaly
                                                                 //                   3 --> AGV waits until conveyor is free
                                                                 //                   4 --> same as case 3 with TMO into error
                                                                 //                   5 --> same as case 3 with TMO into anomaly
        'errorModeConveyorEmpty'            ,1,                  // defined behavior if sensor connected to the combox does not return the expected value
                                                                 // possible values : 1 --> AGV in error mode
                                                                 //                   2 --> cancel mission
                                                                 //                   3 --> AGV waits until conveyor is occupied
                                                                 //                   4 --> same as case 3 with TMO into error
                                                                 //                   5 --> same as case 3 with TMO into cancel
        'errorModeTMOOpeningSafetyFence'    ,1,                  // defined behavior if safety fence is triggered
                                                                 // possible values : 1 --> AGV in error mode
                                                                 //                   2 --> AGV goes to anomaly (drop) or cancel mission (pick)
        'errorModeTriggerSafetyFence'       ,1,                  // defined behavior if safety fence is triggered
                                                                 // possible values : 1 --> AGV in error mode
                                                                 //                   --> no other case possible right now but may come later in case project code is capable of driving a software safety on his own
        'errorModeCam3D'                    ,{errorModeCam3DVar},
        'slotEmplacement'                   ,{[0.0]},            //Distance from green point for each pallet in the line
        'toleranceEmplacement'              ,0.15,               // Tolerance allowed around a drop point in a defined slot of a line (percent of pallet length)
        'distanceToReverseRpkForScan'       ,1.,                 // Distance before the last point where the robot will begin its reverse move, which matches the moment it will begin its scan
        'fillStrategy'                      ,2,                  // defined the fill strategy for the drop off/pick in scan mission
                                                                 // possible values : 1 --> furthest
                                                                 //                   2 --> nearest
                                                                 //                   3 --> equality
                                                                 //                   4 --> scanlist sequence : first item of the scanlist comes first when possible
                                                                 //                   5 --> scanlist sequence : last item of the scanlist comes first when possible
                                                                 //                   6 --> custom case, to be define in missionCallbacks
        'vmaxWhileScanning'                 ,2.0,                // maximum speed of the truck while scanning (can be adjusted depending of the length of zone + number of elements in scanlist)
                                                                 //          => will be applied between rpkScanChoice and rpkLastReverse

        'anticipationDistance'              ,0.50,               // Distance to start slowing down before reaching rpkApproach
        'conveyorDeviceId'                  ,0,                  // id of the device used for the point to check pallet presence on conveyor
        'conveyorDevicePin'                 ,0,                  // id of the device's entry related to the point to check pallet presence on conveyor
        'safetyFenceDeviceId'               ,0,                  // id of the device used for the point to check the safety fence
        'safetyFenceDevicePin'              ,0,                  // id of the device's entry related to check the safety fence
        'safetyFenceEmergencyDeviceId'      ,0,                  // id of the device used for the point if there is a safety fence if emergency
        'safetyFenceEmergencyDevicePin'     ,0,                  // id of the device's entry related to the point if there is a safety fence if emergency
                                                                 // possible values : 1 --> AGV in error mode if scan empty
                                                                 //                   2 --> Mission is canceled
        'distanceSafetyFence'               ,0.,                 // distance in meter from the green point ( roller for reference ) where the safety fence is ) ;
        'rearSafetiesToInhib'               ,{Null},         // List of the safeties that need to be deactivated to allow the AGV to enter into an elevated slot.
                                                                 // Examples :  {Null}                                                   --> All safeties enabled
                                                                 //                 {strn('rearLaserStop')}                              --> Safety from the fixed rear laser deactivated
                                                                 //                 {strn('rearLaserStop','rearMobileLaserStop'))        --> Safety from the fixed and the mobile laser deactivated

        'anomalyMission'                    ,strn(''),           // name of the anomaly mission for this point is it has one
        'destinationAnomaly'                ,{strn("")},         // list of name(s) of the destination(s) where pallet must be brought to in an anomaly case
                                                                 // single name is standard anomaly behavior. No name or > 1 name indicates dynamic anomaly destination
        'backPalletOverflow'                ,0.0,

        'palletLength'                      ,0.,                 // Length of a pallet in a stock line
        'spaceBetweenPallet'                ,0.,                 // Remaining space between 2 pallets in a stock line
        'activateScanInReverse'             ,0,                  // Activate / Deactivate scan in reverse mode
        'elevatedControlMethod'             ,0,                  // Activate / Deactivate the control of the slot with the mobile laser before a drop

        'rackCell'                          , new("rackCell", 1),
        'palletData'                        , new("pallet", 1),
        'cellPattern'                       , new("cellPattern", 1),
        'isInSpecificSafetyZones'           , 0,

        'expectedPalletType'                , strn(""),          // tye of pallet expected to be found at destination (must match a name in pallet design list)
    );    // Call a function to define project specific fields for stkDB

    if (isFunc("stkDB_customDef"))
    {
        stkDBProjectSpecificFields = acall("stkDB_customDef");
    }
    else
    {
        stkDBProjectSpecificFields = varspc();
    }

    mergeVarspc(v, stkDBProjectSpecificFields);
}
