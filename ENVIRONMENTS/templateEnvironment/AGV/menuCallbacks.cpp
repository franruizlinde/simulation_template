/** 
===========================================
    HMI menu callbacks
===========================================
*/

export function menu = menuOptionsCbk()
{
    menu = strn(

    // Default buttons, no custom callbacks needed
    'Picks',
    'Drops',
    'Move',

    // Callback required for each custom button
    // 'Example Button 123', //replace spaces in button name with _ for callback function

    );
}

// export function menu_Picks()
// {
//     // Optional callback to overwrite behavior of default buttons
// }

export function menu_Example_Button_123()
{
    logInfo('[MENU] Example button selected');
}