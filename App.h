#pragma once
#ifndef APP_H
#define APP_H
#include <vector>
#include "Controller.h"
#include "Container.h"
#include "PresentableObject.h"
#include "Window.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <map>
#include"TableContainer.h"
#include "InfoPanelContainer.h"

class App {
private:
	bool quit;
	bool click = false;
	std::vector<Container*> objects;
	SDL_Renderer* rend;
	SDL_Window* window;
	Window* menuWindow;
	InfoPanelContainer* infoPanelContainer;
	Model* model;
	std::string bgImgKey;
	SDL_Rect* screenRect;
	TableContainer* table;
	std::vector<Window*> windowStack;
	Global* global = nullptr;
	Window* focusedWindow=nullptr;

	//***FONTS
public:
	const int WINDOW_WIDTH = 1500;
	const int WINDOW_HEIGHT =750;
	App(SDL_Renderer* r) {
		rend = r;
		//this->global->rend = r;
	}
	App() {}
	//init functions
	void initTTF() {
		
	}
	void initSDL() {
		/* SDL inicializálása és ablak megnyitása */
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
			exit(1);
		}
		window = SDL_CreateWindow("SDL peldaprogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH, WINDOW_HEIGHT, 0);
		SDL_SetWindowFullscreen(window, 0);
		if (window == NULL) {
			SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
			exit(1);
		}
		rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (rend == NULL) {

			SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
			exit(1);
		}
		SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
	}

