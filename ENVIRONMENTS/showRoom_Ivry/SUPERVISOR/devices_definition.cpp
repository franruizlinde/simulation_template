// end of deviceModDefs
export function setupCustomDevicesConfig(conf)
{
    conf->options->Devices =
    {
        /// Conveyor
		varspc("id", COMBOX_CONVEYOR(), 'ip', '10.11.121.1', 'hardware', 'ADAM6050', 'ioCount', 8, 'position', cpx(73,20), 'name',"Conveyor", 'DrawComBoxCarto', 1 ),
		//varspc('ip', '10.10.250.2', 'hardware', 'ADAM6050', 'ioCount', [3,3], 'position', cpx(63,20), 'name',"Doors", 'DrawComBoxCarto', 1 ),

        /// Docking station mobile V1
		// varspc('ip', '10.11.121.2', 'port', 502, 'hardware', 'ModbusTCPDevice', 'ioCount', [0,0,25,5],'ioAddresses', [0,0,1100,2099],'functionCodes',[2,5,3,16], 'position', cpx(-8.5,-8), 'name',"dockingStation", 'DrawDeviceCarto', 0 ),

		/// Docking station mobile V2
		varspc("id", COMBOX_DOCKING(), 'ip', '10.11.121.3', 'port', 1025, 'hardware', 'ModbusTCPDevice', 'ioCount', [0,0,25,5],'ioAddresses', [0,0,0,25], 'position', cpx(-8.5,-8), 'name',"dockingStation", 'DrawDeviceCarto', 0 ),
        
    };
}