/**
 * Compares two string and returns 1 if equal, 0 if not. Equivalent of !strcmp()
 *
 * @param {string} string1 First string to compare
 * @param {string} string2 Second string to compare
 *
 * @return {long} 1 if strings are equal, 0 otherwise
 *
 * @public
*/
export function ans = isStrEqual(string1,string2)
{
    ans = !strcmp(string1,string2);
}

/**
 * returns if an element can be found in a list of elements
 *
 * @param {L.obj} List to investigate
 * @param {obj} value to find in the list
 *
 * @return {long} 1 if object is found in the list, 0 otherwise
 *
 * @public
*/
export function out = isInList(list, valueToFind)
{
    out = size(findElt(list, valueToFind));
}

/**
 * Opposite of sliceVarspc : fuse the 2 varspc together without loosing all informations (not like mergeVarspc which gives priority to one var over the other one)
 *
 * @param {varspc} var1 first varspc element to merge
 * @param {varspc} var2 second varspc element to merge
 *
 * @return {varspc} outVarspc varspc containing lists of each values
 *
 * @public
*/
export function outVarspc = fuseVarspc(var1, var2)
{
    outVarspc = varspc();
    lName1 = sort(var1.name);
    lName2 = sort(var2.name);

    inBuildVarspc = varspc();
    if (size(lName1)==size(lName2))
    {
        k = for (lName1)
        {
            fieldName = lName1.(k);
            if (strcmp(fieldName, lName2.(k)))
            {
                logError("Both varspc do not have identical field names");
                return;
            }
            if (strcmp(getType(getVar(var1, fieldName)),getType(getVar(var2, fieldName))))
            {
                logError("Both varspc do not have identical structures");
                return;
            }
            setVar(inBuildVarspc, fieldName, [getVar(var1,fieldName), getVar(var1,fieldName)]);
        }
        outVarspc = inBuildVarspc;
    }
    else
    {
        logError("Both varspc do not have the same fields");
    }
}

/**
 * Similar to dropElt but working on name objects - Erase from listToClean the name in objectToClean
 *
 * @param {L.name}  listToClean   : List of Name to be cleaned
 * @param {name}    objectToClean : Name to erase in the listToClean
 *
 * @return {L.name} cleanedList   : listToClean without objectToClean
 *
 * @public
*/
export function cleanedList = dropEltName(listToClean, objectToClean)
{
    cleanedList = listToClean;
    indexes = findElt(listToClean, objectToClean);
    cleanedList.(indexes) = [];
}

/**
 * Similar to setUnion but working on name objects - Set Union of list 1 and 2
 *
 * @param {L.name} list1  : First List
 * @param {L.name} list2  : Second List
 *
 * @return {L.name} out   : List 1 and 2 Union
 *
 * @public
*/
export function out = setUnionName(list1, list2)
{
    //Input verification - Inputs should be l.name
    if (!strMatch(getType(list1),"name") || !strMatch(getType(list2),"name") )
    {
        error("Argument 0 and 1 should be lists of names");
    }

    out = strn();
    k = for (list1)
    {
        if (!size(findElt(out, list1.(k))))
        {
            out.(end+1) = list1.(k);
        }
    }
    k = for (list2)
    {
        if (!size(findElt(out, list2.(k))))
        {
            out.(end+1) = list2.(k);
        }
    }
}

/**
 * Similar to setDiff but working on name objects - Remove Names in list2 from list1
 *
 * @param {L.name} list1  : First List
 * @param {L.name} list2  : Second List
 *
 * @return {L.name} out   : List 1 and 2 diff
 *
 * @public
*/
export function out = setDiffName(list1, list2)
{
    //Input verification - Inputs should be l.name
    if (!strMatch(getType(list1),"name") || !strMatch(getType(list2),"name") )
    {
        error("Argument 0 and 1 should be lists of names");
    }

    out = list1;
    k = for(list2)
    {
        out = dropEltName(out, list2.(k));
    }
}

