export function robotOptionsVarspc = robOptions(robId)
{
    if (nargin<1)
    {
        if (isRob())
            robId = getRobId();
        else
            robId = 1;
    }
    serial = mySerial(robId);

    global robotDesignGbl (sprintf("robotDesign%i", robId));
    if (!isvar(robotDesignGbl, "completedOptions"))
    {
        robotOptionsVarspc = varspc();

        // options around battery
        batteryType = getPreferenceValue("battery/hardware", serial);
        switch(batteryType)
        {
            case("TPPL");
                robotOptionsVarspc->batteryType = "TPPL";
            case("CAN", "Voltage", "Kiwi");
                robotOptionsVarspc->batteryType = "Ld";
            case("Emrol");
                robotOptionsVarspc->batteryType = "LTO";
            default;
                robotOptionsVarspc->batteryType = "Ld";
        }
        robotOptionsVarspc->batteryMissionType = getPreferenceValue("battery/missionType", serial);

        // options around hardware modules
        robotOptionsVarspc->isBarCode           = isModule('barcode', serial);
        robotOptionsVarspc->isCurtain3D         = isModule('curtain3D', serial);
        robotOptionsVarspc->isRearLaser         = isModule('rearLaser', serial);
        robotOptionsVarspc->isMobileLaser       = isModule('rearMobileLaser', serial);

        // optinal actuators
        robotOptionsVarspc->isLIEnabled         = getPreferenceValue("forksActuators/initialLift/enable", serial);
        robotOptionsVarspc->isMastEnabled       = getPreferenceValue("forksActuators/verticalForks/enable", serial) && getPreferenceValue("sensors/encoderVerticalForks/enable", serial);
        robotOptionsVarspc->isSideShiftEnabled  = isModule("forksSideShift", serial) && getPreferenceValue("forksActuators/sideShifterForks/enable", serial);
        robotOptionsVarspc->isRotationEnabled   = getPreferenceValue("forksActuators/rotationForks/enable", serial);

        // Sideshift sensor values may be inverted (requiring special treatment)
        robotOptionsVarspc->isSideShiftInverted = getPreferenceValue("sensors/encoderSideShifterForks/valueInverted", serial)
                                               && (  ( getPreferenceValue("sensors/encoderSideShifterForks/sensorValueHighStop", serial)
                                                     > getPreferenceValue("sensors/encoderSideShifterForks/sensorValueLowStop", serial)     )
                                                  != ( getPreferenceValue("sensors/encoderSideShifterForks/physicalMeasureHighStop", serial)
                                                     > getPreferenceValue("sensors/encoderSideShifterForks/physicalMeasureLowStop", serial) ) );

        // Initial lift sensor
        robotOptionsVarspc->isInductiveSum         = getPreferenceValue("sensors/inductiveSum/enable", serial);
        robotOptionsVarspc->isInitialLiftEstimator = getPreferenceValue("sensors/initialLiftEstimator/enable", serial);

        // pallet presence detection
        isCenterRangefinder                       = isModule('rangefinderCenter', serial) && getPreferenceValue("sensors/encoderPalletRangefinder/enable", serial);
        isLCenterRangefinder                      = isModule('rangefinderCenterLeft', serial) && getPreferenceValue("sensors/encoderLCenterRangefinder/enable", serial);
        isRCenterRangefinder                      = isModule('rangefinderCenterRight', serial) && getPreferenceValue("sensors/encoderRCenterRangefinder/enable", serial);
        isPalletSensor                            = isModule('rangefinderCenter', serial) && getPreferenceValue("sensors/encoderPalletBackrest/enable", serial);
        robotOptionsVarspc->isCenterRangefinder   = isCenterRangefinder;
        robotOptionsVarspc->isLCenterRangefinder  = isLCenterRangefinder;
        robotOptionsVarspc->isRCenterRangefinder  = isRCenterRangefinder;
        robotOptionsVarspc->isAnyRangefinder      = isCenterRangefinder || isLCenterRangefinder || isRCenterRangefinder;
        robotOptionsVarspc->isPalletSensor        = isPalletSensor;

        // side sensors
        isLSideRangefinder                        = isModule('telemeterLeft', serial) && getPreferenceValue("sensors/encoderLSideRangefinder/enable", serial);
        isRSideRangefinder                        = isModule('telemeterRight', serial) && getPreferenceValue("sensors/encoderRSideRangefinder/enable", serial);
        if (isLSideRangefinder || isRSideRangefinder)
        {
            isLAntiIntrusion = getPreferenceValue("sensors/encoderLSideRangefinder/safety/enable", serial);
            isRAntiIntrusion = getPreferenceValue("sensors/encoderRSideRangefinder/safety/enable", serial);
            if (isLAntiIntrusion && isRAntiIntrusion)
            {
                robotOptionsVarspc->isOverhang      = 0;
                robotOptionsVarspc->isAntiIntrusion = 1;
            }
            else
            {
                robotOptionsVarspc->isOverhang      = 1;
                robotOptionsVarspc->isAntiIntrusion = 0;
            }
        }
        else
        {
            robotOptionsVarspc->isOverhang      = 0;
            robotOptionsVarspc->isAntiIntrusion = 0;
        }

        // 3D cameras
        sensorNames                             = getPreferenceValue("sensors/sensorIP/sensorNames", serial);
        robotOptionsVarspc->isMastCamera        = size(findElt(sensorNames, "mastCamera"))  || isModule("mastCam3D", serial);
        robotOptionsVarspc->isForksCamera       = size(findElt(sensorNames, "forksCamera")) || isModule("forksCam3D", serial);

        // safety design
        robotOptionsVarspc->isRearSafetyLaserOnTheForks = getPreferenceValue("safety/design/isRearSafetyLaserOnTheForks",serial) != 0;
        robotOptionsVarspc->isPerceptionV2Enabled       = getPreferenceValue("detection/PerceptionV2/enable",serial) == 1;

        // any laser under initial lift?
        potentialLasersUnderLI = strn("rearLaser", "rearRightLaser", "rearLeftLaser");
        isSafetyLaserUnderInitialLift = 0;
        k = for (potentialLasersUnderLI)
        {
            laserName               = potentialLasersUnderLI.(k);
            builtInPref_isUnderLI   = [char(laserName), "/safety/isUnderInitialLift"];
            isUnderLI               = getPreferenceValue(builtInPref_isUnderLI, serial, 0);

            if (isModule(laserName, serial) && isUnderLI)
            {
                isSafetyLaserUnderInitialLift = 1;
                break;
            }
        }
        robotOptionsVarspc->isSafetyLaserUnderInitialLift       = isSafetyLaserUnderInitialLift;

        // any safety blind when forks down
        whileDrivingForward = 0;
        whileDrivingBackward = 0;
        whileScanning = 0;
        potentialBlindLasers = strn("rearLaser", "rearRightLaser", "rearLeftLaser", "rearMobileLaser");
        allLasersUsedForScan = getPreferenceValue("detection/module", serial);
        k = for (potentialBlindLasers)
        {
            laserName                          = potentialBlindLasers.(k);

            // if rearMobileLaser is not the only rear laser, it can be ignored as it will be inhibited during travel
            if (strMatch(laserName, "mobile") && isModule("rearLaser", serial))
            {
                continue;
            }

            builtInPref_isBlindWhenFrkDown     = [char(laserName), "/safety/isBlindWhenForksDown"];
            isBlindWhenForksDown               = getPreferenceValue(builtInPref_isBlindWhenFrkDown, serial, 0);
            builtInPref_enabledInBothDir       = [char(laserName), "/safety/activateLaserInBothDirection"];
            enabledInBothDir                   = getPreferenceValue(builtInPref_enabledInBothDir, serial, 0);

            if (isModule(laserName, serial) && isBlindWhenForksDown)
            {
                whileDrivingBackward = 1;
                if (enabledInBothDir)
                {
                    whileDrivingForward = 1;
                }
                if (size(findElt(allLasersUsedForScan, laserName)))
                {
                    whileScanning = 1;
                }
            }
        }
        robotOptionsVarspc->isSoftSafetyBlindWhenForksDown       = varspc("whileDrivingForward", whileDrivingForward,
                                                                          "whileDrivingBackward", whileDrivingBackward,
                                                                          "whileScanning", whileScanning);

        // Mowo RVB (Remote Vision Box)
        robotOptionsVarspc->isEquipedWithRVB = !strcmp(getPreferenceValue("communication/bridgeType",serial),'RVB');

        unint
        {
            robotDesignGbl->robOptions = robotOptionsVarspc;
            robotDesignGbl->completedOptions = 1;
        }
    }
    else
    {
        unint robotOptionsVarspc = robotDesignGbl->robOptions;
    }
}