	void init() {
		initSDL();
		initTTF();
		screenRect = new SDL_Rect();
		screenRect->x = screenRect->y = 0;
		screenRect->w = WINDOW_WIDTH;
		screenRect->h = WINDOW_HEIGHT;


	}
	void initResources() {
		addFont("roboto", 85, (char*)"fonts/roboto/Inconsolata.otf");
		addFont("inc", 100, (char*)"fonts/roboto/RobotoCondensed-Regular.ttf");
		////////////////IMAGES///////////////////////////////////////////////////////////////////////////////////////
		addImg("gradient", "Assets/images/gradient.jpg");
		addImg("btnbg", "Assets/images/btnbg2.jpg");
		addImg("stars", "Assets/images/allofa.jpg");
		addImg("hearth", "Assets/images/hearth.png");
		addImg("coins", "Assets/images/coin.png");
		addImg("stopwatch", "Assets/images/stopwatch.png");
		addImg("troll", "Assets/images/sprites/troll.png");
		addImg("orc", "Assets/images/sprites/orc.png");
		//Character 
		addImg("orc_walk", "Assets/images/sprites/orc_walk.png");
		addImg("orc_attack", "Assets/images/sprites/orc_attack.png");

		addImg("ogre_walk", "Assets/images/sprites/ogre_walk.png");
		addImg("ogre_attack", "Assets/images/sprites/ogre_attack.png");

		addImg("goblin_walk", "Assets/images/sprites/goblin_walk.png");
		addImg("goblin_attack", "Assets/images/sprites/goblin_attack.png");


		addImg("skeleton_walk", "Assets/images/sprites/skeleton_walk.png");
		addImg("skeleton_attack", "Assets/images/sprites/skeleton_attack.png");

		addImg("icons", "Assets/images/sprites/icons.png");
		addImg("boot", "Assets/images/boot2.png");
		addImg("sword", "Assets/images/sword.png");
		addImg("grass", "Assets/images/grass.jpg");

		//turrets-single image
		addImg("turret_mage_idle_1", "Assets/images/sprites/mage_1.png");
		addImg("economy_idle_1", "Assets/images/sprites/economy_1.png");
		addImg("turret_artilerry_idle_1", "Assets/images/sprites/artilerry_1.png");
		addImg("turret_laser_idle_1", "Assets/images/sprites/laser_1.png");

		addImg("turret_mage_idle_2", "Assets/images/sprites/mage_2.png");
		addImg("economy_idle_2", "Assets/images/sprites/economy_2.png");
		addImg("turret_artilerry_idle_2", "Assets/images/sprites/artilerry_2.png");
		addImg("turret_laser_idle_2", "Assets/images/sprites/laser_2.png");

		addImg("turret_mage_idle_3", "Assets/images/sprites/mage_3.png");
		addImg("economy_idle_3", "Assets/images/sprites/economy_3.png");
		addImg("turret_artilerry_idle_3", "Assets/images/sprites/artilerry_3.png");
		addImg("turret_laser_idle_3", "Assets/images/sprites/laser_3.png");
	}
	void initPortraitGraphics() {
		Animation* orc_portrait =new Animation("icons", global, rend);
		orc_portrait->setup(10, 18, vertical, 1, 3, 0);
		Animation* mage_portrait = new Animation("icons", global, rend);
		mage_portrait->setup(10, 18, vertical, 1, 4, 0);
		Animation* eco_portrait = new Animation("icons", global, rend);
		eco_portrait->setup(10, 18, vertical, 1, 4, 7);
		
		global->portraitTexture.insert(std::pair<Entity::Type, SDL_Texture*>(Entity::Type::Orc, orc_portrait->frameTextures[0]));
		global->portraitTexture.insert(std::pair<Entity::Type, SDL_Texture*>(Entity::Type::Turret, mage_portrait->frameTextures[0]));
		global->portraitTexture.insert(std::pair<Entity::Type, SDL_Texture*>(Entity::Type::Economic, eco_portrait->frameTextures[0]));

	}
	void initGameGraphics() {
	
		//MAGE
			//animation
			Animation* turret_idle = new Animation("turret_mage_idle_1", global,rend);	//a kép kulcsa a global->images-ben
			turret_idle->setup(1, 1);
			turret_idle->t->stop();
			//sprite
			Sprite* turretSprite = new Sprite(global);
			turretSprite->addAnimation(Entity::Status::idle, turret_idle);
			turretSprite->addSkin("level_2", "turret_mage_idle_2");
			turretSprite->addSkin("level_3", "turret_mage_idle_3");
			table->setSpriteForType(Entity::Type::Turret, turretSprite);
		//MAGE-END
		//ARTILERRY
			//animation
			Animation* artilerry_idle = new Animation("turret_artilerry_idle_1", global, rend);	//a kép kulcsa a global->images-ben
			artilerry_idle->setup(1, 1);
			artilerry_idle->t->stop();
			//sprite
			Sprite* artilerrySprite = new Sprite(global);
			artilerrySprite->addAnimation(Entity::Status::idle, artilerry_idle);
			artilerrySprite->addSkin("level_2", "turret_artilerry_idle_2");
			artilerrySprite->addSkin("level_3", "turret_artilerry_idle_3");
			table->setSpriteForType(Entity::Type::Artillery, artilerrySprite);
		//ARTILERRY-END
		//ARTILERRY
			//animation
			Animation* laser_idle = new Animation("turret_laser_idle_1", global, rend);	//a kép kulcsa a global->images-ben
			laser_idle->setup(1, 1);
			laser_idle->t->stop();
			//sprite
			Sprite* laserSprite = new Sprite(global);
			laserSprite->addAnimation(Entity::Status::idle, laser_idle);
			laserSprite->addSkin("level_2", "turret_laser_idle_2");
			laserSprite->addSkin("level_3", "turret_laser_idle_3");
			table->setSpriteForType(Entity::Type::Laser, laserSprite);
		//ARTILERRY-END
		//ECOOOOOOO TODO
			//animation
			Animation* eco_idle = new Animation("economy_idle_1", global, rend);	//a kép kulcsa a global->images-ben
			eco_idle->setup(1, 1);
			eco_idle->t->stop();
			//sprite
			Sprite* ecoSprite = new Sprite(global);
			ecoSprite->addAnimation(Entity::Status::idle, eco_idle);
			ecoSprite->addSkin("level_2", "economy_idle_2");
			ecoSprite->addSkin("level_3", "economy_idle_3");
			table->setSpriteForType(Entity::Type::Economic, ecoSprite);
		//ORC
		//animations
			Animation* orc_north_walk = new Animation("orc_walk", global, rend);	//a kép kulcsa a global->images-ben
			orc_north_walk->setup(5, 5, vertical, 5, 0, 0);
			orc_north_walk->setTimer(165);

			Animation* orc_north_attack = new Animation("orc_attack", global, rend);	//a kép kulcsa a global->images-ben
			orc_north_attack->setup(5, 4, vertical, 4, 0, 0);
			//sprite
			Sprite* orcSprite = new Sprite(global);
			orcSprite->addAnimation(Entity::Status::walk_north, orc_north_walk);
			orcSprite->addAnimation(Entity::Status::attack_north, orc_north_attack);
			table->setSpriteForType(Entity::Orc, orcSprite);
			//ORC-END
		//GOBLIN-BEGIN
				//animations
			Animation* goblin_north_walk = new Animation("goblin_walk", global, rend);	//a kép kulcsa a global->images-ben
			goblin_north_walk->setup(5, 9, vertical, 9, 0, 0);
			goblin_north_walk->setTimer(70);
			Animation* goblin_north_attack = new Animation("goblin_walk", global, rend);	//a kép kulcsa a global->images-ben
			goblin_north_attack->setup(5, 9, vertical, 9, 0, 0);
			//sprite
			Sprite* goblinSprite = new Sprite(global);
			goblinSprite->addAnimation(Entity::Status::walk_north, goblin_north_walk);
			goblinSprite->addAnimation(Entity::Status::attack_north, goblin_north_attack);
			table->setSpriteForType(Entity::Goblin, goblinSprite);
		//GOBLIN-END
		//OGRE-BEGIN
				//animations
			Animation* ogre_north_walk = new Animation("ogre_walk", global, rend);	//a kép kulcsa a global->images-ben
			ogre_north_walk->setup(5, 5, vertical, 5, 0, 0);
			ogre_north_walk->setTimer(700);
			Animation* ogre_north_attack = new Animation("ogre_attack", global, rend);	//a kép kulcsa a global->images-ben
			ogre_north_attack->setup(5, 4, vertical, 4, 0, 0);
			//sprite
			Sprite* ogreSprite = new Sprite(global);
			ogreSprite->addAnimation(Entity::Status::walk_north, ogre_north_walk);
			ogreSprite->addAnimation(Entity::Status::attack_north, ogre_north_attack);
			table->setSpriteForType(Entity::Ogre, ogreSprite);
		//OGRE-END
		//SKELETON-BEGIN
				//animations
			Animation* skeleton_north_walk = new Animation("skeleton_walk", global, rend);	//a kép kulcsa a global->images-ben
			skeleton_north_walk->setup(5, 5, vertical, 5, 0, 0);
			skeleton_north_walk->setTimer(120);
			Animation* skeleton_north_attack = new Animation("skeleton_attack", global, rend);	//a kép kulcsa a global->images-ben
			skeleton_north_attack->setup(5, 4, vertical, 4, 0, 0);
			//sprite
			Sprite* skeletonSprite = new Sprite(global);
			skeletonSprite->addAnimation(Entity::Status::walk_north, skeleton_north_walk);
			skeletonSprite->addAnimation(Entity::Status::attack_north, skeleton_north_attack);
			table->setSpriteForType(Entity::Skeleton, skeletonSprite);
		//SKELETON-END

	}
	void initGlobalResources() {


	}

