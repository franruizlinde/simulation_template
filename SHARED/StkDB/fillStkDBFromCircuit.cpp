/**
 * Get interest point information from the circuit
 *
 * @param {varspc} pointData pointData before edit
 *
 * @return {varspc} pointData after edit
 *
 * @public
*/
export function pointData = fillStkDBfromCircuit(pointData)
{
    serial = isRob() ? getRobSerial() : number2Serial(1);

    pointID = pointData->id;
    pointName = pointData->name;
    // Custom field
    defaultConf = stkDBDef();
    setUnexistingVar(pointData, defaultConf);

    palletData = new("pallet", 1);

    layerName = pointData->layerName;

    [destName, destZoneName, destType] = TRACK_getDestNameFromId(layerName, pointData->id);

    switch(destType)
    {
        case (0); //destinationPoint
            // --------------------- POINT ---------------------
            interestPoint = TRACK_getDestinationPoint(layerName, destName);

            if(interestPoint.type_taxi == 1)
            {
                pointData->type         = 'taxi';
            }
            else if(interestPoint.type_charger == 1)
            {
                pointData->type         ='batt';
            }
            else if (interestPoint.type_init == 1)
            {
                pointData->type         ='init';
            }
            else
            {
                pointData->type         = 'move';
                pointData->npalsx       = 0;
                pointData->scan_sick    = 0;
            }

            pointData->zend           = interestPoint.point{0};
            pointData->portionId      = interestPoint.portionId;
        case (1); // Stock Zone
            stockZone = getStockZoneByName(destZoneName);
            pointData->name         = destZoneName;
            pointData->groupName    = destZoneName;
            pointData->npalsx       = stockZone.listStockLines{0}.(0).nbPallet;
            if(pointData->npalsx == 1)
                pointData->type     = 'slotZone';
            else
                pointData->type     = 'lineZone';

            pointData->heightTable  = {[stockZone.listStockLines{0}.(0).height]};
            pointData->palletLength = stockZone.listStockLines{0}.(0).palletLength;
            pointData->spaceBetweenPallet = stockZone.listStockLines{0}.(0).spaceBetweenPallet;
            slotLng      = stockZone.listStockLines{0}.(0).palletLength + stockZone.listStockLines{0}.(0).spaceBetweenPallet;

            slotEmplacement = [];
            k = for (1, pointData->npalsx)
            {
                slotEmplacement.(end+1) = k*slotLng - pointData->spaceBetweenPallet;
            }
            pointData->slotEmplacement = {slotEmplacement};

            scanList = [];
            k = for(stockZone.listStockLines{0})
            {
                scanList = [scanList, stockZone.listStockLines{0}.(k).Id];
            }
            pointData->scanList = {scanList};

            pointData->zend      = stockZone.listStockLines{0}.(0).roadPortion{0}.start{0};
            pointData->zstart    = stockZone.listStockLines{0}.(0).roadPortion{0}.end{0};
            pointData->portionId = stockZone.listStockLines{0}.(0).roadPortion{0}.id;

        case (2); //stockLine
            // ---------------- STOCK LINE -----------------
            stockLine = getStockLineByName(destName);
            stockZone = getStockZoneByName(destZoneName);
            pointData->groupName    = destZoneName;
            pointData->npalsx       = stockLine.nbPallet;

            pointData->heightTable  = {[stockLine.height]};
            pointData->palletLength = stockLine.palletLength;
            pointData->spaceBetweenPallet = stockLine.spaceBetweenPallet;

            slotLng      = stockLine.palletLength + stockLine.spaceBetweenPallet;

            if(pointData->npalsx != 1)
            {
                pointData->type         = 'line';
                slotEmplacement = [];
                k = for(1,stockLine.nbPallet)
                {
                    slotEmplacement.(end+1) = k*slotLng - stockLine.spaceBetweenPallet;
                }
                pointData->slotEmplacement = {slotEmplacement};
            }
            else
            {
                pointData->type             = 'slot';
                pointData->slotEmplacement = {stockLine.palletLength};
            }

            scanList = [];
            listStockLines = stockZone.listStockLines;
            allLinesData = listStockLines.{0};
            k = for (allLinesData)
            {
                scanList.(end+1) = allLinesData.(k).Id;
            }
            pointData->scanList = {scanList};

            pointData->zend     = stockLine.roadPortion{0}.start{0};
            pointData->zstart   = stockLine.roadPortion{0}.end{0};
            pointData->portionId= stockLine.roadPortion{0}.id;

        case (5); //palletData
            // --------------- RACK ----------------
            rackCell = pointData->rackCell ;
            cellPattern = pointData->cellPattern;
            palletData = pointData->palletData;
            palletPortion                    = palletData.portion;

            pointData->groupName             = destZoneName;
            pointData->type                  = 'elevated';
            pointData->npalsx                = rackCell.palletsNumberInDepth;
            pointData->safetyPointMargin     = rackCell.approchDistanceToRackFront;

            pointData->heightTable           = {[rackCell.cellBottom + rackCell.beamThickness]};

            pointData->palletLength          = cellPattern.palletLength;
            pointData->slotEmplacement       = {cellPattern.palletLength};

            pointData->backPalletOverflow    = pointData->palletLength - (palletPortion.{0}.length + cellPattern.palletOverflow);

            pointData->zend             = palletPortion.{0}.start{0};
            pointData->zstart           = palletPortion.{0}.end{0};
            pointData->portionId        = palletPortion.{0}.id;

            pointData->errorModeTractionBlockedPick = 2;
            pointData->errorModeTractionBlockedDrop = 2;

            pointData->isInSpecificSafetyZones = isPointInSpecificSafetyZone(pointData);
            if (rackCell.cellBottom >= 0.5)  // hardwritten value coming from sdk
            {
                pointData->scan_sick    = 0;
                pointData->scanList         = {[]};
            }
            else
            {
                pointData->scanList         = {[pointID]};
            }

        default;
//        case (1, 3, 4, 6); // meta Structure
            // 1 = stockZone
            // 2 = stock line
            // 3= zoneRack            // ie. rack1_doNotUse
            // 4 = rackData ??? // Not even sure this kind of data actually exist
            // 6 = cannot be found such as ...
            // cellData (inside rack editor)...
            // rack upright...
    }

    elevatedCheckModule = getPreferenceValue("elevatedControl/moduleName", serial);
    builtInPref_zTranslation = [char(elevatedCheckModule), "/zTranslation"];
    zTranslation = getPreferenceValue(builtInPref_zTranslation, serial);
    if (isRob)
    {
        allSafeties = acall("secuGet");
        allRearSafetyNames = [];
        j = for(allSafeties)
        {
            if (strMatch(allSafeties.(j).name, "rear") && strMatch(allSafeties.(j).name, "stop"))
            {
                allRearSafetyNames.(end+1) = strn(allSafeties.(j).name);
            }
        }
    }
    else
        allRearSafetyNames = [];

    rearSafetiesToInhib = [];

    palletHeightOffset  = PALLET_getPal(palletData.name{0}, "palletHeightOffset");

    // if liftHeightOffset is high enough so that rearMobile laser can see above the beam by at least beamUpMarginObstacle
    if ((pointData->liftHeightOffset + palletHeightOffset) > pointData->rackCell.beamUpMarginObstacle - zTranslation)
    {
        pointData->elevatedControlMethod = 0;
        k = for (allRearSafetyNames)
        {
            if (!strMatch(allRearSafetyNames.(k), "mobile"))
            {
                rearSafetiesToInhib.(end+1) = strn(allRearSafetyNames.(k));
            }
        }
    }
    else
    {
        pointData->elevatedControlMethod = 1;
        rearSafetiesToInhib = allRearSafetyNames;
    }
    pointData->rearSafetiesToInhib = {rearSafetiesToInhib};
}

