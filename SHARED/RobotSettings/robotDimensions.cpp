/**
 * Function to get the distance between rollers and backrest when deployed
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_ROLLERS_BACKREST_DEPLOYED(robId)
{
    if (nargin<1)
    {
        if(robDimensions()->rollerToBackrestDeployed != 0.)
            out = robDimensions()->rollerToBackrestDeployed;
        else
            out = robDimensions()->rollerToBackrest;
    }
    else
    {
        if(robDimensions(robId)->rollerToBackrestDeployed != 0.)
            out = robDimensions(robId)->rollerToBackrestDeployed;
        else
            out = robDimensions(robId)->rollerToBackrest;
    }
}

/**
 * Function to get the distance between rollers and backrest
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_ROLLERS_BACKREST(robId)
{
    if (nargin<1)
    {
        out = robDimensions()->rollerToBackrest;
    }
    else
    {
        out = robDimensions(robId)->rollerToBackrest;
    }
}

/**
 * Function to get the distance between rollers and the tip of the forks
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_ROLLERS_ENDFRK(robId)
{
    if (nargin<1)
    {
        out = robDimensions()->rollerToForksEnd;
    }
    else
    {
        out = robDimensions(robId)->rollerToForksEnd;
    }
}

/**
 * Function to get the distance between rollers and the end of the empty safety field
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_ROLLERS_EMPTYFIELD(robId)
{
    if (nargin<1)
    {
        out = robDimensions()->rollerToEndEmptyField;
    }
    else
    {
        out = robDimensions(robId)->rollerToEndEmptyField;
    }
}

/**
 * Function to get the distance between rollers and the end of the carry safety field
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_ROLLERS_CARRYFIELD(robId)
{
    if (nargin<1)
    {
        out = robDimensions()->rollerToEndCarryField;
    }
    else
    {
        out = robDimensions(robId)->rollerToEndCarryField;
    }
}

/**
 * Function to get the distance between backrest and the end of the empty safety field
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_DPLY_BACKREST_EMPTYFIELD(robId)
{
    if (nargin<1 && isRob)      robId = getRobId();
    else if (nargin<1)          robId = 1;

    if(DIST_ROLLERS_BACKREST_DEPLOYED() != 0.0)
        distRollerBackrest = DIST_ROLLERS_BACKREST_DEPLOYED(robId);
    else
        distRollerBackrest = DIST_ROLLERS_BACKREST(robId);

    out = - distRollerBackrest + DIST_ROLLERS_EMPTYFIELD(robId) ;
}

/**
 * Function to get the distance between backrest and the end of the empty safety field
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_BACKREST_EMPTYFIELD(robId)
{
    if (nargin<1)
    {
        out = - DIST_ROLLERS_BACKREST() + DIST_ROLLERS_EMPTYFIELD();
    }
    else
    {
        out = - DIST_ROLLERS_BACKREST(robId) + DIST_ROLLERS_EMPTYFIELD(robId);
    }
}

/**
 * Function to get the distance between backrest and the end of the carry safety field
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_DPLY_BACKREST_CARRYFIELD(robId)
{
    if (nargin<1 && isRob)      robId = getRobId();
    else if (nargin<1)          robId = 1;

    if(DIST_ROLLERS_BACKREST_DEPLOYED() != 0.0)
        distRollerBackrest = DIST_ROLLERS_BACKREST_DEPLOYED(robId);
    else
        distRollerBackrest = DIST_ROLLERS_BACKREST(robId);

    out = - distRollerBackrest + DIST_ROLLERS_CARRYFIELD(robId) ;
}

/**
 * Function to get the distance between backrest and the end of the carry safety field
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_BACKREST_CARRYFIELD(robId)
{
    if (nargin<1)
    {
        out =  - DIST_ROLLERS_BACKREST() + DIST_ROLLERS_CARRYFIELD();
    }
    else
    {
        out = - DIST_ROLLERS_BACKREST(robId) + DIST_ROLLERS_CARRYFIELD(robId);
    }
}

/**
 * Function to get the distance between backrest and the tip of the forks
 *
 * @param {long} robId (optional) : Robot id
 *
 * @return {double} distance
 *
 * @public
 */
export function out = DIST_ENDFRK_BACKREST(robId)
{
    if (nargin<1)
    {
        out = DIST_ROLLERS_BACKREST() - DIST_ROLLERS_ENDFRK();
    }
    else
    {
    out = DIST_ROLLERS_BACKREST(robId) - DIST_ROLLERS_ENDFRK(robId);
    }
}