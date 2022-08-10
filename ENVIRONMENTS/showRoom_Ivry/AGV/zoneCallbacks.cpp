// ***************************************
// 	AGV ZONES MANAGING
    
//     Create zones "agv_xxx" in simx
//     For Group zones, names zones "agv_xxx_yy"
//     For zones used both in PCsol AND AGV, name zones "agv-pc_xxxx"
    
//     In project code, you can use 4 functions relatives to actions using these zones :
//         - zone_xxxx_outside         Launched while AGV is outside of the zone
//         - zone_xxxx_inside          Launched while AGV is inside the zone
//         - zone_xxxx_entering        Launched once when AGV enters the zone
//         - zone_xxxx_leaving         Launched once when AGV leaves the zone
        
//     EXAMPLE
//     - SIMX : create 3 zones for Horn :
//         agv_horn_1
//         agv_horn_2
//         agv_horn_3
        
//     - PROJECT CODE : define function which make horn the AGV when entering these zones
 export function zone_horn_entering()
 {
//	 setHorn(1000);
 }

