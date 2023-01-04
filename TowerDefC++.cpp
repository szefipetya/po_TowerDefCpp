
#include <SDL.h>
#include <iostream>
//#include <SDL2_gfxPrimitives.h>

#include <math.h>
#include <stdlib.h>
#include <SDL_events.h>
#include <SDL_events.h>
#include "App.h"
#include "App.h"
#include "Window.h"
#include <SDL_ttf.h>
#include "Entity.h"
#include "InfoPanelContainer.h"

void listProperties(Controller* c) {
	std::cout << c->fontSize << "\n";
	std::cout << c->isClick() << "\n";
	std::cout << c->text << "\n";
}
void onKeyF2() {
	std::cout <<"F2"<< "\n";
	return;
}
void spawnStuff(Model* model) {
	/*for (int k = 0; k < 1; k++) {
		for (int j = 0; j < model->matrixSizeX; j++) {
			model->addEntity(Entity::Type::Orc, j, model->matrixSizeY-3);
		}
	}*/
	//same line as eco
	//model->addEntity(Entity::Type::Orc, (15 * 2 - 7) / 2, 20  - 1);
	//model->canBuild((15 * 2 - 7) / 2, 20 - 1, 3);

	//clear way
	//model->addEntity(Entity::Type::Orc, (15 * 2) / 2, 20 * 2 - 1);
	//model->addEntity(Entity::Type::Orc, (15 * 2) / 2 + 1, 20 * 2 - 1);
	//model->canBuild(0, 0, 3);
	//model->build(Entity::Type::Economic, 10, 10);
	//model->build(Entity::Type::Economic,2, 2);
	/*model->build(Entity::Type::Economic, 13,10);
	model->build(Entity::Type::Laser, 10, 10);

	
	model->addEntity(Entity::Type::Skeleton, 10, 25);
	model->addEntity(Entity::Type::Skeleton, 10, 25);
	model->addEntity(Entity::Type::Skeleton, 10, 25);
	model->addEntity(Entity::Type::Skeleton, 10, 25);*/

	//model->build(Entity::Type::Artillery, 15, 15);

	//model->addEntity(Entity::Type::Skeleton, 15,20);
	//model->addEntity(Entity::Type::Skeleton, 9, 20);
	//model->addEntity(Entity::Type::Ogre, 15, 21);

	//spawning eco building(s)
	//model->addEntity(Entity::Type::Economic, 10,10);

	//spawning turret(s)
	//	model->addEntity(Entity::Type::Turret, 5, 2);
	//model->addEntity(Entity::Type::Turret, 20, 20);

	//test canBuild
	if (model->canBuild(11, 11, 3))
	{
		std::cout << "[Model] can build" << std::endl;
	}
	else
	{
		std::cout << "[Model] cannot build" << std::endl;
	}
}
void onlyModel() {
	int x = 10 * 2;///szélesség <--->
	int y = 20 * 2;///magasság^v

	Model* model = new Model(x, y);
	spawnStuff(model);
	while (1) {

	}
}
void InitUIAndModel(App &app) {
	srand(time(NULL));
	//-----------------------x , y
	if (0 > TTF_Init()) {
		std::cout << "TTF error!...\n";
	}
	int x = 15*2;///szélesség <--->
	int y = 20*2;///magasság^v
	Model* model = new Model(x,y);
	Global* global = new Global();
	app.setGlobal(global);
	app.setModel(model);
	//VIEW INIT///
	app.initResources();
	app.initPortraitGraphics();
	app.setBackgroundImg("stars");
	/////
	TableContainer* table = new TableContainer(app.WINDOW_WIDTH / 5, 30, (app.WINDOW_WIDTH * 4 / 5), app.WINDOW_HEIGHT - 30);
	app.addContainer(table);
	//table->setBackgroundImg("grass");
	app.setTable(table);
	app.initGameGraphics();

	//MAX 1000 OBJECTS
	
	//SDL_SetWindowFullscreen(app.window, SDL_WINDOW_FULLSCREEN);
	
	
	Container* topmenu = new Container(0, 0, app.WINDOW_WIDTH / 3, 30);
	app.addContainer(topmenu);
	Container* topInfo = new Container(app.WINDOW_WIDTH / 3, 0, app.WINDOW_WIDTH*2 / 3, 30);
	app.addContainer(topInfo);
	topInfo->setMargins(0);
	Label* timeLabel=(Label*)topInfo->add("label","00:00",2,10);
	timeLabel->setIconImg("stopwatch");
	Label* waweLabel = (Label*)topInfo->add("label","Wave: 1",2,10);
	Label* lifeLabel = (Label*)topInfo->add("label","20",2,10);
	lifeLabel->setIconImg("hearth");
	Label* goldLabel = (Label*)topInfo->add("label","0",2,10);
	goldLabel->setIconImg("coins");
	goldLabel->setOrientaion(horizontal);
	using std::placeholders::_1;
	app.getModel()->setUpdateMoneyEventListener(std::bind(&Controller::updateText, goldLabel, _1));
	app.getModel()->setUpdateHealthEventListener(std::bind(&Controller::updateText, lifeLabel, _1));
	app.getModel()->setUpdateWaveEventListener(std::bind(&Controller::updateText, waweLabel, _1));
	app.getModel()->setUpdateTImeEventListener(std::bind(&Controller::updateText, timeLabel, _1));
	topInfo->setBorder(false);
	topmenu->setMargins(0);
	topmenu->setBorder(false);

	topmenu->setFontSize(11);
	Button* file = (Button*)topmenu->add("button", "File", 0);
	Button* options = (Button*)topmenu->add("b", "Options", 0);
	
	options->addEventlistener("click", std::bind(&App::onToggleOptions, &app));//HERE
	Button* save = topmenu->addButton("Save", 0);
	//INFO PANEL////////////////////////////
	InfoPanelContainer* infoContainer = new InfoPanelContainer(0, 30, app.WINDOW_WIDTH/5, (app.WINDOW_HEIGHT - 30) / 3);
	app.addContainer(infoContainer);
	app.setInfoPanelContainer(infoContainer);
	
	//INFO PANEL END/////////////////////
	Container* sideMenu = new Container(0, (app.WINDOW_HEIGHT - 30) / 3+30, 55, (app.WINDOW_HEIGHT - 30)*2/3);
	app.addContainer(sideMenu);
	
	Container* sideShop= new Container(55, (app.WINDOW_HEIGHT - 30) / 3 + 30, app.WINDOW_WIDTH / 5-55, (app.WINDOW_HEIGHT - 30) * 2 / 3);
	app.addContainer(sideShop);
	sideShop->setBorder(false);
	sideShop->setMargins(5);
	sideShop->setOrientaion(vertical);

	Button* shop_turret;
	if (model->diff == 0)
	{
		shop_turret = (Button*)sideShop->add("b", std::to_string(model->metaMapEasy[Entity::Turret - 5][0].cost) + " Turret", 2, 9);
		shop_turret->setIconImg("coins");
		shop_turret->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Turret)));
	}
	else if (model->diff == 1)
	{
		shop_turret = (Button*)sideShop->add("b", std::to_string(model->metaMapMedium[Entity::Turret - 5][0].cost) + " Turret", 2, 9);
		shop_turret->setIconImg("coins");
		shop_turret->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Turret)));
	}
	else if (model->diff == 2)
	{
		shop_turret = (Button*)sideShop->add("b", std::to_string(model->metaMapHard[Entity::Turret - 5][0].cost) + " Turret", 2, 9);
		shop_turret->setIconImg("coins");
		shop_turret->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Turret)));
	}
	//Button* shop_turret=(Button*)sideShop->add("b", std::to_string(model->metaMap[Entity::Turret-5][0].cost)+" Turret" ,2,9);
	//shop_turret->setIconImg("coins");
	//shop_turret->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Turret)));
	//Buy Economic in shop
	Button* shop_economic;
	if (model->diff == 0)
	{
		shop_economic = (Button*)sideShop->add("b", std::to_string(model->metaMapEasy[Entity::Economic - 5][0].cost) + " Economy", 2, 9);
		shop_economic->setIconImg("coins");
		shop_economic->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Economic)));
	}
	else if (model->diff == 1)
	{
		shop_economic = (Button*)sideShop->add("b", std::to_string(model->metaMapMedium[Entity::Economic - 5][0].cost) + " Economy", 2, 9);
		shop_economic->setIconImg("coins");
		shop_economic->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Economic)));
	}
	else if (model->diff == 2)
	{
		shop_economic = (Button*)sideShop->add("b", std::to_string(model->metaMapHard[Entity::Economic - 5][0].cost) + " Economy", 2, 9);
		shop_economic->setIconImg("coins");
		shop_economic->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Economic)));
	}
	//Button* shop_economic=(Button*)sideShop->add("b", std::to_string(model->metaMap[Entity::Economic-5][0].cost) + " Economy",2,9);
	//shop_economic->setIconImg("coins");
	//shop_economic->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Economic)));
	//Artilerry in shop
	Button* shop_artillery;
	if (model->diff == 0)
	{
		shop_artillery = (Button*)sideShop->add("b", std::to_string(model->metaMapEasy[Entity::Artillery - 5][0].cost) + " Artilery", 2, 9);
		shop_artillery->setIconImg("coins");
		shop_artillery->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Artillery)));
	}
	else if (model->diff == 1)
	{
		shop_artillery = (Button*)sideShop->add("b", std::to_string(model->metaMapMedium[Entity::Artillery - 5][0].cost) + " Artilery", 2, 9);
		shop_artillery->setIconImg("coins");
		shop_artillery->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Artillery)));
	}
	else if (model->diff == 2)
	{
		shop_artillery = (Button*)sideShop->add("b", std::to_string(model->metaMapHard[Entity::Artillery - 5][0].cost) + " Artilery", 2, 9);
		shop_artillery->setIconImg("coins");
		shop_artillery->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Artillery)));
	}
	//Button* shop_artillery = (Button*)sideShop->add("b", std::to_string(model->metaMap[Entity::Artillery-5][0].cost) + " Artilery", 2, 9);
	//shop_artillery->setIconImg("coins");
	//shop_artillery->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Artillery)));



	Button* shop_laser;
	if (model->diff == 0)
	{
		shop_laser = (Button*)sideShop->add("b", std::to_string(model->metaMapEasy[Entity::Laser - 5][0].cost) + " Laser", 2, 9);
		shop_laser->setIconImg("coins");
		shop_laser->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Laser)));
	}
	else if (model->diff == 1)
	{
		shop_laser = (Button*)sideShop->add("b", std::to_string(model->metaMapMedium[Entity::Laser - 5][0].cost) + " Laser", 2, 9);
		shop_laser->setIconImg("coins");
		shop_laser->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Laser)));
	}
	else if (model->diff == 2)
	{
		shop_laser = (Button*)sideShop->add("b", std::to_string(model->metaMapHard[Entity::Laser - 5][0].cost) + " Laser", 2, 9);
		shop_laser->setIconImg("coins");
		shop_laser->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Laser)));
	}
	//Button* shop_laser = (Button*)sideShop->add("b", std::to_string(model->metaMap[Entity::Laser - 5][0].cost) + " Artilery", 2, 9);
	//shop_laser->setIconImg("coins");
	//shop_laser->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Laser)));
	//Button* shop_laser = (Button*)sideShop->add("b", std::to_string(model->metaMap[Entity::Laser - 5][0].cost) + " Laser", 2, 9);
	/*shop_laser->setIconImg("coins");
	shop_laser->addEventlistener("click", std::bind(&Model::setShopMode, app.getModel(), ShopEventArgs(Entity::Type::Laser)));*/

	sideMenu->setMargins(5);
	sideMenu->setOrientaion(vertical);
	sideMenu->setFontSize(8);
	Button* turrets = sideMenu->addButton("T u r r e t s", 0, 10);
	turrets->setOrientaion(vertical);
	Button* spells = sideMenu->addButton("B u i l d i n g s", 0,10);
	spells->setOrientaion(vertical);
	Window* window = new Window(app.WINDOW_WIDTH / 2-400, app.WINDOW_HEIGHT / 2 - 150, 400, 300, 1);
	app.addWindow(window);
	app.setMenu(window);
	window->setOrientaion(vertical);
	window->add("l", "Menu", 1, 14);
	Button* newgamebtn=window->addButton("New game", 5, 11);
	newgamebtn->setTextAlign(Auto);

	newgamebtn->addEventlistener("click", std::bind(&Model::newGame, app.getModel(),_1));
	//newgamebtn->setIconImg("gradient");
	Button* escMenuSaveButton = (Button*)window->add("b", "SaveButton", 5, 11);
	escMenuSaveButton->addEventlistener("click", std::bind(&Model::TempSaveGameButtonFunction, app.getModel()));
	Button* escMenuLoadButton = (Button*)window->add("b", "LoadButton", 5, 11);
	escMenuLoadButton->addEventlistener("click", std::bind(&Model::TempLoadGameButtonFunction, app.getModel()));
	Button* exit = (Button*)window->add("b", "Exit", 5, 11);
	exit->addEventlistener("click", std::bind(&App::exitApp , &app,_1));

	//OPTIONS

	Window* optionsW= new Window(app.WINDOW_WIDTH / 2, app.WINDOW_HEIGHT / 2 - 150, 400, 300, 5);
	app.addWindow(optionsW);
	app.setOptionsWindow(optionsW);
	optionsW->setOrientaion(vertical);
	optionsW->add("l", "Options", 5, 13);
	Button* e=(Button*)optionsW->add("b", "Easy", 5, 13);
	e->addEventlistener("click", std::bind(&Model::setDifficulty, app.getModel(), 0));
	Button* m = (Button*)optionsW->add("b", "Medium", 5, 13);
	m->addEventlistener("click", std::bind(&Model::setDifficulty, app.getModel(), 1));
	Button* h = (Button*)optionsW->add("b", "Hard", 5, 13);
	h->addEventlistener("click", std::bind(&Model::setDifficulty, app.getModel(), 2));

	//ADD STUFF

	Window* upradgeWindow= new Window(app.WINDOW_WIDTH/2- 180, app.WINDOW_HEIGHT -80,app.WINDOW_WIDTH*0.05+100, app.WINDOW_HEIGHT*0.05+60, 1);
	app.addWindow(upradgeWindow);
	upradgeWindow->setOrientaion(horizontal);
	upradgeWindow->transparent = true;
	upradgeWindow->setMargins(20);
	Button*upradgeButton= (Button*) upradgeWindow->add("b", "Upradge", 5, 8);
	upradgeButton->addEventlistener("click", std::bind(&Model::onUpradgeClick, app.getModel()));
	/*upradgeButton->setFlexSize(2);
	Label* lvlLabel = (Label*)upradgeWindow->add("l", "  ", 5, 8);
	lvlLabel->transparent = true;
	lvlLabel->setBorder(false);
	Button* sellButton=(Button*)  upradgeWindow->add("b", "Sell", 5, 8);
	sellButton->setFlexSize(2);*/

	upradgeWindow->updateObjectPositions();

	app.addKeyEventListener(SDLK_F4, std::bind(&App::exitApp,&app,_1));
	app.addKeyEventListener(SDLK_F1, std::bind(&App::onToggleOptions, &app));//HERE

	//SPAWN STUFF
	spawnStuff(model);

}
Container* createFpsCounter(App app) {
	Container* fps=new Container(app.WINDOW_WIDTH - 30, 0, 30, 20);
	app.addContainer(fps);
	fps->setMargins(0);
	fps->setBorder(false);
	return fps;
}
int main(int argc, char* argv[]) {
	//onlyModel(); return 0;
	App app;
	app.init();
	InitUIAndModel(app);
	//fps counter
	Container* fps = createFpsCounter(app); 
	Label* fpslabel = (Label*)fps->add("l", "10", 0);
	SDL_Event event;
	SDL_Rect r;
	int fpsint = 0;
	int since_last = 0;
	int lastprint = 0;
		while (SDL_WaitEvent(&event)&&event.type != SDL_QUIT && !app.isQuit()) {


			SDL_RenderClear(app.getRenderer());
		
			switch (event.type) {
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			//texturák ujra töltése
				/* eger kattintas */
			case SDL_MOUSEBUTTONDOWN:
				app.onClickStart(event.button);
				app.render();
				if (event.button.button == SDL_BUTTON_LEFT) {
					app.setClick(true);
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {			
				}
				break;
				/* egergomb elengedese */
			case SDL_MOUSEBUTTONUP:
				app.onClickEnd(event.button);
				app.render();
				if (event.button.button == SDL_BUTTON_LEFT) {
					app.setClick(false);
				}
				break;
				/* eger mozdulat */
			case SDL_MOUSEMOTION:
				if (app.isClick()) {
				
				}
				app.onMouseMove();
				/* a kovetkezo mozdulat esemenyhez */
			//	app.render();
				break;
			case SDL_KEYDOWN:
				app.keyDown(event.key.keysym.sym);
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						//app.quit = true;
					default:
						break;
				}
			case SDLK_ESCAPE:
				//app.quit = true;
				break;
			}	
		app.render();

		SDL_RenderPresent(app.getRenderer());
		//FPS COUNTER BEG
		SDL_SetRenderDrawColor(app.getRenderer(), 0x00, 0x00, 0x00, 0x00);
		if ((SDL_GetTicks() - since_last) != 0){
			fpsint = (int)(1000 / (SDL_GetTicks() - since_last));
		}
		if ((SDL_GetTicks() - lastprint >200)) {
			char text[10];
			sprintf(text, "%d", fpsint);
			fpslabel->setText(text);
			lastprint = SDL_GetTicks();
		}
		since_last = SDL_GetTicks();
		//FPS COUNTER END
	}
	/* ablak bezarasa */
	SDL_Quit();
	return 0;
}