	//Editor functions
	void addFont(std::string name,int size, char* path) {
		TTF_Font* f = TTF_OpenFont(path,size);
		this->global->fonts.insert(std::pair<std::string, TTF_Font*>(name, f));
	}
	void addImg(std::string name,std::string path) {
		//TTF_Font* f = TTF_OpenFont(path, size);
		SDL_Texture* img = NULL;
		img = IMG_LoadTexture(rend, Utils::str_tochar(path));
		SDL_Surface* surface= IMG_Load(Utils::str_tochar(path));
		this->global->imagesSurface.insert(std::pair<std::string, SDL_Surface*>(name, surface));
		this->global->imagesTexture.insert(std::pair<std::string, SDL_Texture*>(name,img));
	}
	void addContainer(Container *p) {
		p->setRenderer(rend);
		p->setGlobal(global);
		//(std::cout << typeid(*p).name());
		p->addHitBoxToRegister();
		p->setModel(model);
		objects.push_back(p);
	}
	void addWindow(Window* w) {
		windowStack.push_back(w);
		w->setRenderer(rend);
		w->setGlobal(global);
		//w->addHitBoxToRegister();
		w->setModel(model);

	}
	void addKeyEventListener(SDL_Keycode key, std::function<void(EventArgs)> fun) {

		this->global->keyMap.insert(std::pair<SDL_Keycode, std::function<void(EventArgs)>>(key,fun));
	}

	//graphics
	void render() {
		for (Container* o: objects) {
			o->render();
		}
		for (Window* w : windowStack) {
			w->render();
		}
		std::vector<Window*>::reverse_iterator iter;
		for (iter = windowStack.rbegin(); iter != windowStack.rend();iter++) {
			(*iter)->render();
		}
		//if(focusedWindow)
		//focusedWindow->render(rend);
	}
	void renderBackgroundImg() {
		SDL_RenderCopy(rend, this->global->imagesTexture[bgImgKey], NULL, screenRect);
	}

