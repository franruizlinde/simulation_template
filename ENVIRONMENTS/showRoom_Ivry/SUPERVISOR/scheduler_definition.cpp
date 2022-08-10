// end of schdModDefs
export function setupCustomSchedulerConfig(conf)
{
    // conf->options->taskConf.{end+1} = varspc('taskName', 'Move Pallet', 'beforeStartCbk', 'cleverPickAndDrop_startCbk', 
                                            // 'typeSrc','SR','typeDst','SR', 'visibleOnInterface', 1);

    // conf->options->taskConf.{end+1} = varspc('taskName', 'Pick Single Line No Scan', 'beforeStartCbk', 'pickSingleLineNoScan_startCbk', 
											// 'typeSrc','','typeDst','S', 'visibleOnInterface', 1);
	// conf->options->taskConf.{end+1} = varspc('taskName', 'Pick Elevated', 'beforeStartCbk', 'pickElevated_startCbk', 
											// 'typeSrc','','typeDst','R', 'visibleOnInterface', 1);
	// conf->options->taskConf.{end+1} = varspc('taskName', 'Drop Single Line No Scan', 'beforeStartCbk', 'dropSingleLineNoScan_startCbk', 
											// 'typeSrc','','typeDst','S', 'visibleOnInterface', 1);
	// conf->options->taskConf.{end+1} = varspc('taskName', 'Drop Elevated', 'beforeStartCbk', 'dropElevated_startCbk', 
											// 'typeSrc','','typeDst','R', 'visibleOnInterface', 1);
	// conf->options->taskConf.{end+1} = varspc('taskName', 'Stock No Scan to Stock No Scan', 'beforeStartCbk', 'pickDropStockNoScan_startCbk', 
											// 'typeSrc','S','typeDst','S', 'visibleOnInterface', 1, 'robAuth', {[]});
	// conf->options->taskConf.{end+1} = varspc('taskName', 'Rack to Rack', 'beforeStartCbk', 'pickDropRack_startCbk', 
											// 'typeSrc','R','typeDst','R', 'visibleOnInterface', 1, 'robAuth', {[]});
	// conf->options->taskConf.{end+1} = varspc('taskName', 'Slot No Scan to Rack', 'beforeStartCbk', 'pickDropSlotToRack_startCbk', 
											// 'typeSrc','S','typeDst','R', 'visibleOnInterface', 1, 'robAuth', {[]});
	// conf->options->taskConf.{end+1} = varspc('taskName', 'Rack to Slot No Scan', 'beforeStartCbk', 'pickDropRackToSlot_startCbk', 
											// 'typeSrc','R','typeDst','S', 'visibleOnInterface', 1, 'robAuth', {[]});
                                            
    // conf->options->taskConf.{end+1} = varspc('taskName','Move','beforeStartCbk','move_startCbk','beforeCreateCbk','move_beforeCreateCbk',
		// 'doneCbk','move_doneCbk','cancelCbk','move_cancelCbk','lockCbk','move_lockCbk',
        // 'unlockCbk','move_unlockCbk', 'runCbk', 'move_runCbk','waitCbk','move_waitCbk','typeSrc','P','typeDst','P',
		// 'visibleOnInterface', 1);
    
    // /// Uncomment this line if you have a docking station!
    // conf->options->taskConf.{end+1} = varspc('taskName', 'battery','beforeCreateCbk','battery_createCbk','beforeStartCbk','battery_startCbk',
											// 'doneCbk','battery_doneCbkWithDocking','cancelCbk','battery_cancelCbkWithDocking',
											 // 'typeDst','P');

    conf->options->taskConf.{end+1} = varspc('taskName',        'movePal',
                                             'beforeStartCbk',  'cleverPickAndDrop_startCbk',
                                             'beforeCreateCbk', 'example_beforeCreateCbk',
                                             'waitCbk',         'example_waitCbk',
                                             'runCbk',          'example_runCbk',
                                             'doneCbk',         'example_doneCbk',
                                             'cancelCbk',       'example_cancelCbk',
                                             'lockCbk',         'example_lockCbk',
                                             'unlockCbk',       'example_unlockCbk',
                                             'cancelCbk',       'example_cancelCbk');
                                             
    conf->options->taskConf.{end+1} = varspc('taskName',        'vidange',
                                             'beforeStartCbk',  'cleverPickAndDrop_startCbk',
                                             'beforeCreateCbk', 'example_beforeCreateCbk',
                                             'waitCbk',         'example_waitCbk',
                                             'runCbk',          'example_runCbk',
                                             'doneCbk',         'example_doneCbk',
                                             'cancelCbk',       'example_cancelCbk',
                                             'lockCbk',         'example_lockCbk',
                                             'unlockCbk',       'example_unlockCbk',
                                             'cancelCbk',       'example_cancelCbk');
    
    // Custom field (non string and not displayed on HMI) will be inserted here by the task Editor : 
    // @START_AUTO_SCHD_DB_CONF
    // @END_AUTO_SCHD_DB_CONF
    
    // Custom field (only strings that are displayed on the RMI) :
	// Customs fields example (space in custom filed name not supported)
	//conf->options->customFields = strn('PERSO1','PERSO2');
    
    // Tasks defined in task editor will be inserted here :
    // @START_AUTO_TASK_CONF
    // @END_AUTO_TASK_CONF
    
}