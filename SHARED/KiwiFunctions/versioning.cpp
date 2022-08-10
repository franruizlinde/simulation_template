
/**
 * Update the SDK field in every preference file to match the current SDK version
 *
 * @public
*/
export function updateSdkVersionPrefs()
{
    // Load version of current SDK
    currentScmVersionInfo = scmVersionInfo();
    currentSdkVersion = strSplit(currentScmVersionInfo, '-').{0};

    // Load names of all preference files
    prefDir = fnameConc(pwd(), "PREF");
    filesToChange = rls(prefDir);

    successCount = 0;
    failCount = 0;

    i = for(filesToChange)
    {
        try
        {
            // Read content of each preference file
            filePath = filesToChange.{i};
            fileContent = readText(filePath);

            // Extract SDK version currently in prefs
            prefFileSdk = extractTextBetween(fileContent, 'Preferences sdk="', '"').{0};

            // If SDK mismatch, replace old SDK version with new one and save pref file
            if(!isStrEqual(prefFileSdk, currentSdkVersion))
            {
                fileContent = strReplace(fileContent, prefFileSdk, currentSdkVersion);
                saveText(filePath, fileContent);

                successCount = successCount + 1;
            }
        }
        catch
        {
            failCount = failCount + 1;
            logError("[PREF] Error updating SDK version in '%s'", filePath);
        }
    }

    // Log final tally of updated files
    logInstall("[PREF] %i PREF files converted to SDK %s (%i failed)", successCount, currentSdkVersion, failCount);
}
