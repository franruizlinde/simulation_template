
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                    GRAPHIC USER INTERFACE INITIALIZATION                    //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    initialize the simulation windows
*/
export function initSimWindow() {
    global g('simConf');

    //if(strcmp(g->scheduler,'FIFO')) {
        simWin('close');
        simWin('init');
        simWin('show');
        fct = sprintf('$simWin %s','update');
        setRandCbk(fct,'10s','0s');
    //}
}


/*
    initialize the mission editor windows
*/
export function initMissEditor() {
    global g('simConf');
    global z('winConf');

    if(g->missEditorEnable == 0) return;

    try updateMissGrid;
    catch { //init windows varspc in the global
        z->missWin = varspc('id',0);
        z->addWin = varspc('id',0);
        z->editWin = varspc('id',0);
        z->pntWin = varspc('id',0);
        z->pntEditWin = varspc('id',0);
        z->missList = {};
        z->pntList = {};
    }

    //dbMiss = initMissDB(); // init the miss editor DB
    //dbConf = initPntDB(); // init the points DB
}

/*
    initialize the error windows
*/
export function initErrWindow() {

    errWin('close');
    errWin('init');
}


/*
    initialize the mission editor DB
*/
export function db = initMissDB() {
    global w('w');
    global d('db');
    d->allowDBonDisk = 1; //me demandez pas pourquoi mais ce field est à 0 par default grrrr

    if(!isdir(fnameConc((w->env->proj),['SIM\\_DB FILES']))) mkdir(fnameConc((w->env->proj),['SIM\\_DB FILES']));
    cd(fnameConc((w->env->proj),['SIM\\_DB FILES'])); // concatenate path of the working folder
    dbName = fnameName(w->env->name); // dbname is same as circuit to keep track
    w->env->dbMiss = dbName; //store DB name in global

    //check if cir name is short enough to be put as DB name
    if(size(dbName)>21) displayError('[initMissDB] The name of the circuit is too long to be accepted as DB name (21 char max).');

    // DB defiinition
    dbVarspc = varspc(
        'key',0,					//unique id of miss
        'title',strn(''),		//mission name
        'enable',0,				//0- disabled, 1-enabled
        'ratePerHour',0.,      //rate per hour
        'group',strn(''),      //group of rob
        'src',strn(''),			//source of task
        'dst',strn(''),         //destination of task
        'flowsName',strn(''),  //flow name
        'linkedTo',0,            //unique id of previous linked task
    );

    if(!isfile(fnameConc(pwd,[dbName,'.obj']))) {
        logDebug('[initMissDB] File not found ! Creating DB');

        DBcreate_gbl(dbName,dbVarspc,'key'); //create DB
        DBsave_gbl(dbName); // save file
    }
    else {
        logDebug('[initMissDB] DB found !');
        DBcreate_gbl(dbName,dbVarspc,'key'); //load DB
    }

    db = dbName;

    //leave db folder
    cd('C:/agv/');
}


