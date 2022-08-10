/*

    Function that add tasks field to json content

    - tasks (from schd)

    - flows (from results)

    - robot info (from replay if enabled)

    @param content_in [string] -> content of the json file

    @return content_out [string] -> new content

*/

export function content_out = CUSTOM_addJsonResultData()
{

    // * * * * * * * Title * * * * * * * * * * *

    // * * * * * * * scheduler * * * * * * * * *

    schdData = DBget_gbl('simSchdDB'); // get all tasks
    // check that result data validity
    if (isempty(schdData))
        displayError("[writeJsonResults] Schd DB empty !");
    else if (!isvar(schdData, 'id'))
        displayError("[writeJsonResults] Field 'id' does not exist in schd DB !");
    else if (isempty(schdData->id))
        displayError("[writeJsonResults] Field 'id' is empty in schd DB !");
    // fill the missions from results DB

    content = '{\n\t"tasks": [';
    k = for (schdData->id)
    {

        // Write the content of each key

        elt = (k == 0) ? '\n\t\t\t{' : ',\n\t\t\t{';
        elt = [ elt, '"id": ', num2str(schdData->id.(k)) ];
        // elt = [ elt, ', "missName": "', char(schdData->missName.(k)), '"' ];
        // elt = [ elt, ', "flowName": "', char(schdData->flowName.(k)), '"' ];
        elt = [ elt, ', "missId": ', num2str(schdData->missId.(k)) ];
        elt = [ elt, ', "pgm": "', char(schdData->pgm.(k))];
        elt = [ elt, ', "state": ', num2str(schdData->state.(k)) ];
        elt = [ elt, ', "robAuth": ', tab2str(schdData->robAuth.(k).{0}) ];
        elt = [ elt, ', "rob": ', num2str(schdData->rob.(k)) ];
        elt = [ elt, ', "priority": ', num2str(schdData->priority.(k)) ];
        // elt = [ elt, ', "addTime": ', num2str(schdData->addTime.(k)) ];
        // elt = [ elt, ', "startTime": ', num2str(schdData->startTime.(k)) ];
        elt = [ elt, ', "addDate": ', num2str(schdData->addDate.(k)) ];
        elt = [ elt, ', "startDate": ', num2str(schdData->startDate.(k)) ];
        // elt = [ elt, ', "endTime": ', num2str(schdData->endTime.(k)) ];
        // elt = [ elt, ', "carryTime": ', num2str(schdData->carryTime.(k)) ];
        // elt = [ elt, ', "emptyTime": ', num2str(schdData->emptyTime.(k)) ];
        // elt = [ elt, ', "pickTime": ', num2str(schdData->pickTime.(k)) ];
        // elt = [ elt, ', "dropTime": ', num2str(schdData->dropTime.(k)) ];
        // elt = [ elt, ', "npal": ', num2str(schdData->npal.(k)) ];

        elt = [ elt, '}' ];
        // Add it to the global content

        content = [ content, elt ];
    }

    content = [ content, '\n\t\t]' ]; // Close the content (tasks)

    // Close the content (result data)

    content_out = [ content, '\n\t}' ];
}

/*

    Function that add tasks field to json content

    - tasks (from schd)

    - flows (from results)

    - robot info (from replay if enabled)

    @param content_in [string] -> content of the json file

    @return content_out [string] -> new content

*/

export function content_out = CUSTOM_addJsonPlanData()
{

    // * * * * * * * Title * * * * * * * * * * *

    // * * * * * * * scheduler * * * * * * * * *

    schdData = DBget_gbl('csvDB'); // get all tasks

    // check that result data validity
    if (isempty(schdData))
        displayError("[writeJsonResults] Schd DB empty !");
    else if (!isvar(schdData, 'id'))
        displayError("[writeJsonResults] Field 'id' does not exist in schd DB !");
    else if (isempty(schdData->id))
        displayError("[writeJsonResults] Field 'id' is empty in schd DB !");

    // fill the missions from results DB

    content = '{\n\t"Targettasks": [';
    k = for (schdData->id)
    {
        // Write the content of each key
        elt = (k == 0) ? '\n\t\t\t{' : ',\n\t\t\t{';
        elt = [ elt, '"id": ', num2str(schdData->id.(k)) ];
        elt = [ elt, ', "missName": ', char(schdData->mission.(k)), '"' ];
        // elt = [elt, ', "LaunchTime": ', /*poner el segundo en el que se lanza la mission*/, ""];

        // elt = [ elt, ', "planStart": ', num2str(schdData->cnvSec.(k)) ];
        // elt = [ elt, ', "launched": ', num2str(schdData->LaunchedOk.(k)) ];
        elt = [ elt, '}' ];
        // Add it to the global content
        content = [ content, elt ];
    }

    content = [ content, '\n\t\t]' ]; // Close the content (tasks)
    // Close the content (result data)
    content_out = [ content, '\n\t}' ];
}
/*

    Function to write JSON results files from results DB

    @param file [string] -> name (path) of the results file

*/

export function CUSTOM_writeJsonResults(file)
{

    global g('simConf');
    // if (strcmp(toupper(g->simResultsFile), 'JSON'))
    //     return;
    // fill json content



    content = CUSTOM_addJsonPlanData();
    // Select the file extension
    extension = 'PD.json';
    // Picks the name of the results file and change the .txt to .json just to have the same name of the files
    fileT = strReplace(file, '.txt', extension);
    saveText(fileT, content);
    // fill json content

    content = CUSTOM_addJsonResultData();
    // Select the file extension
    extension = 'RD.json';
    fileT = strReplace(file, '.txt', extension);
    saveText(fileT, content);
    logDebug("[RESULTS] JSONs file created.");
}