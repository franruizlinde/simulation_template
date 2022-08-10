export function endMissPick_customBehaviour(taskId, robId, pointData, positionInLine, manualValidation)
{
    STOCK_updateAfterPick(pointData);
}

export function endMissDrop_customBehaviour(taskId, robId, pointData, positionInLine, manualValidation)
{
    STOCK_updateAfterDrop(pointData);
}