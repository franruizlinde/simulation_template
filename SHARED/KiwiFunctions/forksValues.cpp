/**
 * Functions called to make the actuators more readable
 *
 * @return {long} v actuator index
 *
 * @public
 */
export function v = VERTICALFORKS           {v = 0;}
export function v = SIDESHIFT               {v = 1;}
export function v = REACH                   {v = 2;}
export function v = RETRACT                 {v = 2;}
export function v = TILT                    {v = 3;}
export function v = INITIALLIFT             {v = 4;}
export function v = AUXILIARY               {v = 5;}
export function v = ROTATION_SIDE           {v = 6;}
export function v = ROTATION_FRONT          {v = 8;}

/**
 * Functions called to make the controllers more readable
 *
 * @return {long} v controller Id
 *
 * @public
 */
export function v = CONTROLLER_PRECISE           {v = 0;}
export function v = CONTROLLER_FAST              {v = 1;}
export function v = CONTROLLER_SLOW              {v = 2;}
export function v = CONTROLLER_BLIND             {v = 3;}
export function v = CONTROLLER_PIECEWISE         {v = 4;}
export function v = CONTROLLER_GENERIC_FORKS     {v = 5;}
export function v = CONTROLLER_GENERIC_SIDESHIFT {v = 6;}

/*
 * Functions called to make the controllers more readable
 *
 * @return {long} v controller Id
 *
 * @public
 */
export function sensorName = actuator2encoder(actuator)
{
    switch(actuator)
    {
        case(0);
            if(isVNA())
            {
                sensorName = "encoderVerticalForks";
            }
        case(3);
        case(4);
        case(5);
            sensorName = "encoderAuxiliaryMastForks";
        case(6);
        case(8);
    }
}