/**
 * Called during stkDB creation - Add all destination point from circuit editor in stkDB, with basic fields
 *
 * @public
 */
export function addAllDestinationPoints()
{
    allLayers = TRACK_getAllLayers();

    // Now we have to get all points contained in the circuit
    allDestinationPoints = [];
    k = for(allLayers)
    {
        if(same(char(allLayers.(k)),'Common')) continue; // Destination points not supported by Common layer

        allDestinationPoints = TRACK_getAllDestinationPoints(allLayers.(k));
        i = for(allDestinationPoints)
        {
            // Get dst point
            destinationPnt = allDestinationPoints.(i);
            // Default informations
            pointData = varspc();
            pointData->id = destinationPnt.id;
            pointData->name = strn(destinationPnt.name{0});
            pointData->layerName = strn(allLayers.(k));

            // Fill the entry
            newEntry = fillStkDBfromCircuit(pointData);
            newEntry = stkDB_alter(newEntry);
            // Add entry to sdkDB
            DBadd_gbl("stkDB", newEntry);
        }
    }
}

/**
 * Called during stkDB creation - Add all rack cell from circuit editor in stkDB, with automatically configured fields
 *
 * @public
 */
export function addAllRackPallet()
{
    allRackPallet = TRACK_getAllRackPalletNames();
    layerName = myLayer();

    k = for(allRackPallet)
    {
        cellPattern = new("cellPattern", 1);
        palletData  = new("pallet", 1);
        rackCell = TRACK_getCellFromName(allRackPallet.{k});
        if (size(rackCell))
        {
            allCellPatterns = rackCell.cellPatterns{0};
            j=for(allCellPatterns)
            {
                allPalletInCellPattern = allCellPatterns.pallets{j};
                palletsName = allPalletInCellPattern.name;
                fndIdx = findElt(palletsName,allRackPallet.{k});
                if(size(fndIdx))
                {
                    break;
                }
            }
            cellPattern = allCellPatterns.(j);
            palletData = allPalletInCellPattern.(fndIdx);
        }

        // Default informations
        pointData = varspc();
        pointData->name = strn(allRackPallet.{k});
        pointData->id = palletData.id;
        pointData->layerName = strn(layerName);
        pointData->rackCell = rackCell;
        pointData->cellPattern = cellPattern;
        pointData->palletData = palletData;
        // Fill the entry
        newEntry = fillStkDBfromCircuit(pointData);
        newEntry = stkDB_alter(newEntry);
        // Add entry to sdkDB
        DBadd_gbl("stkDB", newEntry);
    }
}

