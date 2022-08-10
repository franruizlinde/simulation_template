/**
 * Switch the current environment to an existing one
 *
 * @param {string} name of the existing environment to switch to
 *
 * @public
*/
export function switchEnvironment(newEnvironment)
{
    // Check if environment exists
    exists = !isempty(findElt(getAllEnvironments(),newEnvironment));

    // If environment exists, update current environment in _environment.obj
    // TODO: integrate with _envProjectViewer.obj in kiwiEditor
    if(exists)
    {
        v = varspc();
        v->current = newEnvironment;
        save('_environment.obj',v);
        logInstall('[switchEnvironment] Active environment set to %s',newEnvironment);
        // Copy over install and comp files of current environment
        prepareEnvForCompilation();
    }
    else
    {
        logWarn('[switchEnvironment] Environment %s does not exist!',newEnvironment);
    }
}

/**
 * Create a new environment, with an existing environment as a template
 *
 * @param {string} name of the new environment
 * @param {string} (optional) name of the existing environment to be used as the template for the new one (templateEnvironment by default)
 *
 * @public
*/
export function createNewEnvironment(newEnvironment, templateEnvironment)
{
    // Use 'templateEnvironment' as basis if no environment specified
    if(nargin < 2)
    {
        templateEnvironment = 'templateEnvironment';
    }

    // Determine if new/template environment exist and react accordingly
    existingReference = findElt(getAllEnvironments(), templateEnvironment);
    existingNew = findElt(getAllEnvironments(), newEnvironment);

    if(!isempty(existingNew))
    {
        logWarn('[createNewEnvironment] Environment already exists! Doing nothing.');
        return;
    }
    if(isempty(existingReference))
    {
        logWarn('[createNewEnvironment] Referenced environment does not exist! Doing nothing.');
        return;
    }

    // Copy environment folders from template to new environment
    envDirs = getEnvironmentDirs();

    i = for(envDirs)
    {
        templateDir = fnameConc(envDirs.{i}, templateEnvironment);
        newDir = fnameConc(envDirs.{i}, newEnvironment);

        copyDir(templateDir, newDir);
    }

    prefDir = fnameConc(pwd(),'PREF');

    // Get file path of new and template environment install prefs
    newInstallFile = sprintf('install_%s.xml', newEnvironment);
    newInstallPath = fnameConc(prefDir, newInstallFile);
    templateInstallFile = sprintf('install_%s.xml', templateEnvironment);
    templateInstallPath = fnameConc(prefDir, templateInstallFile);

    // Use existing install.xml as template if template environment does not have a specific install pref
    if(!isfile(templateInstallPath))
    {
        templateInstallPath = fnameConc(prefDir, 'install.xml');
    }

    // Copy template install prefs to new environment name
    copyfile(templateInstallPath, newInstallPath, 0);

    // Change pref in new install file to match new environment (do we even need this pref anymore?)
    newEnvPref = {"general/environment", newEnvironment};
    updatePreferenceFile(newEnvPref, newInstallPath, "install");

    // Get file path of new and template environment compFiles file
    newCompFile = sprintf('compilationFiles_%s.txt', newEnvironment);
    newCompFilePath = fnameConc(pwd(), newCompFile);
    templateCompFile = sprintf('compilationFiles_%s.txt', templateEnvironment);
    templateCompFilePath = fnameConc(pwd(), templateCompFile);

    // Use existing compilationFiles.txt as template if template environment does not have a specific compFiles file
    if(!isfile(templateCompFilePath))
    {
        templateCompFilePath = fnameConc(pwd(), 'compilationFiles.txt');
    }

    // Copy template compFiles file to new environment name
    copyfile(templateCompFilePath, newCompFilePath, 0);

    // Change compiled environment files to match new environment
    replaceTextInFile(newCompFilePath,sprintf('\\%s\\',templateEnvironment),sprintf('\\%s\\',newEnvironment));

    // Switch to new environment
    switchEnvironment(newEnvironment);
}

/**
 * Get a list of all environments in the current project
 *
 * @return {L.string} list of environment names
 *
 * @public
*/
export function allEnvironments = getAllEnvironments()
{
    environmentDirs = getEnvironmentDirs();
    allEnvironments = strn();

    i = for(environmentDirs)
    {
        [_, dirs] = ls(environmentDirs.{i});
        d = for(dirs)
        {
            dirName = strn(dirs.{d});
            if(size(findElt(allEnvironments, dirName)) == 0)
            {
                allEnvironments.(end+1) = dirName;
            }
        }
    }
}


/**
 * Get the file paths of every directory that contains environment folders
 *
 * @return {cell} list of environment directories
 *
 * @public
*/
export function environmentDirs = getEnvironmentDirs()
{
    projectDir = pwd();
    environmentDirs = {};

    environmentDirs.{end+1} = fnameConc(projectDir,'AGV','05_Environments');
    environmentDirs.{end+1} = fnameConc(projectDir,'SUPERVISOR','Environments');
    environmentDirs.{end+1} = fnameConc(projectDir,'SHARED','Environments');
    environmentDirs.{end+1} = fnameConc(projectDir, 'ENVIRONMENTS');
}


/**
 * Copy install prefs and compilation files specific to the current environment into install.xml and compilationFiles.txt (with backups)
 *
 * @public
*/
export function prepareEnvForCompilation()
{
    [environment, savedLocally] = getCurrentEnvironment();
    if(!savedLocally)
    {
        logInstall('[prepareEnvForCompilation] No _environment.obj file found. Doing nothing.');
        return;
    }

    prefDir = fnameConc(pwd(),'PREF');

    // Prepare install.xml
    installEnvName = sprintf('install_%s.xml',environment);
    installEnvPath = fnameConc(prefDir, installEnvName);

    installFilePath = fnameConc(prefDir, 'install.xml');
    installBackupPath = fnameConc(prefDir, 'backup_install.xml');

    if(isfile(installEnvPath))
    {
        if(isfile(installFilePath)) copyfile(installFilePath, installBackupPath, 1);
        copyfile(installEnvPath, installFilePath, 1);
    }

    // Prepare compilationFiles.txt
    compFileEnvName = sprintf('compilationFiles_%s.txt',environment);
    compFileEnvPath = fnameConc(pwd(), compFileEnvName);

    compFilePath = fnameConc(pwd(), 'compilationFiles.txt');
    compFileBackupPath = fnameConc(pwd(), 'backup_compilationFiles.txt');

    if(isfile(compFileEnvPath))
    {
        if(isfile(compFilePath)) copyfile(compFilePath, compFileBackupPath, 1);
        copyfile(compFileEnvPath, compFilePath, 1);
    }
}


/**
 * Get the currently open environment of the project
 *
 * @return {string} currently open environment
 * @return {int} returns 1 if environment is saved as a local .obj; 0 if only present in install prefs
 *
 * @public
*/
export function [currentEnvironment, locallySaved] = getCurrentEnvironment()
{
    environmentFileName = '_environment.obj';
    if(isfile(fnameConc(pwd(),environmentFileName)))
    {
        env = load(environmentFileName);
        currentEnvironment = env->current;
        locallySaved = 1;
    }
    else
    {
        currentEnvironment = getPreferenceValue('general/environment');
        locallySaved = 0;
    }
}