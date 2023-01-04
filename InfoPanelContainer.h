#pragma once
#include"Container.h"
#include"Entity.h"
#include"Label.h"
#include "ThreadSafeVector.h"
class InfoPanelContainer : public Container {
public:
	InfoPanelContainer() :Container() {
	
	}
	InfoPanelContainer(int x, int y, int w, int h, int lvl = 1) :Container(x, y, w, h, lvl) {
		
	}

	Label* nameLabel;
	Label* hpLabel;
	Label* dmgLabel;
	Label* apsLabel;
	Label* speedLabel;
	Label* levelLabel;
	int labelFontSize = 9;
	Container* portrait;
	Container* underportrait;
	int splitPoint = 5;//when the small picture splits
	ThreadSafeVector<Entity*> trackedEntities;
	std::mutex m_;
	void deleteFromTrackedEntities(Entity* e) {
		ThreadSafeVector<Entity*>::iterator it;
		it = trackedEntities.begin_m();
		for (it; it != trackedEntities.end(); it++) {
			if (*it == e) {
				std::cout << "DELETE HAPPENED IN TRACKING\n";
				trackedEntities.erase(it);
				break;
			}
		}
	}

	void updateByField(Field* field) {
		trackedEntities.clear();
		for (int i = 0; i < underportrait->objects.size(); i++) {
			delete underportrait->objects[i];
		}
		//underportrait->objects.clear();
		underportrait->objects.resize(0);

		if (field->getEntities().size() >=1) {
			portrait->setBackgroundTexture(field->getEntities()[0]->getType());
			updateMainPanel(field->getEntities()[0]);

		}
		else if(field->getEntities().size() == 0) {
			portrait->setTextureMode(false);
			nameLabel->setText((char*)"Empty Field");

			hpLabel->setText((char*)"-");
			//hpLabel
			dmgLabel->setText((char*)"-");
			//apsLabel
			speedLabel->setText((char*)"-");
			levelLabel->setText((char*)"-");
		}
		if (field->getEntities().size() >= 1) {
			int i = 0;
			Container* actual=nullptr;
			for (i = 0; i < field->getEntities().size(); i++) {
				trackedEntities.push_back(field->getEntities()[i]);
				if (i % splitPoint == 0) {

					actual = underportrait->addContainer(0);
					actual->setBorder(false);
					actual->transparent = true;
				}

				Button* btn=actual->addButton("", 1, 0);
				btn->setBackgroundTexture(field->getEntities()[i]->getType());
			}
			if (i < splitPoint * 2) {
				while (i <= splitPoint * 2) {
					i += splitPoint;
					Container* ctn = underportrait->addContainer(0);
					ctn->setBorder(false);
					ctn->transparent = true;
				}
			}

			for (i; i % splitPoint != 0; i++) {
				Button* btn =actual->addButton("", 1, 0);
				btn->setBorder(false);
				btn->transparent = true;
			}
		}

		//Multiple units


	}

	void updateMainPanel(Entity* e) {
		if (e == nullptr) return;
		char* pre = new char[10];
		std::cout << Utils::EntityTypeToChars(e->getType()) << std::endl;
		std::cout << Utils::intToChar(e->getHp()) << std::endl;
		std::cout << Utils::intToChar(e->getDmg()) << std::endl;
		std::cout << Utils::intToChar((float)e->getSpeed()*1000) << std::endl;


		//portrait->setBackgroundTexture(e->getType());
		nameLabel->setText(Utils::EntityTypeToChars(e->getType()));

		hpLabel->setText(Utils::intToChar(e->getHp()));
		//hpLabel
		dmgLabel->setText(Utils::intToChar(e->getDmg()));
		//apsLabel
		speedLabel->setText(Utils::intToChar((float)e->getSpeed()*1000.0f));
		if (e->getType() == Entity::Economic) {
			if (Economic* ec = dynamic_cast<Economic*>(e)) {
				levelLabel->setText(Utils::intToChar((float)ec->getInterest()));
			}
		}
		else {
			levelLabel->setText(Utils::intToChar((float)e->getAps() * 1000.0f));
		}
		

	}

	void updateByEntity(Entity* e) {
		for (Entity* act : trackedEntities) {
			if (act == e) {
				updateMainPanel(e);
				break;
			}
		}
	}
	void init() {
		setMargins(5);
		setBorder(false);
		Container* infoPanel = (Container*)this->add("c");
		infoPanel->setOrientaion(Orientation::horizontal);
		Container* info_left = (Container*)infoPanel->add("c", "", 1);
		info_left->setGlobal(global);
		info_left->setMargins(3);
		info_left->setOrientaion(vertical);
		portrait = (Container*)info_left->add("c", "", 3);
		portrait->setFlexSize(6);
		underportrait = (Container*)info_left->add("c", "", 3);
		underportrait->setFlexSize(4);
		underportrait->setOrientaion(vertical);
		portrait->setOrientaion(vertical);
		//info_left->setBackgroundImg("turret_mage_idle");
		Container* info_right = (Container*)infoPanel->add("c", "", 1);
		info_right->setOrientaion(vertical);
		nameLabel=(Label*)info_right->add("l", "Name", 5, labelFontSize);
		hpLabel =(Label*)info_right->add("l", "hp", 5, labelFontSize);
		hpLabel->setIconImg("hearth");
		dmgLabel=(Label*)info_right->add("l", "dmg", 5, labelFontSize);
		dmgLabel->setIconImg("sword");
		//apsLabel=(Label*)info_right->add("l", "aps: 1.2", 5, labelFontSize);
		speedLabel=(Label*)info_right->add("l", "speed", 5, labelFontSize);
		speedLabel->setIconImg("boot");
		levelLabel=(Label*)info_right->add("l", "aps", 5, labelFontSize);
		info_right->setMargins(3);
	}
};