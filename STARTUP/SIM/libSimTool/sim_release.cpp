export function version = simToolGetVersion()
{
    versionMajor = 4;
    versionMinor = 2;
    versionPatch = 2;
    version = sprintf('%i.%i.%i', versionMajor, versionMinor, versionPatch);
}
export function version = simToolGetGitInfo()
{
    global g('appData');
    version = g->libSimTool->gitVersion;
}