export function setupInitialStateOfShowRoom()
{
    // Set outputs of the conveyor to the initial conditions
	deviceSetOutputDataByNumber(COMBOX_CONVEYOR(), 1, 1);
	deviceSetOutputDataByNumber(COMBOX_CONVEYOR(), 2, 1);
    logInfo('Conveyor initialized : %d, %d', deviceGetOutputDataByNumber(COMBOX_CONVEYOR(),1), deviceGetOutputDataByNumber(COMBOX_CONVEYOR(),2));
    
    // Initialisation des compteurs stock
	global stock('compteurs');	
	stock->nbPalletteStk1 = 0;			
    stock->nbPalletteStk2 = 0;
	stock->nbPalletteStk3 = 0;
	stock->compteurConv = 0;
	stock->nbPaletteSol = 0;
}

export function STOCK_updateAfterPick(pointData)
{
	//On checke les compteurs
	global stock('compteurs');	
    
     switch(pointData->name)
    {	
       //Gestion des freeSpace En Rack
	   case("400400");
			freeSpace400400 = 1;
            logInstall("[ONEVENT-] emplacement 400400 free");
		break;
		case("400100");
            freeSpace400100 = 1;
            logInstall("[ONEVENT-] emplacement 400100 free");
        break;
		case("400200");
            freeSpace400200 = 1;
            logInstall("[ONEVENT-] emplacement 400200 free");
        break;
		case("400500");
            freeSpace400500 = 1;
            logInstall("[ONEVENT-] emplacement 400500 free");
        break;
		case("400300");
            freeSpace400300 = 1;
            logInstall("[ONEVENT-] emplacement 400300 free");
        break;
		case("400600");
            freeSpace400600 = 1;
            logInstall("[ONEVENT-] emplacement 400600 free");
        break;
		
		
		//Convoyeur de sortie
		/*case("401200"); 
			stock->nbPalletteConv = (stock->nbPalletteConv -1) ;
            logInstall("[ONEVENT-] emplacement 401100 free");
        break;
		*/
		
		//Lignes de stock
		case("stk1");
            stock->nbPaletteSol = (stock->nbPaletteSol- 1);
			logInfo("[ONEVENT-] onEndMissPickSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk11");
           stock->nbPaletteSol = (stock->nbPaletteSol- 1);
			logInfo("[ONEVENT-] onEndMissPickSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk2");
           stock->nbPaletteSol = (stock->nbPaletteSol- 1);
		   logInfo("[ONEVENT-] onEndMissPickSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk22");
            stock->nbPaletteSol = (stock->nbPaletteSol- 1);
			logInfo("[ONEVENT-] onEndMissPickSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk3");
           stock->nbPaletteSol = (stock->nbPaletteSol- 1);
		   logInfo("[ONEVENT-] onEndMissPickSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk33");
            stock->nbPaletteSol = (stock->nbPaletteSol- 1);
			logInfo("[ONEVENT-] onEndMissPickSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		default;
        break;
    } 
}

export function STOCK_updateAfterDrop(pointData)
{
	//On checke les compteurs
	global stock('compteurs');	

    switch(pointData->name)
    {
        //Gestion des freeSpace En Rack
	   case("400400");
            freeSpace400400 =  0 ;
            logInstall("[ONEVENT-] emplacement 400400 plein");
        break;
		case("400100");
            freeSpace400100 = 0;
            logInstall("[ONEVENT-] emplacement 400100 plein");
        break;
		case("400200");
            freeSpace400200 = 0;
            logInstall("[ONEVENT-] emplacement 400200 plein");
        break;
		case("400500");
            freeSpace400500 = 0;
            logInstall("[ONEVENT-] emplacement 400500 plein");
        break;
		case("400300");
            freeSpace400300 = 0;
            logInstall("[ONEVENT-] emplacement 400300 plein");
        break;
		case("400600");
            freeSpace400600 = 0;
            logInstall("[ONEVENT-] emplacement 400600 plein");
			demoShowroom(1,2);
        break;
		
		//Convoyeur Entrée
		case("401100");
		stock->compteurConv = stock->compteurConv + 1;
			 if (stock->compteurConv == 1)	
	          {
               TASKS_movePal("401200", "stk11", 2); 
	          }
               else
		       if (stock->compteurConv == 2)
		       {
			     TASKS_movePal("401200", "stk22", 2); 
		       }	 
	             else
                 if (stock->compteurConv == 3)
		         {
		          TASKS_movePal("401200", "stk33", 2); 	
		         }
				  else
                  if (stock->compteurConv == 4)
		          {
		           TASKS_movePal("401200", "stk1", 2); 
				  } 
		           else
                   if (stock->compteurConv == 5)
		           {
		            TASKS_movePal("401200", "stk2", 2); 	
		           }
				    else
                    if (stock->compteurConv == 6)
		            {
		             TASKS_movePal("401200", "stk3", 2); 	
		            }
				
			     logInstall("[ONEVENT-]  drop emplacement 401100");
				 logInfo("[ONEVENT-] onEndMissDrop - nombre palette possées sur le convoyeur au total= %z ", stock->compteurConv );
        break;
		
		
		//Lignes de stock
		case("stk1");
            stock->nbPaletteSol = (stock->nbPaletteSol + 1);
			logInfo("[ONEVENT-] onEndMissDropSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk11");
            stock->nbPaletteSol = (stock->nbPaletteSol + 1);
			logInfo("[ONEVENT-] onEndMissDropSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk2");
            stock->nbPaletteSol = (stock->nbPaletteSol + 1);
			logInfo("[ONEVENT-] onEndMissDropSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk22");
            stock->nbPaletteSol = (stock->nbPaletteSol + 1);
			logInfo("[ONEVENT-] onEndMissDropSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk33");
            stock->nbPaletteSol = (stock->nbPaletteSol + 1);
			logInfo("[ONEVENT-] onEndMissDropSol - nombre palette sol = %z ", stock->nbPaletteSol);
        break;
		case("stk3");
            stock->nbPaletteSol = (stock->nbPaletteSol + 1);
			logInfo("[ONEVENT-] onEndMissDropSol - nombre palette sol = %z ", stock->nbPaletteSol);
			stock->compteurConv = 0;
			solRack(1,2);
        break;
		default;
        break;
    }
}

/**
    call by the robot
    anwer if the conveyor is free of pallet or not
**/
export function isFree = onIsConveyorFree_Drop(args)
{
     isFree  = 0 ; 
     //depiling argument for the event
     idRob = args.(0);
     idstk = args.(1);
    
     //get all the agv id 
     allRob     = pcGetAllRob();
    
     //checking if the received ifRob is in allRob 
     resultFind = find(allRob == idRob) ;
    
     if( !size(resultFind) )
     {
         logError("[onIsConveyorFree] SUPERVISOR received an event from a unknow robot") ;
     }
    
     //looking in the stk db the information associated with the point//As the proper verication has been made on the mission AGV it is not mandatory to 
     //check input
     // idKey = DBfind_gbl('stkDB','id','==',idstk);    
     // point = DBget_gbl('stkDB',idKey) ;
    

     //get the state for the comBox 
     //isFree  =  GetInForTDevice(point->conveyorBpCan) ; //GetInStateComBox becomes
     if (deviceGetComStatus(COMBOX_CONVEYOR()) == 1 && deviceGetInputDataByNumber(COMBOX_CONVEYOR(),4) == 0  && deviceGetInputDataByNumber(COMBOX_CONVEYOR(),7) == 0)
     {
        isFree  =  1;
         logInfo('State of inputs 3 et 6  : %d et %d', deviceGetInputDataByNumber(COMBOX_CONVEYOR(),4), deviceGetInputDataByNumber(COMBOX_CONVEYOR(),7));
         logInfo('isFree vaut 1, the drop can begin');
     }
     else
     {
        isFree = 0;
        logInfo('State of inputs 3 et 6  : %d et %d', deviceGetInputDataByNumber(COMBOX_CONVEYOR(),4), deviceGetInputDataByNumber(COMBOX_CONVEYOR(),7));
        logInfo('isFree vaut 0, the drop cannot begin');
     }
} 

export function isFree = onIsConveyorFree_Pick(args)
{
     isFree  = 0 ; 
     //depiling argument for the event
     idRob = args.(0);
     idstk = args.(1);
    
     //get all the agv id 
     allRob     = pcGetAllRob();
    
     //checking if the received ifRob is in allRob 
     resultFind = find(allRob == idRob) ;
    
     if( !size(resultFind) )
     {
         logError("[onIsConveyorFree] SUPERVISOR received an event from a unknow robot") ;
     }
    
     //looking in the stk db the information associated with the point//As the proper verication has been made on the mission AGV it is not mandatory to 
     //check input
     // idKey = DBfind_gbl('stkDB','id','==',idstk);    
     // point = DBget_gbl('stkDB',idKey) ;
    

     //get the state for the comBox 
     //isFree  =  GetInForTDevice(point->conveyorBpCan) ; //GetInStateComBox becomes
     if (deviceGetComStatus(COMBOX_CONVEYOR()) == 1 && deviceGetInputDataByNumber(COMBOX_CONVEYOR(),5) == 1  && deviceGetInputDataByNumber(COMBOX_CONVEYOR(),6) == 1 && deviceGetInputDataByNumber(COMBOX_CONVEYOR(),8) == 0)
     {
        isFree  =  1;
        logInfo('State of inputs 4, 5 et 7  : %d,  %d et %d', deviceGetInputDataByNumber(COMBOX_CONVEYOR(),5), deviceGetInputDataByNumber(COMBOX_CONVEYOR(),6), deviceGetInputDataByNumber(COMBOX_CONVEYOR(),8));
        logInfo('isFree vaut 1, the pick can begin');
     }
     else
     {
        isFree = 0;
        logInfo('State of inputs 4, 5 et 7  : %d,  %d et %d', deviceGetInputDataByNumber(COMBOX_CONVEYOR(),5), deviceGetInputDataByNumber(COMBOX_CONVEYOR(),6), deviceGetInputDataByNumber(COMBOX_CONVEYOR(),8));
        logInfo('isFree vaut 0, the pick cannot begin');
     }
}