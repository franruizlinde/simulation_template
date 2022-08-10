/**
 * Transform a failure tag into an ID so that it can be send via a message
 *
 * @param {string} tag of the failure
 *
 * @return {cell} Id of the failure
 *
 * @public
 */
export function failureId = failureName2Id(failureName)
{
    failureId = -1;
    [_,_,failureId] = translate(failureName);
}

/**
 * Transform a failure ID into an tag
 *
 * @param {string} ID of the failure
 *
 * @return {cell} tag of the failure
 *
 * @public
 */
export function failureName = failureId2Name(key)
{
    failureName = DBget_gbl("translationsDB", key, "tag");
}