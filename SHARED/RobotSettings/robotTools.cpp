/**
 * Function to get the current layer name
 *
 * @param {long} (optional) id of the inquired robot
 *
 * @return {string} name of the layer
 *
 * @public
 */
export function layerName = myLayer(robId)
{
    if (nargin<1)
    {
        if (isRob)
        {
            robId = getRobId();
        }
        else
        {
            robId = 1;
        }
    }

    layerName = getRobModel(robId);
}

/**
 * Function to get the serial of the robot
 *
 * @param {long} (optional) id of the inquired robot
 *
 * @return {string} serial number of the inquired robot
 *
 * @public
 */
export function serial = mySerial(robId)
{
    if (nargin<1)
    {
        if (isRob)
        {
            robId = getRobId();
        }
        else
        {
            robId = 1;
        }
    }

    serial = number2Serial(robId);
}