/**
 * Similar to setInter but working on name objects - returns a list of names present in both list1 and list2
 *
 * @param {L.name} list1  : First List
 * @param {L.name} list2  : Second List
 *
 * @return {L.name} out   : List 1 and 2 intersection
 *
 * @public
*/
export function out = setInterName(list1, list2)
{
    //Input verification - Inputs should be l.name
    if (!strMatch(getType(list1),"name") || !strMatch(getType(list2),"name") )
    {
        error("Argument 0 and 1 should be lists of names");
    }

    out = strn();
    k = for(list1)
    {
        if (size(findElt(list2, list1.(k))))
        {
            out.(end+1) = list1.(k);
        }
    }
}

/**
 * Similar to unique but working on names - returns a list of names without any duplaicate
 *
 * @param {L.name} listOfName  : List to be filtered
 *
 * @return {L.name} listOfUniqueNames   : Filtered list
 *
 * @public
*/
export function listOfUniqueNames = uniqueName(listOfName)
{
    listOfUniqueNames = strn();
    k = for (listOfName)
    {
        if (!size(findElt(listOfUniqueNames, listOfName.(k))))
        {
            listOfUniqueNames.(end+1) = listOfName.(k);
        }
    }
}

/**
 * Extract the substring between two tags within a string
 *
 * @param {string} the text content to search for the substring
 * @param {string} start tag
 * @param {string} end tag
 *
 * @return {cell} text contained between the two tags (multiple instances possible)
 * @return {cell} text between and containing the two tags (multiple instances possible)
 *
 * @public
*/
export function [textBetween, textInclusive] = extractTextBetween(textContent, textStartTag, textEndTag)
{
    // Initialize output lists
    textBetween = {};
    textInclusive = {};

    // Find all occurrences of start and end tags
    allStarts = strfind(textContent, textStartTag);
    allEnds = strfind(textContent, textEndTag);

    // Return if no start or no end found
    if(!size(allStarts) || !size(allEnds))
    {
        return;
    }

    // Start search index at start of file
    tagEnd = -1;

    // Iterate through pairs of start/end tags until invalid index
    // text-start-between-end-text
    //      ^     ^     ^   ^     <-- key indices
    while(1)
    {
        nextStartIdx = find(allStarts > tagEnd);            // Find next indexed start tag after last end tag
        if(isempty(nextStartIdx)) return;                   // Return if no possible next index
        tagStart = allStarts.(nextStartIdx).(0);            // Next start tag location found after previous end tag is recorded
        betweenStart = tagStart + size(textStartTag);       // Text between tags starts after end of start tag

        nextEndIdx = find(allEnds >= betweenStart);         // Find next indexed end tag after start of between-text
        if(isempty(nextEndIdx)) return;                     // Return if no possible next index
        betweenEnd = allEnds.(nextEndIdx).(0) - 1;          // End of between text is immediately before start of next end tag
        tagEnd = betweenEnd + size(textEndTag);             // Record last position of end tag

        // Store characters from content between indexed locations
        textBetween.{end+1} = textContent.(lin(betweenStart, betweenEnd));
        textInclusive.{end+1} = textContent.(lin(tagStart, tagEnd));
    }
}

/**
 * Replace all instances of a string within a file
 *
 * @param {string} file path of the file to replace the text within
 * @param {string} text currently within the file, to be replaced
 * @param {string} text to replace the existing text with
 *
 * @public
*/
export function replaceTextInFile(filePath,oldText,newText)
{
    fileContent = readText(filePath);
    fileContent = strReplace(fileContent, oldText, newText);
    saveText(filePath, fileContent);
}

/**
 * Remove a single cell (by index) from a cell list
 *
 * @param {cell} existing cell list
 * @param {int} index of the cell to be removed
 *
 * @return {cell} cell list with the appropriate cell removed
 *
 * @public
*/
export function newCells = removeCell(cells, idxRemove)
{
    idx = lin(0, size(cells) - 1);
    idx = dropElt(idx, idxRemove);
    newCells = cells.(idx);
}

