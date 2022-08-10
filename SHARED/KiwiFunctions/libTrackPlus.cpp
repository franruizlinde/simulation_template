/**
 * Provide the id of a door entry associated to a MOBILE docking station, given the destination ID of the point on the track to reach the mobile docking station
 *
 * @param {long} destinationId : ID of the point in circuit editor
 *
 * @return {string} ID of the door entry from the doorDB of the associated mobile docking station. Empty if none associated
 *
 * @public
*/
export function doorId = TRACK_getDockingDoorFromDest(destinationId)
{
    destinationName = id2name(destinationId);
    doorId = setInter(DBfind_gbl('doorDB','dockingDestPoint','==',destinationName),DBfind_gbl('doorDB','dockingStation','==',1));
}

/**
 * Check if a robot is inside a zone. Check can be done relatively to gabarit or point.
 *
 * @param {string | long} zoneNameOrId name or id of the zone
 * @param {string} (optional) layer name (where the zone is defined), if known
 * @param {long} (optional) robId Id of the robot
 * @param {string} (optional) projectionMode gabarit or point
 *
 * @return {long} isInZone returns 1 if robot is in zone, otherwise 0
 *
 * @public
*/
export function isInZone    = TRACK_isRobotInsideZone(zoneNameOrId, layerName, robId, projectionMode)
{
    //By default return 0
    isInZone    = 0;

    //On RM side, If robot id is not passed as argument, consider robot 1
    if(nargin < 3)
        robId   = isRob() ? getRobId() : 1;

    // if no layer was given, search it
    if (nargin < 2)
    {
        //Retrieve information about the zone
        layerList   = [strn(getRobModel(robId)), strn("Common")];
        k = for(layerList)
        {
            layerName       = layerList.(k);
            if(isNumeric(zoneNameOrId))
                zoneData    = TRACK_GetZoneFromId(layerList.(k), zoneNameOrId);
            else
                zoneData    = TRACK_GetZoneFromName(layerList.(k), zoneNameOrId);

            if(!isnull(zoneData)) break;
        }
        //Return 0 if zone does not exist
        if(isnull(zoneData))
        {
            logWarn("[TRACK_isRobotInsideZone] - Could not find zone %z in any layer", zoneNameOrId);
            return;
        }
    }
    else
    {
        if(isNumeric(zoneNameOrId))
            zoneData    = TRACK_GetZoneFromId(layerName, zoneNameOrId);
        else
            zoneData    = TRACK_GetZoneFromName(layerName, zoneNameOrId);
    }

    //If projection mode is not passed in argument, select one by looking at the zone definition
    if(nargin < 4)  projectionMode  = zoneData.projection == 0 ? "point" : "gabarit";

    //Retrieve current robot position
    if(isRob())
    {
        robPos  = acall("getzm");
        robCap  = acall("getcapm");
    }
    else
    {
        robPos  = acall("robGetz", robId);
        robCap  = acall("robGetcap", robId);
    }

    //Return 0 if robot position is unknown
    if(isnan(robPos) || isnan(robCap))
    {
        return;
    }

    //Then, discuss the projection mode
    if(isStrEqual(projectionMode, "point"))
    {
        isInZone    = TRACK_IsPointInZone(layerName, robPos, zoneData.name{0});
    }
    else if(isStrEqual(projectionMode, "gabarit"))
    {
        //Build a polygon after robot's pattern
        robPattern      = getPreferenceValue("cantonnement/pattern", number2Serial(robId));
        robPoly         = cnt2poly(robPattern);

        //Rotate and translate this polygon to current robot's position
        movedRobPoly    = movePoly(robPoly, robCap, robPos);

        //Check if polygon and zone intersect
        isInZone        = TRACK_IsPolygonCollisionZone(layerName, zoneData.name{0}, movedRobPoly.cnt{0});
    }
}

/**
 * Turn a coordinate (of any object : robot, point, whatever) into a corrected position on the track
 *
 * @param {cpx} non projected position
 * @param {double} (optional) cap : known cap of the point to project
 *
 * @return {cpx} projected position
 *
 * @public
*/
export function projectedPosition = TRACK_projectOnTrack(position, cap)
{
    if (nargin == 2)
        trackPosition = TRACK_GetTrackPosition(myLayer(), position, cap);
    else
        trackPosition = TRACK_GetTrackPosition(myLayer(), position);

    portionIdInformation = TRACK_GetPortionInfoFromID(myLayer(), trackPosition.portionId);
    projectedPosition = point2cpx(portionIdInformation.start{0}) + trackPosition.curvilinearAbscissa * iexp(portionIdInformation.cap);
}