export function robotDimensionsVarspc = robDimensions(robId)
{
    if (nargin<1)
    {
        if (isRob())
            robId = getRobId();
        else
            robId = 1;
    }
    serial = mySerial(robId);

    global robotDesignGbl (sprintf("robotDesign%i", robId));
    if (!isvar(robotDesignGbl, "completedDimensions"))
    {
        robotDimensionsVarspc = varspc();
        robotDimensionsVarspc->rollerToEndEmptyField            = min(getPreferenceValue('safety/pattern/Fempty' , serial).re);
        robotDimensionsVarspc->rollerToEndCarryField            = min(getPreferenceValue('safety/pattern/Fcarry' , serial).re);
        robotDimensionsVarspc->rollerToBackrestDeployed         = getPreferenceValue('general/backrestDeployedAbscissa', serial);
        robotDimensionsVarspc->rollerToBackrest                 = getPreferenceValue('general/backrestAbscissa', serial);
        robotDimensionsVarspc->rollerToForksEnd                 = getPreferenceValue('general/forksEndAbscissa', serial);

        unint
        {
            robotDesignGbl->robDimensions = robotDimensionsVarspc;
            robotDesignGbl->completedDimensions = 1;
        }
    }
    else
    {
        unint robotDimensionsVarspc = robotDesignGbl->robDimensions;
    }
}