/**
 * Move a single cell in a cell list from one index to another, shifting the other cells accordingly
 *
 * @param {cell} existing cell list
 * @param {int} index of the cell to be moved
 * @param {int} new index of the moved cell
 *
 * @return {cell} cell list with the appropriate cell repositioned
 *
 * @public
*/
export function newCells = moveCell(cells, idxMove, newIdx)
{
    // Initialize output cell list
    newCells = {};

    // Put moving cell into temporary variable and remove cell from old location
    movedCell = cells.{idxMove};
    inserted = 0;

    cells = removeCell(cells, idxMove);

    // Iterate through existing cell list
    i = for(cells)
    {
        // If index matches new index of moving cell, re-insert cell into list
        if(i == newIdx)
        {
            newCells.{end+1} = movedCell;
            inserted = 1;
        }

        // Otherwise, just move all cells over to new list
        newCells.{end+1} = cells.{i};
    }

    // If new index was never reached, put the cell at the end of the new list
    if(!inserted)
    {
        newCells.{end+1} = movedCell;
    }
}

/**
 * Function to remove all special character from a message, as they might not be translatable into ASCII
 *
 * @param {char} input message
 *
 * @return {char} output message, without special characters
 *
 * @public
 */
export function msg = removeSpecialChar(msg)
{
    //Build a list of common "special" characters in latin languages with special character as first element, and ASCII equivalent as second element
    specialCharList = {};
    specialCharList.{end+1} = {"é", "e"};
    specialCharList.{end+1} = {"ê", "e"};
    specialCharList.{end+1} = {"ë", "e"};
    specialCharList.{end+1} = {"É", "E"};
    specialCharList.{end+1} = {"È", "E"};
    specialCharList.{end+1} = {"à", "a"};
    specialCharList.{end+1} = {"â", "a"};
    specialCharList.{end+1} = {"ä", "a"};
    specialCharList.{end+1} = {"Å", "A"};
    specialCharList.{end+1} = {"À", "A"};
    specialCharList.{end+1} = {"ö", "o"};
    specialCharList.{end+1} = {"ô", "o"};
    specialCharList.{end+1} = {"Ö", "O"};
    specialCharList.{end+1} = {"Ô", "O"};
    specialCharList.{end+1} = {"ç", "c"};
    specialCharList.{end+1} = {"Ç", "C"};
    specialCharList.{end+1} = {"û", "u"};
    specialCharList.{end+1} = {"ü", "u"};
    specialCharList.{end+1} = {"ù", "u"};
    specialCharList.{end+1} = {"ú", "u"};
    specialCharList.{end+1} = {"Ù", "U"};
    specialCharList.{end+1} = {"Ú", "U"};
    specialCharList.{end+1} = {"Ü", "U"};
    specialCharList.{end+1} = {"Û", "U"};
    specialCharList.{end+1} = {"î", "i"};
    specialCharList.{end+1} = {"ï", "i"};
    specialCharList.{end+1} = {"í", "i"};
    specialCharList.{end+1} = {"ì", "i"};
    specialCharList.{end+1} = {"Î", "I"};
    specialCharList.{end+1} = {"Ï", "I"};
    specialCharList.{end+1} = {"Ì", "I"};
    specialCharList.{end+1} = {"Í", "I"};
    specialCharList.{end+1} = {"ñ", "n"};
    specialCharList.{end+1} = {"Ñ", "N"};
    specialCharList.{end+1} = {"ß", "ss"};
    specialCharList.{end+1} = {"ẞ", "ss"};
    specialCharList.{end+1} = {"œ", "oe"};
    specialCharList.{end+1} = {"Œ", "Oe"};

    //Replace every found special character by its equivalent in the input message
    k = for(specialCharList)
    {
        if(!isempty(strfind(msg, specialCharList.{k}.{0})))
        {
            msg = strReplace(msg, specialCharList.{k}.{0}, specialCharList.{k}.{1});
        }
    }
}

/**
 * Function to wait until the named DB is loaded (prevent crash if module are not launched in the right sequence)
 *
 * @param {char} DB name
 *
 * @public
 */
export function waitForInitialisationOfDB(DBname)
{
    initialTime = time();
    TMO = 6;
    while ((time - initialTime) < TMO)
    {
        try
        {
            DBgetKeyField_gbl(DBname);
            break;
        }
        catch
        {
            wait(300);
            logWarn("waiting for DB %s to be initialized", DBname);
        }
    }
}