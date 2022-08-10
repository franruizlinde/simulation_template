/**
 * Function to init the translation DB
 *
 * @public
 */
export function translationsDBInit()
{
    allLanguages = getAllAvailableLanguages();

    def = varspc();
    def->id = 0;
    def->tag = strn(""); // key used in project code to recognise the message type (max 31 char)

    k = for(allLanguages)
    {
        setVar(def, allLanguages.(k), {""});
        logInstall('[translationsDBInit] Adding language %s', allLanguages.(k));
    }

    def->errorCategory = 0; // error category = 0 if translation is not an error message
                            //                = 1 if translation is an Client error
                            //                = 2 if translation is a BalyoShort error
                            //                = 3 if translation is a BalyoLong error

    res = DBcreate_gbl("translationsDB", def, "id", 0);

    if (res == 0)
    {
        fillTranslationsDB();
    }
}

/**
 * List all managed languages
 *
 * @return {name} list of all languages tags (same as the list of possible values for the pref general/language)
 * @public
 */
export function out = getAllAvailableLanguages()
{
    out = strn('FR','EN','US','IT','DE','ES','CH', 'PT', 'PL', 'SV', 'CZ', 'HU');
}

export function initTranslationIfNeeded()
{
    try
    {
        DBgetAllKeys_gbl("translationsDB");
    }
    catch
    {
        logInstall("[TRANSLATIONS] - initialisation of DB");
        translationsDBInit();
        return;
    }
    logInfo("[TRANSLATIONS] - DB is already loaded");
}

/**
 * Function to translate a tag into another language
 *
 * @param {string} tag Tag to translate
 * @param {string} requestedLanguage Language to translate into
 *
 * @return {string} Translated tag
 * @return {long} Error code
 *
 * @public
 */
export function [outString, errCode, outKey] = translate(tag, requestedLanguage)
{
    loadedLanguage = getDefaultLanguage();
    if (nargin<2)
    {
        if (isRob)  requestedLanguage = call("getSelectedLanguage");
        else        requestedLanguage = loadedLanguage;
    }

    easterEgg = 0;
    if (strcmp(requestedLanguage, "BE") == 0)
    {
        language = "FR";
        easterEgg = 1;
    }

    outString = tag;
    errCode = 1;

    keysFound = DBfind_gbl("translationsDB", "tag", "==", tag);
    if (!size(keysFound))
    {
        logWarn ("[TRANSLATIONS] - missing case : %s", tag);
        errCode = 2;
        outKey = -1;
        return;
    }
    else
    {
        outKey = keysFound.(0);
    }

    translationVar = DBget_gbl("translationsDB", outKey);

    isAnError = translationVar->errorCategory !=0;

    outString = getVar(translationVar, requestedLanguage).{0};
    if (!size(outString))
    {
        logWarn ("[TRANSLATIONS] - missing case : %s in %s", tag, requestedLanguage);
        outString = getVar(translationVar, "EN").{0};
        errCode = 3;
    }

    if (!size(outString))
    {
        logWarn ("[TRANSLATIONS] - default missing case ! : %s ", tag);
        outString = tag;
        errCode = 4;
    }

    if (isAnError && strcmp(requestedLanguage, loadedLanguage) != 0)
    {
        textInLoadedLanguage = getVar(translationVar, loadedLanguage).{0};
        if (size(textInLoadedLanguage))
        {
            outString = [outString, "\n", textInLoadedLanguage];
        }
        else if (errCode != 3 && size(getVar(translationVar, "EN").{0}))
        {
            outString = [outString, "\n", getVar(translationVar, "EN").{0}];
        }
        else if (errCode != 4)
        {
            outString = [outString, "\n", tag];
        }
    }

    if (easterEgg)
    {
        outString = [outString, ", une fois"];
    }

    if (strcmp(requestedLanguage, "EN") && strcmp(requestedLanguage, "US") && strcmp(requestedLanguage, "FR") && translationVar->errorCategory !=0)
    {
        logError("%s", getVar(translationVar, "EN").{0});
    }
}

/**
 * Function to add a translation into the DB
 *
 * @param {varspc} var Varspc to add to the database
 *
 * @public
 */
export function translationAdd(var)
{
    key = DBfind_gbl("translationsDB", "tag", "==", var->tag );

    if (size(var->tag))
    {
        if (!size(key))
        {
            DBadd_gbl( "translationsDB", var);
        }
        else
        {
            logWarn("key %s exists in multiple versions", var->tag);
            DBaddOrSet_gbl("translationsDB", key, var);
        }
    }
}

/**
 * Function to get the error message list from the translationDB
 *
 * @return {Cell} Error message list
 *
 * @public
 */
export function errMsgList = getErrMsgListFromTranslationDB()
{
    errMsgList = {};
    allKeys = DBfind_gbl("translationsDB", "errorCategory", ">", 0);
    k = for (allKeys)
    {
        errMsgList.{end+1} = translate(DBget_gbl("translationsDB", allKeys.(k), "tag"));
    }
}

/**
 * shortcut for the preferenceValue general/language
 *
 * @return {Cell} Error message list
 *
 * @public
 */
export function out = getDefaultLanguage()
{
    global g("savedResults");
    if (isvar(g, 'language'))
    {
        unint out = g->language;
    }
    else
    {
        out = getPreferenceValue("general/language");
        unint g->language = out;
    }
}

/**
 * Permet de transmettre l'erreur produite sur l'AGV jusqu'au PCsol. Il faut dÃ©finir la liste de toutes les erreurs ajoutÃ©es dans le code projet et mettre mettre une partie unique du message.
 *
 * @return {cell} Liste des erreurs definies dans le projet
 *
 * @public
 */
export function errMsgList = errMsgListDef()
{
    global errMsg ("errMsgList_gbl");

    // if the list has never been build, then build it out of
    if (!isvar(errMsg, "list"))
    {
        initTranslationIfNeeded();

        errMsgList = acall("getErrMsgListFromTranslationDB");
        unint errMsg->list = errMsgList;
    }
    else
    {
        unint errMsgList = errMsg->list;
    }
}
