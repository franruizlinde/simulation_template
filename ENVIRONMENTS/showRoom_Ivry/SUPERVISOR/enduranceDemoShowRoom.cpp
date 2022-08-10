export function demoShowroom(idrob, idrob2)
{
	// Vidange du Rack sur le convoyeur
	
	TASKS_movePal("400100", "401100", idrob);
	
	TASKS_movePal("400200", "401100", idrob);
	
	TASKS_movePal("400300", "401100", idrob);
	
	TASKS_movePal("400400", "401100", idrob);
	
	TASKS_movePal("400500", "401100", idrob);
	
	TASKS_movePal("400600", "401100", idrob);
}

export function solRack(idrob, idrob2)
{
	TASKS_movePal("stk1", "400100", idrob);

	TASKS_movePal("stk2", "400400", idrob);	
	
	TASKS_movePal("stk3", "400200", idrob);
	
	TASKS_movePal("stk11", "400500", idrob2);	
	
	TASKS_movePal("stk22", "400300", idrob2);	
	
	TASKS_movePal("stk33", "400600", idrob2);
}

export function vidange(idrob, idrob2)
{
    TASKS_vidange("stk1", "400100", idrob);

	TASKS_vidange("stk2", "400400", idrob2);	
	
	TASKS_vidange("stk3", "400200", idrob);

	TASKS_vidange("stk11", "400500", idrob2);	
	
	TASKS_vidange("stk22", "400300", idrob);	
	
	TASKS_vidange("stk33", "400600", idrob2);
}

export function TASKS_movePal(pickspot, dropspot, robotID)
{
	v = varspc();

    v->taskName = "movePal";
    v->robAuth  = {[robotID]};
	v->src 		= TRACK_GetIdFromDestName(getRobModel(robotID), pickspot);
	v->dst		= TRACK_GetIdFromDestName(getRobModel(robotID), dropspot);
    v->priority = 1;

    logDebug('[DEMO-] Creating task %s for robot %z: source = %z / dest = %z', v->taskName, robotID, pickspot, dropspot);

    schdTaskNew(v);
}

export function TASKS_vidange(pickspot, dropspot, robotID)
{
	v = varspc();

    v->taskName = "vidange";
    v->robAuth  = {[robotID]};
	v->src 		= TRACK_GetIdFromDestName(getRobModel(robotID), pickspot);
	v->dst		= TRACK_GetIdFromDestName(getRobModel(robotID), dropspot);
    v->priority = 100;

    logDebug('[DEMO-] Creating task %s for robot %z: source = %z / dest = %z', v->taskName, robotID, pickspot, dropspot);

    schdTaskNew(v);
}