/*
    initialize the point configuration DB
*/
export function db = initPntDB() {
    global w('w');
    global d('db');
    d->allowDBonDisk = 1; // moi non plus ne demandez pas pourquoi ce field est à 0 par defaut >:-(

    cd(fnameConc((w->env->proj),['sim\\_DB FILES'])); // concatenate path of the working folder
    dbName = [fnameName(w->env->name),'_pnt']; // dbName is same as circuit (+pnt) to keep track
    w->env->dbConf = dbName;

    //DB defiinition
    dbVarspc = varspc(
        'key',0,
        'grpName',strn(''),
        'type',strn(''),
        'pallet',strn(''),
        'nbPnt',0,
        'heightTable',{[0.0]},
        'safetyPointMargin',0,
        'slotLng',0,
        'npalsx',0,
        'heightOffSet',0,
        'detectionTime',0,
    );

    if(!isfile(fnameConc(pwd,[dbName,'.obj']))) {
        logDebug('[initPntDB] File not found ! Creating DB');

        DBcreate_gbl(dbName,dbVarspc,'key'); //create DB
        DBsave_gbl(dbName); // save file
    }

    else {
        logDebug('[initPntDB] DB found !');
        DBcreate_gbl(dbName,dbVarspc,'key'); //load DB
    }

    //delete old key (pnt which don't exist anymore)
    clearPntDB();

    //update DB
    addPntDB();

    db = dbName;

    //leave db folder
    cd('C:/agv/');
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                 GRAPHIC USER INTERFACE DEFINITION FUNCTIONS                 //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


/*
  Definition of the simulation windows
  @param action [char] -> action to realize : 'init', 'show', 'update', 'close'
*/
export function simWin(action)
{
    global simWindow('simWindow');
    id = getSimxGlobalId();
    global w(id);

    if (nargin==0) action='show';
    switch(action)
    {
        case('init');
            simWindow->fig = -1;
            simWindow->txttsk = 0;
            simWindow->pos = [100,100];
        case('show');
            if (isobj(simWindow->fig)) raise(simWindow->fig);
            else
            {
                simWindow->fig = win(w->fig,'style','noresize|tool|float','pos',simWindow->pos,'clientSize',[440,330],'closeCbk','$simWin close','title','Simulation Scheduler');
                simWindow->txttsk = staticText(simWindow->fig ,'ALIGN_LEFT','pos',[0,0],'size',[440,260]);
                simWindow->txtrob = staticText(simWindow->fig ,'ALIGN_LEFT','pos',[0,270],'size',[440,60]);
            }
            simWin('update');
        case('update');
            if (isobj(simWindow->fig))
            {
                tasks = strJoin(simSchdGetTasks());
                set(simWindow->txttsk,'string',tasks);
                set(simWindow->txtrob,'string',' ');
            }
        case('close');
            if (isvar(simWindow,'fig') && isobj(simWindow->fig))
            {
                simWindow->pos = get(simWindow->fig,'pos');
                delete(simWindow->fig);
                simWindow->fig = -1;
            }
        default;
            warning([ 'simWin unknown action:',action ]);
    }
}


/*
    Definition of the error windows
    @param action [char] -> action to realize : 'init', 'show', 'update', 'close'
*/
export function errWin(action)
{
    global errWindow('errWindow');
    id = getSimxGlobalId();
    global w(id);

    if (nargin==0) action='show';
    switch(action)
    {
        case('init');
            errWindow->fig = -1;
            errWindow->txttsk = 0;
            errWindow->pos = [100,100];
            errWindow->errMsg = '';
        case('show');
            if (isobj(errWindow->fig)) raise(errWindow->fig);
            else
            {
                winErrSize = [250,100];
                errWindow->fig = win(w->fig,'style','noresize|tool|float','pos',get(w->fig,'pos')-winErrSize/2+get(w->fig,'size')/2,'clientSize',winErrSize,'closeCbk','$errWin close','title','Simulation Error');
                errWindow->txttsk = staticText(errWindow->fig ,'ALIGN_LEFT','pos',[20,10],'size',[210,50]);
                errWindow->txtrob = staticText(errWindow->fig ,'ALIGN_LEFT','pos',[20,50],'size',[210,50]);
            }
            errWin('update');
        case('update');
            if (isobj(errWindow->fig))
            {
                set(errWindow->txttsk,'string',errWindow->errMsg);
                set(errWindow->txtrob,'string',' ');
            }
        case('close');
            if (isvar(errWindow,'fig') && isobj(errWindow->fig))
            {
                errWindow->pos = get(errWindow->fig,'pos');
                delete(errWindow->fig);
                errWindow->fig = -1;
            }
        default;
            warning([ 'errWin unknown action:',action ]);
    }
}


/*
    create and fill the mission windows
    @param idWin [long] -> id of the parent windows
*/
export function createMissWin(idWin) {
    global z('winConf');

    //creation of toolbar
    idTool = toolbar(idWin,'FLAT|HORIZONTAL|TEXT|HORZ_LAYOUT');
    z->missWin->idTool = idTool;

    //creation of the grid
    idGrid = gridBasicCreate(idWin,z->missList,{"Title","Source","Type","Destination","Type","Rate","Custom"},"ROWS");
    z->missWin->idGrid = idGrid;
    //idScroll = scrollbar(idWin,'style','VERTICAL');

    //creation of the cbk for the buttons
    addMissCbk = 'addMiss';
    editMissCbk = sprintf("$editMiss %i",idGrid);
    delMissCbk = sprintf("$delMiss %i",idGrid);
    importDBCbk = 'importDB';
    pntConfCbk = 'pntConf';
    //creation of the buttons
    buttAddMiss = toolbarButton(idTool,'NORMAL','label','Add Miss','cbk',addMissCbk);
    buttEditMiss = toolbarButton(idTool,'NORMAL','label','Edit Miss','cbk',editMissCbk);
    buttDelMiss = toolbarButton(idTool,'NORMAL','label','Delete Miss','cbk',delMissCbk);
    buttImportDB = toolbarButton(idTool,'NORMAL','label','Import DB','cbk',importDBCbk);
    buttPntConf = toolbarButton(idTool,'NORMAL','label','Points Config','cbk',pntConfCbk);

    toolbarRealize(idTool); //update toolbar
}


/*
    Create and fill the point conf windows
    @param v [varspc] -> varpsc data of the parent windows
*/
export function createConfWin(v) {
    global z('winConf');
    global w('w');
    idWin = v->id;
    dbName = w->env->dbConf;


    //creation of toolbar
    idTool = toolbar(idWin,'FLAT|HORIZONTAL|TEXT|HORZ_LAYOUT');
    z->pntWin->idTool = idTool;

    //creation of the grid
    idGrid = gridBasicCreate(idWin,z->pntList,{"Name","Type","Pallet","Number","Height Table","Strategy"},"ROWS");
    z->pntWin->idGrid = idGrid;
    //idScroll = scrollbar(idWin,'style','VERTICAL');

    //creation of the cbk for the buttons
    editPntCbk = sprintf("$editPnt %i",idGrid);
    //creation of the buttons
    buttEditMiss = toolbarButton(idTool,'NORMAL','label','Edit Point','cbk',editPntCbk);

    toolbarRealize(idTool); //update toolbar
}


/*
  Create and fill the add miss windows
  @param v [varspc] -> varspc data of the parent windows
  @param fct [char] -> action needed : 'add' or 'edit'
  @param data* [varspc] -> varspc data of the flow to edit (not needed if 'add' action)
*/
export function createFlowWin(v,fct,data) {
    global z('winConf');
    idWin = v->id;

    //creation of toolbar
    idTool = toolbar(idWin,'FLAT|HORIZONTAL|TEXT|HORZ_LAYOUT');
    v->idTool = idTool;

    //get points from circuit
    [allPnt, allGrp] = cirGetAllPnt();

    //creation of text boxes
    lblTitle = staticText(idWin,'ALIGN_LEFT','title','MISSION TITLE','size',[100,30],'pos',[50,20]);
    v->idTitle = editText(idWin,'style','BESTWRAP','fontPointSize',11,'size',[150,30],'pos',[150,10]);
    lblSrc = staticText(idWin,'ALIGN_LEFT','title','SOURCE :','size',[100,20],'pos',[50,55]);
    v->idSource = choice(idWin,'string',allGrp,'size',[150,20],'pos',[150,50]);
    v->idTypeSrc = staticText(idWin,'ALIGN_LEFT','title','SOURCE TYPE (??)','size',[100,50],'pos',[330,55]);
    lblDst = staticText(idWin,'ALIGN_LEFT','title','DESTINATION :','size',[100,20],'pos',[50,85]);
    v->idDest = choice(idWin,'string',allGrp,'size',[150,20],'pos',[150,80]);
    v->idTypeDst = staticText(idWin,'ALIGN_LEFT','title','DESTI TYPE (??)','size',[100,50],'pos',[330,85]);
    lblRate = staticText(idWin,'ALIGN_LEFT','title','WORK RATE :','size',[100,30],'pos',[50,120]);
    v->idRate = editText(idWin,'style','RIGHT','fontPointSize',11,'size',[150,30],'pos',[150,110]);

    //custom option A RENOMMER
    v->idCustom1 = checkBox(idWin,'title','CUSTOM OPTION 1','size',[100,20],'pos',[50,160]);
    v->idCustom2 = checkBox(idWin,'title','CUSTOM OPTION 2','size',[100,20],'pos',[200,160]);
    v->idCustom3 = checkBox(idWin,'title','CUSTOM OPTION 3','size',[100,20],'pos',[350,160]);

    if(!strcmp(fct,'add')) {
        logInfo('[MISSION EDITOR] : Add window launched.');
        z->addWin = v; //store vrspc in global
        saveMissCbk = sprintf('$saveMiss %s',fct);
    }
    else if(!strcmp(fct,'edit')) {
        logInfo('[MISSION EDITOR] : Edit window launched.');
        set(v->idTitle,'string',char(data->title)); //TODO error if already same name
        set(v->idRate,'string',num2str(data->ratePerHour));
        set(v->idSource,'value',findElt(get(v->idSource,'string'),char(data->src)));
        set(v->idDest,'value',findElt(get(v->idDest,'string'),char(data->dst)));

        z->editWin = v; //store vrspc in global
        saveMissCbk = sprintf("$saveMiss %s %s",fct,num2str(data->key));

    }
    else {
        logError('va voir ailleurs si jy suis');
        return;
    }
    //creation of the buttons
    buttSaveMiss = toolbarButton(idTool,'NORMAL','label','Save Miss','cbk',saveMissCbk);
    toolbarRealize(idTool); //update toolbar
}



/*
  Create and fill the point edit windows
  @param v [varspc] -> varspc data of the parent windows
  @param data [varspc] -> varspc data of the point to edit
*/
export function editPntWin(v,data) {
    global z('winConf');
    idWin = v->id;

    //creation of toolbar
    idTool = toolbar(idWin,'FLAT|HORIZONTAL|TEXT|HORZ_LAYOUT');
    v->idTool = idTool;

    //creation of text boxes
    lblName = staticText(idWin,'ALIGN_LEFT','title','POINT NAME :','size',[100,30],'pos',[50,20]);
    v->idName = editText(idWin,'style','BESTWRAP','fontPointSize',11,'size',[150,30],'pos',[150,10]);
    set(v->idName,'string',char(data->grpName));
    editTextSetEditable(v->idName,0);

    lblType = staticText(idWin,'ALIGN_LEFT','title','POINT TYPE :','size',[100,20],'pos',[50,55]);
    v->idType = choice(idWin,'string',{'Slot','Line','Rack','Conveyor','Elevated','Move'},'size',[150,20],'pos',[150,50]);
    set(v->idType,'value',findElt({'slot','line','rack','conveyor','elevated','move'},char(data->type)));

    lblStrat = staticText(idWin,'ALIGN_LEFT','title','STRATEGY :','size',[100,20],'pos',[50,85]);
    v->idStrat = choice(idWin,'string',{'Furthest','Closest','Equality','Random'},'size',[150,20],'pos',[150,80]);
    set(v->idStrat,'value',0);

    lblHeight = staticText(idWin,'ALIGN_LEFT','title','HEIGHT TABLE :','size',[100,30],'pos',[50,120]);
    v->idHeight = editText(idWin,'style','RIGHT','fontPointSize',11,'size',[150,30],'pos',[150,110]);
    set(v->idHeight,'string',vnum2str(data->heightTable.{0}));

    v->idDetect = checkBox(idWin,'title','DETECTION','size',[100,20],'pos',[50,160]);
    //TODO

    //creation of the button save
    savePntCbk = sprintf('$savePnt %s',num2str(data->key));
    buttSaveMiss = toolbarButton(idTool,'NORMAL','label','Save Point','cbk',savePntCbk);
    toolbarRealize(idTool); //update toolbar

}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                    GRAPHIC USER INTERFACE CALL FUNCTIONS                    //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


/*
    Launch the mission editor windows
*/
export function openMissEditor() {
    global g('simConf');

    if(g->missEditorEnable == 0) {
        logInfo('[MISSION EDITOR] : Not enabled.');
        return;
    }

    missWinCall();
    logInfo('[MISSION EDITOR] : Launched.');
}

/*
  Open the mission editor windows
*/
export function missWinCall() {
    global z('winConf');
    try {
        maximize(z->missWin->id,0);
        raise(z->missWin->id);
    }
    catch {
        z->missWin->id = win(0,'style','noresize','pos',[50,50],'size',[500,500],'title','MISSION EDITOR'/*,'closeCbk','editorClose'*/);
        createMissWin(z->missWin->id); // create the windows interface
        logDebug('win id = %z',z->missWin->id);
    }
    updateMissGrid();
}


/*
  Open the point editor windows
*/
export function pntWinCall() {
    global z('winConf');
    try {
        maximize(z->pntWin->id,0);
        raise(z->pntWin->id);
    }
    catch {
        z->pntWin->id = win(z->missWin->id,'style','floatOnTop','pos',[50,50],'size',[500,300],'title','CONFIGURE POINTS');
        createConfWin(z->pntWin);
    }

    updateConfGrid();
}


/*
    Open the mission add windows
*/
export function missAddCall() {
    global z('winConf');
    try {
        maximize(z->addWin->id,0);
        raise(z->addWin->id);
    }
    catch {
        z->addWin->id = win(z->missWin->id,'style','floatOnTop','pos',[50,50],'size',[500,300],'title','ADD MISSION');
        createFlowWin(z->addWin,"add");
    }
}


/*
    Open the mission edit windows
    @param data [varpsc] -> config data of the selected mission
*/
export function missEditCall(data) {
    global z('winConf');
    try {
        close(z->editWin->id);
    }
    catch {
        //
    }
    z->editWin->id = win(z->missWin->id,'style','floatOnTop','pos',[50,50],'size',[500,300],'title','EDIT MISSION');
    createFlowWin(z->editWin,"edit",data);
}


/*
    Open the point edit windows
    @param data [varpsc] -> config data of the selected point
*/
export function pntEditCall(data) {
    global z('winConf');
    try {
        close(z->pntEditWin->id);
    }
    catch {
        //
    }
    z->pntEditWin->id = win(z->pntWin->id,'style','floatOnTop','pos',[50,50],'size',[500,300],'title','EDIT POINT');
    editPntWin(z->pntEditWin,data);
}

/*
    Delete a mission from the mission editor DB
*/
export function delMissFromConf(title) {
  // TODO
}

/*
    Add a mission from the mission editor DB (deprecated)
*/
/*
export function addMissFromDB() {
    global w('w');
    global g('simConf');

    //get all the existings keys of the DB
    dbName = w->env->dbMiss; // dbname is same as circuit
    allKeys = DBgetAllKeys_gbl(dbName);
    if(size(allKeys)) {
        k = for(allKeys) {
            data = DBget_gbl(dbName,allKeys.(k));
            m = varspc( //mission definition
                'title',data->title,
                'enable',1,
                'ratePerHour',data->ratePerHour,
                'group',data->group,
                'missAuto',1,
                'objectives', {
                    varspc('task', 'pick', 'dst',data->src, 'nbPal', 1),
                    varspc('task', 'drop', 'dst',data->dst, 'nbPal', 1),
                },
                'flows', {
                    varspc('flowName',data->title,'numberOfStep',2,'flowDisplay',1),
                },
            );
            g->missions.{end+1} = m;
        }
    }
    else logWarn('[%s DB] : No keys found.',dbName);
}
*/

/*
    Delete a mission from the mission editor DB (deprecated)
*/
/*
export function delMissFromDB(title) {
    global w('w');

    //find 'title' in the DB
    dbName = w->env->dbMiss; // dbname is same as circuit
    idMiss = DBfind_gbl(dbName,'title','==',title);
    if(!isempty(idMiss)) {
        DBdel_gbl(dbName,idMiss);
        logInfo('[%s DB] : Mission " %s "" deleted from DB.',dbName,title);
    }
    else logWarn('[%s DB] : Mission " %s " not found in DB !',dbName,title);
}
*/



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                   GRAPHIC USER INTERFACE UPDATES FUNCTIONS                  //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
  Update the text of a textbox (useless ?)
  @param idTxtBx [long] -> gui id of the textbox
  @param strVal [char] -> text to set
*/
export function updateReadBx(idTxtBx,strVal) {
    editTextChangeValue(idTxtBx,strVal);
}


/*
  Update the list of a choice list
  @param idChoice [long] -> gui id of the choice list
  @param list [L.char/L;long] -> list of values to set
*/
export function updateChoice(idChoice,list) {
    set(idChoice,'string',list);
}


/*
  Update the grid of the mission editor windows
*/
export function updateMissGrid() {
    global z('winConf');
    idGrid = z->missWin->idGrid;

    gridClearGrid(idGrid); //clear the grid before filling
    gridDeleteRows(idGrid,0,gridGetNumberRows(idGrid));

    updateMissGridConf(); //update mission list from custom missions located in the [].c file
    //updateMissGridDB(); //update mission list from missions in the [].obj database (deprecated)

}


/*
    Update the grid of the point configuration windws
*/
export function updateConfGrid() {
    global z('winConf');
    idGrid = z->pntWin->idGrid;

    gridClearGrid(idGrid); //clear the grid before filling
    gridDeleteRows(idGrid,0,gridGetNumberRows(idGrid));

    updatePntGrid(); //update pnt list

}


/*
    Update the grid of the mission editor with custom values from .c file
*/
export function updateMissGridConf() {
    global z('winConf');
    global g('simConf');
    idRow = 0;

    m = for(g->missions) { //get missions config from .c file
        if(g->missions.{m}->enable==0) continue;//skip for disabled missions
        else {
            valList = getValCustom(g->missions.{m}); //get miss value from conf
            updateGridRow(z->missWin->idGrid,idRow,valList); //update grid row
            idRow = idRow + 1;
        }
    }
}


/*
    Update the grid of the pnt configuration windows with custom values from .c file
*/
export function updatePntGrid() {
    global z('winConf');
    global g('simConf');
    global w('w');

    dbName = w->env->dbConf;

    // registration of all points and groups
    [allPnt,allGrp,nbPerGrp] = cirGetAllPnt();

    p = for(allGrp) { //get pnt config from DB

        name = allGrp.{p};

        //find if exist in the DB
        id = DBfind_gbl(dbName,'grpName','==',name);

        if(!isempty(id)) {
            //get data from DB
            data = DBget_gbl(dbName,id);
            type = data->type;
            pallet = data->pallet;
            nbPnt = data->nbPnt;
            heightTable = vnum2str(data->heightTable.{0});
            strategy = 0;

            valList = {name,type,pallet,nbPnt,heightTable,strategy};
            updateGridRow(z->pntWin->idGrid,p,valList); //update grid row
        }
    }
}


/*
    Update the grid of the mission editor with DB values (deprecated)
    @param idWin [long] -> gui id of the parent windows
*/
/*
export function updateMissGridDB(idWin) {
    global z('winConf');
    global w('w');

    idGrid = z->missWin->idGrid;
    rowN = gridGetNumberRows(idGrid); //get the number of rows

    //get all the existings keys of the DB
    dbName = w->env->dbMiss; // dbname is same as circuit
    allKeys = DBgetAllKeys_gbl(dbName);
    if(size(allKeys)) {
        k = for(allKeys) {
            valList = {};
            data = DBget_gbl(dbName,allKeys.(k));
            valList.{end+1} = data->title;
            valList.{end+1} = data->src;
            srcData = missPntGetInfo(data->src); //TODO: look from DB !
            valList.{end+1} = srcData->type;
            valList.{end+1} = data->dst;
            dstData = missPntGetInfo(data->dst);
            valList.{end+1} = dstData->type;
            valList.{end+1} = num2str(data->ratePerHour);
            valList.{end+1} = data->group;
            // TODO check the size of valList
            row = rowN + k;
            updateGridRow(z->missWin->idGrid,row,valList);
        }
    }
    else logWarn('[%s DB] : No keys found.',dbName);
}
*/

/*
    Update the grid row of a grid
    @param idGrid [long] -> gui id of the grid to update
    @param row [long] -> number of the row to update
    @param valList [L.long / L.char] -> list of values to set
*/
export function updateGridRow(idGrid,row,valList) {
    gridUpdateRow(idGrid,row,valList); //set cell value
    gridAutoSize(idGrid);
    //logInstall("[GRID] : Fill the mission %s on row %i",valList.{0},row);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//               GRAPHIC USER INTERFACE CALLBACK BUTTON FUNCTIONS              //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    Called when click on "point conf" button
*/
export function pntConf() {
    pntWinCall();
}


/*
    Called when click on "add miss" button
*/
export function addMiss() {
    missAddCall();
}


/*
    Called when click on "edit miss" button
    @param idG [char] -> gui id of the grid
*/
export function editMiss(idG) {
    global w('w');
    idGrid = str2num(idG);

    selectTitle = getTitleFromGrid(idGrid); //get title from grid mission
    if(isempty(selectTitle)) return;

    //find 'title' in the mission text
    missvar = varspc();


    if(!isempty(missvar)) {
        missEditCall(missvar);
        logInfo('[EDIT MISSION] : Mission "%s" imported from .c file.',selectTitle);
    }
    else {
        logWarn('[EDIT MISSION] : Mission "%s" not found in .c file) !',selectTitle);
        logError('[EDIT MISSION] : Error in the mission. Cannot edit.');
    }
}


/*
    Called when click on "edit pnt" button
    @param idG [char] -> gui id of the grid
*/
export function editPnt(idG) {
    global w('w');
    idGrid = str2num(idG);

    selectPnt = getTitleFromGrid(idGrid); //get pnt title from grid

    //find 'title' in the pnt DB
    dbName = w->env->dbConf; // dbname is same as circuit
    k = DBfind_gbl(dbName,'grpName','==',selectPnt);
    if(size(k)) {
        data = DBget_gbl(dbName,k);
        pntEditCall(data);
        //logInfo('[%s DB] : Groupe point " %s " imported from DB.',dbName,selectPnt);
    }
    else {
        logWarn('[%s DB] : Group point " %s " not found in DB !',dbName,selectPnt);
        logError('[EDIT POINTS] : Error in the point configuration. Cannot edit.');
    }
}


/*
    Called when click on "del miss" button
    @param idG [char] -> gui id of the grid
*/
export function delMiss(idG) {
    idGrid = str2num(idG);
    selectTitle = getTitleFromGrid(idGrid); //get title from grid mission
    delMissFromConf(selectTitle); //delete selected mission from DB
    wait(51);
    updateMissGrid(); //update mission editor grid
}


/*
    Called when click on "save miss" button
    @param fct [char] -> action to realize : "add" or "edit"
*/
export function saveMiss(fct) { //save a mission on the database
    global z('winConf');
    missvar = varspc(); //mission varspc

    // get file path
    filepath = kiwiEditorCircuitFullPath();

    //choose windows elements
    if(!strcmp(fct,'add')) window = z->addWin;
    else if (!strcmp(fct,'edit')) window = z->editWin;
    else {
        logError('[saveMiss] parameter can only be "add" or "edit".');
        return;
    }

    //TODO check that all fields are corrects
    missvar->title = get(window->idTitle,'string'); //TODO error if already same name
    //missvar->title = strReplace(missvar,' ','_');
    missvar->ratePerHour = str2num(get(window->idRate,'string'));
    srcSlct = get(window->idSource,'value'); //TODO error if -1 !
    missvar->src = get(window->idSource,'string').{srcSlct};
    dstSlct = get(window->idDest,'value'); //TODO error if -1 !
    missvar->dst = get(window->idDest,'string').{dstSlct};
    missvar->group = []; //TODO add textbox

    // write in file
    if(!strcmp(fct,'add')) { // add mission
      addmiss2file(missvar,filepath);
    }
    else {// edit mission
      logError('[saveMiss] edit not developed yet.');
      //editmiss2file(missvar,filepath);
    }

    //update mission editor grid
    updateMissGrid();
    // close window
    close(window->id);
}


/*
    Called when click on "save point" button
    @param k [char] -> DB key of the entry to save
*/
export function savePnt(k) {
    global z('winConf');
    global w('w');
    key = str2num(k);

    cd(fnameConc((w->env->proj),['db'])); // concatenate path of the working folder

    //get original data from db
    dbName = w->env->dbConf;
    data = DBget_gbl(dbName,key);

    //retrieve all fields from window
    window = z->pntEditWin;
    typeSlct = get(window->idType,'value');
    data->type = get(window->idType,'string').{typeSlct};
    data->heightTable = {str2cellNum(get(window->idHeight,'string'))};

    //update DB
    DBset_gbl(dbName,data);
    DBsave_gbl(dbName);

    //update pnt grid
    updatePntGrid();

    //leave db folder
    cd('C:/agv/');

    close(window->id);

}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//                                                                             //
//                   GRAPHIC USER INTERFACE USEFUL FUNCTIONS                   //
//                                                                             //
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*
    display an error message in a pop up windows
    @param errMsg [char] -> error message to display
*/
export function displayError(errMsg)
{
    global errWindow('errWindow');

    unint errWindow->errMsg = errMsg;

    errWin('show');
    error(errMsg);
}


/*
    Create a list from varspc (just what we need); //TODO: to rework (disgusting)
*/
export function valList = getValCustom(missInfo) {
    m = missInfo;

    taskNumber = size(m->objectives);
    // get title
    title = m->title;

    // get source point
    if(isvar(m->objectives.{0},'dst')) src = m->objectives.{0}->dst;
    else if(isvar(m->objectives.{0},'grp')) src = m->objectives.{0}->grp;
    if(istype(src,'L.char')) src = src.{0}; //take first of group for now //TODO change this
    src = m->objectives.{0}->dst;
    srcData = missPntGetInfo(src);
    srcType = char(srcData->type);

    if(isvar(m->objectives.{taskNumber-1},'dst')) dst = m->objectives.{taskNumber-1}->dst;
    else if(isvar(m->objectives.{taskNumber-1},'grp')) dst = m->objectives.{taskNumber-1}->grp;
    if(istype(dst,'L.char')) dst = dst.{0}; //take first of group for now //TODO change this
    dstData = missPntGetInfo(dst);
    dstType = char(dstData->type);

    rate = num2str(m->ratePerHour*taskNumber/2);

    if(isempty(m->group)) grp = 'all';
    else grp = tab2str(m->group);

    valList = {title,src,srcType,dst,dstType,rate,grp};
}


/*
  Get the title of a selected row
  @param idGrid [long] -> gui id of the grid
  @return str [char] -> title of the selected row
*/
export function str = getTitleFromGrid(idGrid) {
    str = new('char',0);
    titleCol = 0; //id of the "title" column

    selectRow = gridGetSelectedRows(idGrid);
    //trigger error if several row selected
    if(size(selectRow)!=1) {
      logError('[getTitleFromGrid] You have to select 1 row ! (%z selected)',size(selectRow));
      return;
    }

    str = gridGetCellValue(idGrid,selectRow,titleCol); //get the title of the selected row
}

/*
 * Get the mission varspc from the .c file (from name)
 * @param title [string] -> title of the mission
 * @return startIdx [long] -> id of the start of the bloc mission
 * @return endIdx [long] -> id of the end of the bloc mission
 */
export function [startIdx,endIdx] = getMissIdxFromTitle(title) {

  //get file text
  [_,filepath] = kiwiEditorCircuitFullPath();
  filetext = readText(filepath);

  // text before file
  startOfMissText = ["\n    conf->missions.{end+1} = varspc(\n",
  "        'title','",title];
  endOfMissText = [title,"','numberOfStep',2,'flowDisplay',1),\n",
  "         }\n",
  "    );\n\n"];

  // find title in .c file
  startIdx = strfind(filetext,startOfMissText);
  if(isempty(startIdx)) {
    logError('[getMissionVarspc] Mission title "%s" not found in .c file!',title);
    return;
  }
  endIdx = strfind(filetext,endOfMissText);
  endIdx = endIdx + size(endOfMissText);

}


/*
    Function to add all the point from the circuit to the point configuration DB
*/
export function addPntDB() {
    global w('w');

    dbName = w->env->dbConf;

    //get all pnt and grp pnt
    [allPnt, allGrp, nbPerGrp] = cirGetAllPnt();

    p = for(allGrp) {
        //loop over all grp of pnt
        grpName = allGrp.{p};
        idGrp = DBfind_gbl(dbName,'grpName','==',grpName);

        //if not found in the DB, add in DB
        if(isempty(idGrp) && !strMatch(grpName,'TOPO(KERN)')) {
            data = missPntGetInfo(grpName);
            data->heightTable = {data->heightTable};
            data->nbPnt = nbPerGrp.(p);
            data->grpName = grpName;
            varspcRemove(data,'pntName');
            DBadd_gbl(dbName,data);
            logDebug('[addPntDB] Group " %s " added to pntDB',grpName);
        }
    }
}


/*
    Remove point from the DB that no longer exist in the circuit
*/
export function clearPntDB() {
    global w('w');

    dbName = w->env->dbConf;
    allKeys = DBgetAllKeys_gbl(dbName);

    //get all pnt and grp pnt
    [allPnt, allGrp] = cirGetAllPnt();

    //loop for all the DB entry
    k = for(allKeys) {
        grpName = DBget_gbl(dbName,allKeys.(k),'grpName');
        if(isempty(findElt(allGrp,char(grpName)))) {
            DBdel_gbl(dbName,k);
            logDebug('[clearPntDB] Points " %s " deleted from pntDB (do not exist)',grpName);
        }
    }
}


/*
 *
 * @param missvar [varspc] -> mission varspace
 * @return missstr [string] -> mission bloc string
 */
export function missstr = missvar2str(missvar) {
  missstr = new('char',0); //create str

  // retrieve data
  misstitle = missvar->title;
  missflowname = ['FLOW_',misstitle];
  missenable = 1; //TODO
  missrate = missvar->ratePerHour;
  missgroup = missvar->group;
  missauto = 1; //TODO
  misssrc = missvar->src;
  missdst = missvar->dst;

  //create bloc string
  missstr = ["\n    conf->missions.{end+1} = varspc(\n",
  "        'title','",misstitle,"',\n",
  "        'enable',",num2str(missenable),",\n",
  "        'ratePerHour',",num2str(missrate),",\n",
  "        'group',",tab2str(missgroup),",\n",
  "        'missAuto',",num2str(missauto),",\n",
  "        'objectives', {\n",
  "            varspc('task', 'pick', 'dst', '",misssrc,"'),\n", //TODO add other parameters
  "            varspc('task', 'drop', 'dst', '",missdst,"'),\n", //TODO add other parameters
  "         },\n",
  "         'flows', {\n",
  "            varspc('flowName', '",missflowname,"', 'numberOfStep', 2, 'flowDisplay', 1),\n",
  "         }\n",
  "    );\n\n"];
}

/*
 *
 *
 */
export function addmiss2file(missstr,filepath) {
  // get text
  currentFileText = readText(filepath);

  // find start and end of file
  endIdx = findLastElt(currentFileText,'}');
  if(isempty(endIdx)) displayError(sprintf('[ADD MISS] Error in %s file (no end of file found).',filepath));
  endOfFileText = currentFileText.(lin(endIdx,end));
  startOfFileText = currentFileText.(lin(0,endIdx -1));

  // create new text
  newFileText = [startOfFileText,missstr,endOfFileText];

  // overwrite text file
  saveText(filepath,newFileText);
}
