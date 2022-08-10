/**
 * Function to edit the list of fields in doorDB
 *
 * @return {varspc} list of extra fields with their default values
 *
 * @public
 */
export function doorDBCustomFields = doorDB_customDef()
{
    doorDBCustomFields = varspc();
    /*
        // Here is an example of how adding fields to the doorDB
        doorDBDefinition->customField1 = strn('');      // Char
        doorDBDefinition->customField2 = 0;             // Int
        doorDBDefinition->customField1 = {[]};          // Array of values
    */
}

/**
 * Function called to define each door (function called on robot and on robot manager)
 * Each filed not configure will take default value or will be kept from previous doorDB.obj file
 *
 * @return {cell} Door configuration (each cell contains the varspc for a single door)
 *
 * @public
 */
export function v = doorDB_populate()
{
    v = {};
    return;
    v.{end+1} = varspc(
             'zoneName',        strn('door1'),
             'comboxIDIn',      1,
             'comboxPortIn',    1,
             'comboxIDOut',     1,
             'comboxPortOut',   3,
             'z',               cpx(67.12,13.7),
             'dim',             cpx(2,0.4),
             'dAsk',            6,
             'dStop',           1,
             'checkRate',       500,
             'slowThroughDoor', 0,
             'continuousCheck', 0,
             'useCloseComboxIn',0,
             'useCloseComboxOut',0,
             'fireAlarmCombox', 0,
     );

    /// Docking Station example
	
    v.{end+1} = varspc(
         'zoneName'                          ,'dockingStation',
        //'forceExtension'        ,1,
         'comboxIDIn',1, // Set at the same value as idFlexi
         'comboxPortIn',1, // Not used with docking station
         'comboxIDOut',1, // Set at the same value as idFlexi
         'comboxPortOut',1,  // Not used with docking station
         
         'z'                                 ,cpx(72.54,10.89),
         'dAsk'                              ,1.,
         'dStop'                             ,0.5,
         'checkRate'                         ,500,
         'slowThroughDoor'                   ,0, //optional
         'dim'                               ,cpx(3,2),
         
         'dockingStation'                    ,1, //if docking station
         'dockingState'                      ,0,
         'dockingDestPoint'                  ,'Docking Station',
         'idFlexi'                           ,1,
         // Input
         'dockingRetracted'                  ,{[2,3]}, // Register number / bit number
         'dockingExtended'                   ,{[2,2]}, // Register number / bit number
         'retroReflexSensor'                 ,{[2,9]},
         'interlock'                         ,{[2,5]},
         // Output
         'commandRetract'                    ,{[1,1]},
         'commandReach'                      ,{[1,0]},
         'resetDockingStation'               ,{[2,0]},
         // Info 
         'manualMode'                        ,{[14,6]},
         'errorTimer'                        ,{[14,3]},
         'errorSafetyEdge'                   ,{[14,2]},
         'errorEndStopReach'                 ,{[14,1]},
         'continuousCheck'                   ,1,
    );
/*
    // Standard Door Example below (for 4.10.3 and up)
    v.{end+1} = varspc(
                'zoneName',strn('door1'),
                'comboxIDIn',1,
                'comboxPortIn',1,
				'useCloseComboxIn',0,
			   	'closeComboxIDIn',1, //optional
			   	'closeComboxPortIn',2, //optional
                'comboxIDOut',1,
                'comboxPortOut',1,
                'useCloseComboxOut',0,
                'closeComboxIDOut',1, //optional
                'closeComboxPortOut',2, //optional
                'fireAlarmCombox',0,
                'fireAlarmComboxId',1, //optional
                'fireAlarmComboxPort',3, //optional
                'z',cpx(-12,-6.45),
    			'dim',cpx(0.4,2),
                'dAsk',5.5,
                'dStop',2.,
                'checkRate',500, //in milliseconds
                'slowThroughDoor',0,
				'continuousCheck', 0,
            );

    /// Docking Station example
    v.{end+1} = varspc(
         'zoneName'                          ,'dockingStation',

         'comboxIDIn',0, // Set at the same value as idFlexi
         'comboxPortIn',1, // Not used with docking station
         'comboxIDOut',0, // Set at the same value as idFlexi
         'comboxPortOut',1,  // Not used with docking station

         'z'                                 ,cpx(34.03,9.23),
         'dAsk'                              ,1.,
         'dStop'                             ,0.5,
         'checkRate'                         ,500, //in milliseconds
         'slowThroughDoor'                   ,0, //optional
         'dim'                               ,cpx(2,3),

         'continuousCheck'                    ,1, //if docking station

         'dockingDestPoint'                  ,'Docking Station',
         'idFlexi'                           ,0,
         // Input
         'dockingRetracted'                  ,{[2,3]}, // Register number / bit number
         'dockingExtended'                   ,{[2,2]}, // Register number / bit number
         'retroReflexSensor'                 ,{[2,9]}, //Block déploiement si pas en face du reflecteur du chariot
         'interlock'                         ,{[2,5]}, //Block chargement si pas connecté
         // Output
         'commandRetract'                    ,{[0,1]},
         'commandReach'                      ,{[0,0]},
         // Info
         'manualMode'                        ,{[14,6]},
         'errorTimer'                        ,{[14,3]},
         'errorSafetyEdge'                   ,{[14,2]},
         'errorEndStopReach'                 ,{[14,1]},
    );
*/
}