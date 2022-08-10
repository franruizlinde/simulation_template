// end of deviceModDefs
export function setupCustomDevicesConfig(conf)
{
    conf->options->Devices =
    {

        /// Docking station mobile V2
        // varspc('ip', '10.11.121.4', 'port', 1025, 'hardware', 'ModbusTCPDevice', 'ioCount', [0,0,25,5],'ioAddresses', [0,0,0,25], 'position', cpx(-8.5,-8), 'name',"dockingStation", 'DrawDeviceCarto', 0 ),

        /// Charger Enersys example
//        varspc('ip', '10.11.121.20', 'port', 502, 'hardware', 'ModbusTCPDevice', 'ioCount', [0, 0, 6, 1],  'ioAddresses', [0, 0, 0, 0],   'functionCodes', [0, 0, 3, 16], 'position', cpx(-15, -20), 'name', "ChargerEnersys2", 'DrawDeviceCarto', 0),
//        varspc('ip', '10.11.121.20', 'port', 502, 'hardware', 'ModbusTCPDevice', 'ioCount', [0, 0, 16, 0], 'ioAddresses', [0, 0, 110, 0], 'functionCodes', [0, 0, 3, 0],  'position', cpx(-15, -20), 'name', "ChargerEnersys",  'DrawDeviceCarto', 0),

        /// Docking station mobile V2
        // varspc('ip', '10.11.121.4', 'port', 1025, 'hardware', 'ModbusTCPDevice', 'ioCount', [0,0,25,5],'ioAddresses', [0,0,0,25], 'position', cpx(-8.5,-8), 'name',"dockingStation", 'DrawDeviceCarto', 0 ),

        /// Charger Enersys example
//        varspc('ip', '10.11.121.20', 'port', 502, 'hardware', 'ModbusTCPDevice', 'ioCount', [0, 0, 6, 1],  'ioAddresses', [0, 0, 0, 0],   'functionCodes', [0, 0, 3, 16], 'position', cpx(-15, -20), 'name', "ChargerEnersys2", 'DrawDeviceCarto', 0),
//        varspc('ip', '10.11.121.20', 'port', 502, 'hardware', 'ModbusTCPDevice', 'ioCount', [0, 0, 16, 0], 'ioAddresses', [0, 0, 110, 0], 'functionCodes', [0, 0, 3, 0],  'position', cpx(-15, -20), 'name', "ChargerEnersys",  'DrawDeviceCarto', 0),
    
    };
}