	//event handle
	SDL_Point getMousePoint() {
		int x;
		int y;
		SDL_GetMouseState(&x, &y);
		SDL_Point p;
		p.x = x;
		p.y = y;
		return p;
	}
	void onMouseMove() {
		SDL_Point p=getMousePoint();
		if (focusedWindow)
			focusedWindow->mouseMove(&p);
		for (PresentableObject* cont : this->windowStack) {
			if (!cont->isVisible()) {
				continue;
			}
			if (SDL_PointInRect(&p, cont->rect)) {
				cont->setHover(true);
				cont->mouseMove(&p);
				break;
			}
			else { cont->setHover(false); }
		}
		
		for (PresentableObject* cont : this->global->objectHitBoxRegister) {
			if (!cont->isVisible()) {
				continue;
			}
			if (SDL_PointInRect(&p, cont->rect)) {
				cont->setHover(true);
				cont->mouseMove(&p);
			}
			else { cont->setHover(false); }

		}
	}
	void orderWindows() {
		
std::sort(windowStack.begin(), windowStack.end(),
	[](const Window* a, const Window* b) -> bool
	{
		return a->getZ() > b->getZ();
	});
	}
	int getMaxWindowZ() {
		int max = 0;
		for (Window* w : windowStack) {
			if (w->getZ() > max) max = w->getZ();
		}
		return max;
	}
	void onClickStart(SDL_MouseButtonEvent event) {
		SDL_Point p = getMousePoint();
		orderWindows();
		if (event.button == SDL_BUTTON_LEFT) {
			for (Window* cont : this->windowStack) {
				if (cont->isVisible())
					if (SDL_PointInRect(&p, cont->rect)) {
						cont->setClick(true);
						cont->triggerClickStart(&p);
						focusedWindow = cont;
						focusedWindow->setZ(getMaxWindowZ() + 2);
						orderWindows();
						return;
					}
			}
			for (PresentableObject* cont : this->global->objectHitBoxRegister) {
				if (!cont->isVisible()) { continue; }
				if (SDL_PointInRect(&p, cont->rect)) {
					cont->setClick(true);
					cont->triggerClickStart(&p);
					return;
				}

			}
		}
		else if (event.button == SDL_BUTTON_RIGHT) {
			model->setShopMode(*(new ShopEventArgs(Entity::Type::Undefined)));
		}

	}
	void onClickEnd(SDL_MouseButtonEvent event) {
		SDL_Point p = getMousePoint();
		if (event.button == SDL_BUTTON_LEFT) {
			for (Window* cont : this->windowStack) {
				if (!cont->isVisible()) { continue; }
				cont->setClick(false);
				if (SDL_PointInRect(&p, cont->rect)) { cont->triggerClickEnd(&p); return; }
			}
			if (focusedWindow)
				focusedWindow->triggerClickEnd(&p);


			for (PresentableObject* cont : this->global->objectHitBoxRegister) {
				if (!cont->isVisible()) { continue; }
				cont->setClick(false);
				if (SDL_PointInRect(&p, cont->rect)) cont->triggerClickEnd(&p);
			}

		}
	}
	void keyDown(SDL_Keycode key) {
		if (key == SDLK_ESCAPE)
			toggleMenu();
		else if (this->global->keyMap.find(key) != this->global->keyMap.end())
			this->global->keyMap[key](EventArgs(0));
	}

	//event functions
	void toggleMenu() {
		if (menuWindow != nullptr) {
			menuWindow->setVisible(!menuWindow->isVisible());
		}
	}
	void exitApp(EventArgs e) {
		App::quit = true;
	}

	//setters
	void setInfoPanelContainer(InfoPanelContainer* c) {
		infoPanelContainer = c;
		infoPanelContainer->init();
	}
	void setClick(bool l) {
		click = l;
	}
	void setModel(Model* m)
	{
		model = m;
	}
	void setMenu(Window* w) {
		menuWindow = w;
		menuWindow->setRenderer(rend);
	}
	void setBackgroundImg(std::string name) {
		if (this->global->imagesTexture.find(name) != this->global->imagesTexture.end())
			bgImgKey = Utils::str_tochar(name);
	}
	Window* optionsWindow;
	void setOptionsWindow(Window* o) {
		optionsWindow = o;
	}
	void onToggleOptions() {
		optionsWindow->setVisible(!optionsWindow->isVisible());
	}

	void onTableClickEvent(Field* f) {
		infoPanelContainer->updateByField(f);
		std::cout << "click "<<f->x<<" "<<f->y<<"\n";
	}
	void updateInfoPanelByEntityEvent(Entity* e) {
		//infoPanelContainer->updateByEntity(e);
		std::cout << "panel updated\n";
	}
	void deleteFromTrackedEntitiesInInfoPanel(Entity* e) {
		//infoPanelContainer->deleteFromTrackedEntities(e);
		std::cout << "panel updated\n";
	}
	void setTable(TableContainer* t) {
		table = t;
		this->global->objectHitBoxRegister.push_back(table);
		table->setOnTableClickEvent(std::bind(&App::onTableClickEvent, this, std::placeholders::_1));	
		table->setOnEntityUpdateEvent(std::bind(&App::updateInfoPanelByEntityEvent, this, std::placeholders::_1));
		table->setOnEntityDeathEvent(std::bind(&App::deleteFromTrackedEntitiesInInfoPanel, this, std::placeholders::_1));
		table->setParentRect(screenRect);
		//table->setRenderer(rend);
		
	}
	void setGlobal(Global* g) {
		this->global = g;
		initGlobalResources();
	}

	//getters
	InfoPanelContainer* getinfoPanelContainer() {
		return infoPanelContainer;
	}
	bool isClick() {
		return click;
	}
	bool isQuit() {
		return quit;
	}
	SDL_Renderer* getRenderer() {
		return rend;
	}
	Model* getModel() {
		return model;
	}
};
#endif // !APP_H
