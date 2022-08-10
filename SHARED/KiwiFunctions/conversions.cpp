/**
 * Convert a point (kiwi structure that comes out of circuit editor mainly) into a complex
 *
 * @param {point} pointStruct the point to convert
 *
 * @return {cpx} the x,y position of the point into a complex
 *
 * @public
*/
export function z = point2cpx(pointStruct)
{
    z = cpx(pointStruct.xposition, pointStruct.yposition);
}

/**
 * Convert a cpx into a point (kiwi structure that comes out of circuit editor mainly)
 *
 * @param {cpx} the x,y position to convert
 * @param {double} cap of the point
 *
 * @return {point}
 *
 * @public
*/
export function pointStruct = cpx2point(z, cap)
{
    if (nargin < 2)
    {
        trackPosition = TRACK_getTrackPosition(myLayer(), z);
        cap = TRACK_getPortionInfoFromID(myLayer(), trackPosition.portionId).cap;
    }

    pointStruct = new("point",1);
    pointStruct.xposition = z.re;
    pointStruct.yposition = z.im;
    pointStruct.cap = cap;
}

/**
 * Fonction that format a barcode into an array of long ready to be sent by radio
 *
 * @param {name} code barcode to format
 *
 * @return {L.long} Array (each element correspond to 4 char of the barcode
 *
 * @public
 */
export function args = BCD_code2Args(code)
{
    // Compute how many arguments are going to be necessary to send the barcode
    nbPart = size(code)/4;
    if (size(code)%4 != 0)
    {
        nbPart = nbPart + 1;
    }

    // Varibale initialisation
    args = [];
    // Format all arguments
    k = for(0,nbPart - 1)
    {
        if ((k == nbPart-1) && (size(code)%4 > 0))
        {
            args.(end+1) = str2num(["0x",str2hex(code.(lin(4*k,size(code)-1)))]);
        }
        else
        {
            args.(end+1) = str2num(["0x",str2hex(code.(lin(4*k,4*k+3)))]);
        }
    }

    // Complete the rest of the table with zeros
    if (nbPart < 8)
    {
        i = for(0,8 - nbPart - 1)
        {
            args.(end+1) = 0;
        }
    }
    logInfo('%z',args);
}

/**
 * Function that rebuild an alphanumerical barcode out of a list of array
 *
 * @param {L.long} arrayData Table containing every numerical data to be converted in the bar code (as received through the radio messages)
 *
 * @return {string} barcode Barcode re-assembled
 *
 * @public
 */
export function barcode = BCD_args2Code(arrayData)
{
    // initialize variables
    barcode = [];
    argToStr = [];

    // Loop on every piece of barcode to reconstruct
    i = for(0, size(arrayData)-1)
    {
        j = for(0, 3)
        {
            // We try to convert the piece into a string. If it fails, it means the end of the barcode is reached
            try
            {
                str = char((arrayData.(i) >> 8*j) & 0xFF);
            }
            catch
            {
                str = '';
            }

            // if no error is catched, then the piece of string is added to the currently being build half of barcode
            // each pice to be added is what is provided by one argument of one radio message
            if (strcmp(str,''))
            {
                argToStr = [str,argToStr];
            }
        }

        // Then we add the pieces of barCode together
        // each piece to be added is what is provided by one radio message
        barcode = [barcode, argToStr];
        argToStr = [];
    }

    logInfo('[BARCD] Re-assembled barcode: %s', barcode);
}

/**
 * Converts a cell containing a unique string into a string
 *
 * @param {cell} cell
 *
 * @return {string} string within
 *
 * @public
 */
export function out = cell2str(in)
{
    out = in.{0};
}

/**
 * Converts a cell containing a unique string into a string
 *
 * @param {string} string
 *
 * @return {cell} cell
 *
 * @public
 */
export function out = str2cell(in)
{
    out = {in};
}