export function robotTypeVarspc = robType(robId)
{
    if (nargin<1)
    {
        if (isRob())
            robId = getRobId();
        else
            robId = 1;
    }
    serial = mySerial(robId);

    global robotDesignGbl (sprintf("robotDesign%i", robId));
    if (!isvar(robotDesignGbl, "completedType"))
    {
        robotTypeVarspc = varspc();
        truckType = getPreferenceValue('general/truckType', serial);
        robotTypeVarspc->isTugger           = !strcmp(truckType, 'Tugger');
        robotTypeVarspc->isPalletStacker    = strMatch(truckType, 'PalletStacker');
        robotTypeVarspc->isPalTruck         = strMatch(truckType, 'PalletTruck');
        robotTypeVarspc->isReachTruck       = strMatch(truckType, 'ReachTruck');
        robotTypeVarspc->isVNA              = strMatch(truckType, 'VNA');

        unint
        {
            robotDesignGbl->robType = robotTypeVarspc;
            robotDesignGbl->completedType = 1;
        }
    }
    else
    {
        unint robotTypeVarspc = robotDesignGbl->robType;
    }
}

export function forksDesignVarspc = forksDesign(robId)
{
    if (nargin<1)
    {
        if (isRob())
            robId = getRobId();
        else
            robId = 1;
    }
    serial = mySerial(robId);

    global forksDesignGbl (sprintf("robotDesign%i", robId));
    if (!isvar(forksDesignGbl, "completedForks"))
    {
        forksDesignVarspc = varspc();
        forksDesignVarspc->forksThickness     = getPreferenceValue("forksParameters/verticalForksThickness", serial);
        forksDesignVarspc->toleranceUp        = getPreferenceValue("forksControllers/verticalForks/genericForksController/parameters/toleranceUp", serial);
        forksDesignVarspc->toleranceDown      = getPreferenceValue("forksControllers/verticalForks/genericForksController/parameters/toleranceDown", serial);

        unint
        {
            forksDesignGbl->robType = forksDesignVarspc;
            forksDesignGbl->completedForks = 1;
        }
    }
    else
    {
        forksDesignVarspc = forksDesignGbl->robType;
    }
}