/**
 * Called during stkDB creation - Add all stock point from circuit editor in stkDB, with automatically configured fields
 *
 * @public
 */
export function addAllStockZones()
{
    // First, let's find all stock zone
    allStockZone = TRACK_getAllStockZone();
    layerName = myLayer();

    // Loop around all stock zones
    k = for(allStockZone)
    {
        // Get one stock zone
        stockZone = allStockZone.(k);
        // // Default informations
        pointData = varspc();
        pointData->name = strn(stockZone.name{0});
        pointData->id = stockZone.Id;
        pointData->layerName = strn(layerName);
        // // Fill the entry
        newEntry = fillStkDBfromCircuit(pointData);
        newEntry = stkDB_alter(newEntry);
        // Add entry to sdkDB
        DBadd_gbl("stkDB", newEntry);
        // Loop around each stock line
        j = for(stockZone.listStockLines{0})
        {
            // Default informations
            pointData = varspc();
            pointData->name = strn(stockZone.listStockLines{0}.(j).name{0});
            pointData->id = stockZone.listStockLines{0}.(j).Id;
            pointData->layerName = strn(layerName);
            // Fill the entry
            newEntry = fillStkDBfromCircuit(pointData);
            newEntry = stkDB_alter(newEntry);
            // Add entry to sdkDB
            DBadd_gbl("stkDB", newEntry);
        }
    }
}

/*
* Functon to get if zend point of pallet portion is in the specific safety management zone
* @param {varspc} pointData
* @return {boolean} if zend point of pallet portion is in the specific safety management zone
*/
export function isInSpecificSafetyZone = isPointInSpecificSafetyZone(pointData)
{
    isInSpecificSafetyZone = 0;
    if(!isVNA())
    {
        return;
    }

    specificSafetyZones = TRACK_getZoneFromType(myLayer(),"type_Specific_Safety_Management");
    coordinates = cpx(pointData->zend.xposition, pointData->zend.yposition);
    i= for(specificSafetyZones)
    {
        isPointInZone = TRACK_isPointInZone(myLayer(),coordinates,specificSafetyZones.(i).name{0});
        if(isPointInZone)
        {
            isInSpecificSafetyZone = isInSpecificSafetyZone || isPointInZone;
        }
    }

}
