 /**
  * Function to add custom fields in doorDB
  *
  * @return {varspc} Varspc with fields and default value to add in the doorDB
  *
  * @public
  */
export function doorDBPersoFields = doorDBPersoFieldsDef()
{
    doorDBPersoFields = varspc(
        'doorDoCbk'                          ,strn("defaultDoor_doCbk"),
        'doorStateCbk'                       ,strn("defaultDoor_stateCbk"),

        'maintenance'                        ,0,
        'canBeCrossedInStandAlone'           ,0,

        'forceExtension'                     ,0,
        'dockingDestPoint'                   ,strn(''),
        'dockingLastState'                   ,0,
        'dockingStation'                     ,0,
        'dockingState'                       ,0,
        'idFlexi'                            ,-1,
        'dockingRetracted'                   ,{[]},
        'dockingExtended'                    ,{[]},
        'retroReflexSensor'                  ,{[]},
        'interlock'                          ,{[]},

        'commandRetract'                     ,{[]},
        'commandReach'                       ,{[]},
        'resetDockingStation'                ,{[]},

        'manualMode'                         ,{[]},
        'errorTimer'                         ,{[]},
        'errorSafetyEdge'                    ,{[]},
        'errorEndStopReach'                  ,{[]},

        'lostInterlock'                      ,{[]},
        'errorOvercurrent'                   ,{[]},
        'errorAgvMovedWhileExtending'        ,{[]},
        'errorErmergencyAU'                  ,{[]},

    );

    if (isFunc("doorDB_customDef"))
    {
        customFields = acall("doorDB_customDef");
    }
    else
    {
        customFields = varspc();
    }
    mergeVarspc(doorDBPersoFields, customFields);
}


/**
 * Function called to define each door (function called on robot and on robot manager)
 * Each field not configured will take default value or will be kept from previous doorDB.obj file
 *
 * @return {cell} Door configuration (each cell contains the varspc for a single door)
 *
 * @public
 */
export function allDoorEntries = doorGetEntries()
{
    allDoorEntries = doorDB_populate();

    k = for (allDoorEntries)
    {
        doorEntry = allDoorEntries.{k};
        if (isvar(doorEntry, "doorDoCbk") && !isFunc(doorEntry->doorDoCbk))
        {
            logError("[DOOR] - function defined as doorDoCbk does not exist so cannot be used as doCbk : use defaultDoor_doCbk instead (door %s)", doorEntry->zoneName);
            doorEntry->doorDoCbk = strn("defaultDoor_doCbk");
        }
        if (isvar(doorEntry, "doorStateCbk") && !isFunc(doorEntry->doorStateCbk))
        {
            logError("[DOOR] - function defined as doorStateCbk does not exist so cannot be used as stateCbk : use defaultDoor_stateCbk instead (door %s)", doorEntry->zoneName);
            doorEntry->doorStateCbk = strn("defaultDoor_stateCbk");
        }
        allDoorEntries.{k} = doorEntry;
    }
}