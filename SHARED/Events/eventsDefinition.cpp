/**
 * Definition of all events to be exchange between RM and robot
 * Careful : event names should be 17 chars long max (no longer than 17)
 *
 * @return {cell} Liste des events
 * @return {varspc} Configuration du comportement des events
 *
 * @public
 */
export function [eventList, eventConf] = projectEventsDef()
{
    // Events must be declared with the following syntax:
    //     varspc(
    //      'category','comm',      // category of the event. STRING is expected among a list of possibilities (core, fire, DEVICE...). Use the value 'comm'
    //      'name',,                // name of the event. String expected
    //      'broadcast',0,          // Boolean. Default value is 0. If value is 1 then the event will be sent to all the robots automatically. Not possible to have a broadcast sent from a robot.
    //      'linkTo',{strn('')},    // Name of the function executed on the receiver'side for the non broadcasted functions
    //      'linkOnBcst',{strn('')} // Name of the function executed on the receiver'side for the broadcasted functions
    //    )

    eventList = {
        varspc('category','comm','name','endMissPick','linkTo',{strn('onEndMissPick')}),
        varspc('category','comm','name','endMissDrop','linkTo',{strn('onEndMissDrop')}),
        varspc('category','comm','name','endMissMove','linkTo',{strn('onEndMissMove')}),
        varspc('category','comm','name','goingToBatt','linkTo',{strn('onGoingToBatt')}),
        varspc('category','comm','name','endMissBatt','linkTo',{strn('onEndMissBatt')}),

        varspc('category','batt','name','battMsg','broadcast',1,'linkTo',{strn('LTO_onBattMsg')},'linkOnBcst',{strn('BATT_onBattMsg')}),

        varspc('category','comm','name','pickFailed','linkTo',{strn('onPickFailed')}),
        varspc('category','comm','name','dropFailed','linkTo',{strn('onDropFailed')}),
        varspc('category','comm','name','agvErr','linkTo',{strn('onAgvErr')}),

        varspc('category','comm','name','anomaly','linkTo',{strn('onAnomaly')}),
        varspc('category','comm','name','isSafetyOpen','linkTo',{strn('onIsSafetyFenceOpen')}),
        varspc('category','comm','name','askConveyorState','linkTo',{strn('onAskConveyorState')}),

        varspc('category','fire','name','fireAlarm','broadcast',1,'linkOnBcst',{strn('FIRE_onTrigger')}),
        varspc('category','comm','name','askFireAlarm','linkTo',{strn('onAskFireAlarm')}),
        varspc('category','fire','name','fireAlarmClear','broadcast',1,'linkOnBcst',{strn('FIRE_onClear')}), // Launched on the RM for all robots

        varspc('category','itinerary','name','agvGo', 'broadcast', 0),
        varspc('category','itinerary','name','agvStop', 'broadcast', 0),

        varspc('category','DEVICE','name','DEVICEChange','broadcast',0,'linkTo',{strn('DEVICE_onInputChange')}),  // triggered by RM on RM only
        varspc('category','DEVICE','name','DEVICEComChange','broadcast',0,'linkTo',{strn('DEVICE_onComChange')}), // triggered by RM on RM only

        // Traffic Mgt
        varspc('category','comm','name','changeTrailerLng','linkTo',{strn('onChangeTrailerLength')}),
        varspc('category','TRAFFIC','name','newDeadlock','broadcast',0,'linkTo',{strn('onNewDeadlock')}),   //triggered by RM on RM only
        varspc('category','TRAFFIC','name','endOfDeadlock','broadcast',0,'linkTo',{strn('onEndDeadlock')}), //triggered by RM on RM only
        varspc('category','comm','name','agvInDeadlock','linkTo',{strn('onAgvInDeadlock')}),             //launched by RM to trigger action on robots

        // VNA Frontal mission and stacking/unstacking missions: canton management
        varspc('category','comm','name','cantonHold','linkTo',{strn('onCantonStopReleaseReq')}),
        varspc('category','comm','name','cantonRelease','linkTo',{strn('onCantonStartReleaseReq')}),

        // Barcode and pallet info
        varspc('category','comm','name','sendPalletType','linkTo',{strn('onSendPalletType')}),
        varspc('category','comm','name','askPalletType','linkTo',{strn('onAskPalletType')}),
        varspc('category','comm','name','sendBarcode','linkTo',{strn('onSendBarcode')}),
        varspc('category','comm','name','foundBarcode','linkTo',{strn('onFoundBarcode')}),

        // Docking station
        varspc('category','comm','name','extendStation','linkTo',{strn('onExtendStation')}),
        varspc('category','comm','name','retractStation','linkTo',{strn('onRetractStation')}),
        varspc('category','comm','name','resetStation','linkTo',{strn('onResetStation')}),
        varspc('category','comm','name','dockingState','linkTo',{strn('onDockingState')}),

        // LeadAcid TPPL Charger
        varspc('category','comm','name','setEnableCharger','linkTo',{strn('onSetEnableCharger')}),
        varspc('category','comm','name','setDisableCharger','linkTo',{strn('onResetEnableCharger')}),
        varspc('category','comm','name','setEqualCharge','linkTo',{strn('onSetEqualCharge')}),
        varspc('category','comm','name','resetEqualCharge','linkTo',{strn('onResetEqualCharge')}),

        // varspc('category','comm','name','newDest','linkTo',{strn('changePoseDest')}), //exÃ©cutÃ© sur le rob
        // varspc('category','COMBOX','name','COMBOXComChange','broadcast',0,'linkTo',{strn('onCOMBOXComChange')}), //execute sur le pc
    };

    // Call a function to define project specific events
    if (isFunc("events_populate"))
    {
        newEvents = acall("events_populate");
    }
    else
    {
        newEvents = {};
    }
    eventList.(lin(end+1, end+size(newEvents))) = newEvents;

    // Configuration to display (or not) warnings on unknown events
    eventConf = varspc(
        'warningOnUnkEvent_comm',    1,
        'warningOnUnkEvent_core',    0,
        'warningOnUnkEvent_miss',    0,
        'warningOnUnkEvent_mCir',    1,
    );
}