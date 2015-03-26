#include <fstream>
#include <cereal/archives/xml.hpp>
#include <cereal\types\string.hpp>
#include <cereal\types\polymorphic.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <map>

#include "Component\/OverheadComponent.hpp"
#include "Component\HitboxComponent.hpp"
#include "Component\TransformComponent.hpp"
#include "Component\DialogComponent.hpp"
#include "Component\BaseComponent.hpp"
#include "Component\RenderComponent.h"
#include "Component\GameObject.h"

#include "Text/Message.hpp"
#include "Text/FloatingText.hpp"

#include "ZoneMaker.hpp"
#include "Zone.hpp"
#include "WorldManagement.hpp"

#include "Prefab.hpp"
#include "PrefabContainer.hpp"

#include "Engine.hpp"

#include "LoadAndSave.hpp"

#include "Prefab.hpp"

std::string versionName = "Alpha Test: 1.3v";
CEREAL_REGISTER_ARCHIVE(RenderComponent);

void prefabSave(const GameObject* go)
{
	GameObject testPrefab("test");
	testPrefab.AddComponent(new IBaseComponent<RenderComponent>);
	testPrefab.GetComponent<RenderComponent>()->textureName = "prefabTest.png";
	std::ofstream file("test.prefab",std::ios::binary);
	{
		cereal::BinaryOutputArchive ar(file);
		ar(testPrefab);
	}
}
/*
void saveFile(GameObject& player, Zone& zm)
{
std::string save = "savefile";
save.append(".avfile");

// save character info
// save character inventory, location, xp, gold, quests etc.
// save the current zone
std::ofstream file(save, std::ios::binary);
{
cereal::BinaryOutputArchive ar(file);
ar(player);
ar(zm);
}
}
*/
void loadFile(GameObject& player,Zone& zm)
{
	std::string load = "savefile";
	load.append(".avfile");

	// load character info
	// load character inventory,location,xp, ... etc.
	// load zone...
	std::ifstream file(load,std::ios::binary);
	{
		cereal::BinaryInputArchive ar(file);
		ar(player);
		ar(zm);
	}
}
#ifdef _DEBUG
void testSave()
{
	PrefabContainer con;

	ZoneMap zone;
	zone.ID = 1;

	GameObject *test = new GameObject();
	test->name = "TestPlatform";
	test->GetComponent<TransformComponent>()->position.x = 300;
	test->GetComponent<TransformComponent>()->position.y = 316;
	test->AddComponent(new HitboxComponent());
	test->AddComponent(new RenderComponent("testCutHalf.png"));
	zone.addGameObject(test);

	GameObject *ground = new GameObject();
	ground->name = "da kewl ground";
	ground->GetComponent<TransformComponent>()->position.x = 400;
	ground->GetComponent<TransformComponent>()->position.y = 550;
	ground->AddComponent(new HitboxComponent());
	ground->AddComponent(new RenderComponent("ground.marker_1.png"));
	ground->GetComponent<RenderComponent>()->sprite.setScale(2,1);
	ground->GetComponent<HitboxComponent>()->size.x = ground->GetComponent<HitboxComponent>()->size.x * 2;
	zone.addGameObject(ground);

	GameObject *target = new GameObject();
	target->name = "testTarget";
	target->GetComponent<TransformComponent>()->position.x = 580;
	target->GetComponent<TransformComponent>()->position.y = 480;
	target->AddComponent(new HitboxComponent());
	target->AddComponent(new RenderComponent("testTarget.png"));

	target->AddComponent(new DialogComponent(2));
	target->GetComponent<DialogComponent>()->msg->iconName = "TestDialogChat.png";
	target->GetComponent<DialogComponent>()->dialogMessage = "Senpai!\nNotice me!";
	target->GetComponent<DialogComponent>()->position.x = 64;
	target->GetComponent<DialogComponent>()->position.y = 80;
	target->GetComponent<DialogComponent>()->msg->setOffset(10,5);

	zone.addGameObject(target);

	zone.setBackground(Tile("testBackground_1.png",400,300));
	zone.background.size.x = 4000;
	zone.background.size.y = 800;

	zone.foregrounds.push_back(Tile("blacktree_1.png",250,50));
	zone.foregrounds.push_back(Tile("blacktree_1.png",450,250));
	zone.foregrounds.push_back(Tile("blacktree_1.png",400,250));
	zone.foregrounds.push_back(Tile("blacktree_1.png",350,250));
	zone.foregrounds.push_back(Tile("blacktree_1.png",550,250));
	zone.foregrounds.push_back(Tile("blacktree_1.png",500,250));
	std::ofstream file("test.zone",std::ios::binary);
	{
		cereal::BinaryOutputArchive ar(file);
		ar(zone);
	}

	con.addPrefab(target);
	con.addPrefab(ground);
	con.addPrefab(test);

	//	delete target;
	//	delete ground;
	//	delete test;
	/*

	std::ofstream ffile("prefabtest.prefabs", std::ios::binary);
	{
	cereal::BinaryOutputArchive ar(ffile);
	ar(con);
	}
	*/

	/*
	GameObject go;
	go.name = "testObject";
	go.AddComponent(new DialogComponent(2.0f));
	go.GetComponent<DialogComponent>()->dialogTexture = "TestDialogChat.png";
	go.GetComponent<DialogComponent>()->msg = "Testmsg";
	std::ofstream file("test.binary", std::ios::binary);
	{
	cereal::BinaryOutputArchive ar(file);
	ar(go);
	}
	//*/
}

void testLoad()
{
	/*GameObject test;
	test.name = "SaveTestObjectNotLoaded";

	std::ifstream file("test.binary", std::ios::binary);
	{
	cereal::BinaryInputArchive ar(file);
	ar(test);
	}*/

	PrefabContainer con;
	std::ifstream file("prefabtest.prefabs",std::ios::binary);
	{
		cereal::BinaryInputArchive ar(file);
		ar(con);
	}
}
#endif

#pragma region GameObject
template<class Archive>
void save(Archive& archive,const GameObject& go)
{
	//if (go != NULL)
	{
		archive(versionName);
		archive(go.name);
		archive(go.componentMap.size());
		for(std::map<int,BaseComponent*>::const_iterator it = go.componentMap.begin(); it != go.componentMap.end(); ++it)
		{
			archive(it->first);
			//Make directives id instead and use switches
			if(it->first == IBaseComponent<RenderComponent>::typeID)
			{
				RenderComponent* rcp = (RenderComponent*)it->second;
				archive(*rcp);
			}
			else if(it->first == IBaseComponent<TransformComponent>::typeID)
			{
				TransformComponent* tcp = (TransformComponent*)it->second;
				archive(*tcp);
			}
			else if(it->first == IBaseComponent<HitboxComponent>::typeID)
			{
				HitboxComponent* hcp = (HitboxComponent*)it->second;
				archive(*hcp);
			}
			else if(it->first == IBaseComponent<DialogComponent>::typeID)
			{
				DialogComponent* dcp = (DialogComponent*)it->second;
				archive(*dcp);
			}
			else if(it->first == IBaseComponent<OverheadComponent>::typeID)
			{
				OverheadComponent* ohd = (OverheadComponent*)it->second;
				archive(*ohd);
			}
		}
	}
}

template<class Archive>
void load(Archive& archive,GameObject& go)
{
	RenderComponent rcp;
	TransformComponent tcp;
	HitboxComponent hcp;
	DialogComponent dcp;
	OverheadComponent ohd;
	int ID;
	int componentCount = 0;
	std::string versionName = "";
	{
		archive(versionName);
		//archive(a);
		archive(go.name);
		archive(componentCount);
		for(int i = 0; i < componentCount; i++)
		{
			archive(ID);
			if(ID == IBaseComponent<RenderComponent>::typeID)
			{
				archive(rcp);

				go.AddComponent(new RenderComponent(rcp));

				sf::Sprite* sprite = &go.GetComponent<RenderComponent>()->sprite;

				float x = sprite->getTextureRect().width;
				float y = sprite->getTextureRect().height;
				//sprite->setOrigin(x / 2, y / 2);
			}
			else if(ID == IBaseComponent<TransformComponent>::typeID)
			{
				archive(tcp);
				// change to go.AddComponent(tcp);
				go.GetComponent<TransformComponent>()->position.x = tcp.position.x;
				go.GetComponent<TransformComponent>()->position.y = tcp.position.y;
				go.GetComponent<TransformComponent>()->size.x = tcp.size.x;
				go.GetComponent<TransformComponent>()->size.y = tcp.size.y;
				go.GetComponent<TransformComponent>()->rotation.x = tcp.rotation.x;
				go.GetComponent<TransformComponent>()->rotation.y = tcp.rotation.y;
			}
			else if(ID == IBaseComponent<HitboxComponent>::typeID)
			{
				archive(hcp);
				go.AddComponent(new HitboxComponent(hcp));
			}
			else if(ID == IBaseComponent<DialogComponent>::typeID)
			{
				archive(dcp);
				go.AddComponent(new DialogComponent(dcp));
			}
			else if(ID == IBaseComponent<OverheadComponent>::typeID)
			{
				archive(ohd);
				go.AddComponent(new OverheadComponent(ohd));
			}
		}
	}
}

#pragma endregion

#pragma region RenderComponent
template<class Archive>
void save(Archive& archive,const RenderComponent& rc)
{
	float x = rc.sprite.getScale().x;
	float y = rc.sprite.getScale().y;
	//	archive(rc.getTypeName());
	archive(rc.componentName);
	archive(rc.renderlayer);
	archive(rc.textureName);
	archive(x);
	archive(y);
	archive(rc.size.x);
	archive(rc.size.y);
}
template<class Archive>
void load(Archive& archive,RenderComponent& rc)
{
	float x,y;
	//	archive(rc.getTypeName());
	archive(rc.componentName);
	archive(rc.renderlayer);
	archive(rc.textureName);
	archive(x);
	archive(y);
	rc.sprite.setScale(x,y);
	archive(rc.size.x);
	archive(rc.size.y);
}
#pragma endregion

#pragma region BaseComponent
template<class Archive>
void save(Archive& archive,const BaseComponent& bc)
{
	archive(versionName);
	//archive(bc.componentName);
}
#pragma endregion

#pragma region TransformComponent
template<class Archive>
void save(Archive &ar,const TransformComponent &tc)
{
	//ar(tc.componentName);
	ar(tc.position.x);
	ar(tc.position.y);
	ar(tc.rotation.x);
	ar(tc.rotation.y);
	ar(tc.size.x);
	ar(tc.size.y);
}
template<class Archive>
void load(Archive &ar,TransformComponent &tc)
{
	//ar(tc.componentName);
	ar(tc.position.x);
	ar(tc.position.y);
	ar(tc.rotation.x);
	ar(tc.rotation.y);
	ar(tc.size.x);
	ar(tc.size.y);
}
#pragma endregion

#pragma region HitboxComponent
template <class Archive>
void save(Archive &ar,const HitboxComponent &hc)
{
	ar(hc.position.x);
	ar(hc.position.y);
	ar(hc.size.x);
	ar(hc.size.y);
}
template <class Archive>
void load(Archive &ar,HitboxComponent &hc)
{
	ar(hc.position.x);
	ar(hc.position.y);
	ar(hc.size.x);
	ar(hc.size.y);
}
#pragma endregion

#pragma region DialogComponent

template <class Archive>
void save(Archive& ar,const DialogComponent& dc)
{
	ar(dc.fontName);
	ar(*dc.msg);
	ar(dc.position.x);
	ar(dc.position.y);
	ar(dc.dialogMessage);
}

template <class Archive>
void load(Archive& ar,DialogComponent& dc)
{
	ar(dc.fontName);
	ar(*dc.msg);
	ar(dc.position.x);
	ar(dc.position.y);
	ar(dc.dialogMessage);
}
#pragma endregion

#pragma region OverheadComponent

template <class Archive>
void save(Archive& ar,const OverheadComponent& ohd)
{
	ar(ohd.msg);
	ar(ohd.pos->x);
	ar(ohd.pos->y);
	ar(ohd.offset.x);
	ar(ohd.offset.y);
}

template <class Archive>
void load(Archive& ar,OverheadComponent& ohd)
{
	ar(ohd.msg);
	ar(ohd.pos->x);
	ar(ohd.pos->y);
	ar(ohd.offset.x);
	ar(ohd.offset.y);
}
#pragma endregion

#pragma region ZoneMap
// Saves a Zone with name, id, and vector of tiles, and gameobjects
template<class Archive>
void save(Archive &ar,const ZoneMap &zm)
{
	ar(zm.name);
	ar(zm.ID);
	ar(zm.background);
	ar(zm.foregrounds.size());
	for(int i = 0; i < zm.foregrounds.size(); i++)
	{
		ar(zm.foregrounds[i]);
	}
	ar(zm.objects.size());
	for(int i = 0; i < zm.objects.size(); i++)
	{
		ar(*zm.objects[i]);
	}
}
// Loads a Zone with name, id, and vector of tiles and gameobjects
template<class Archive>
void load(Archive &ar,Zone &zone)
{
	int size = 0;
	GameObject *go = nullptr;
	Tile t;
	ar(zone.name);
	ar(zone.ID);
	ar(t);
	zone.background = t;
	ar(size);
	for(int i = 0; i < size; i++)
	{
		Tile qq;
		ar(qq);
		zone.foregrounds.push_back(Tile(qq));
	}
	ar(size);
	for(int i = 0; i < size; i++)
	{
		go = new GameObject();
		ar(*go);
		zone.objects.push_back(go);
	}
}
#pragma endregion

#pragma region Tile

// Saves a tile texture name, and x, y pos
template<class Archive>
void save(Archive &ar,const Tile & t)
{
	ar(t.name);
	ar(t.position.x);
	ar(t.position.y);
	ar(t.size.x);
	ar(t.size.y);
}

// loads a tile texture name, and x,y pos
template<class Archive>
void load(Archive &ar,Tile & t)
{
	ar(t.name);
	ar(t.position.x);
	ar(t.position.y);
	ar(t.size.x);
	ar(t.size.y);
}
#pragma endregion

#pragma region keybinds

// prepared save function for keybinds
template<class Archive>
void save(Archive &ar,int q)
{
}
// prepared load function for keybinds
template<class Archive>
void load(Archive &ar,int q)
{
}
#pragma endregion

#pragma region zoneInfo
void saveInfo(std::map<unsigned int,Zone>map)
{
	std::ofstream file("zone.info",std::ios::binary);
	{
		cereal::BinaryOutputArchive ar(file);
		std::map<unsigned int,Zone>::iterator it = map.begin();
		ar(map.size());
		for(it; it != map.end(); ++it)
		{
			ar(it->first);
			ar(it->second.getName() + ".zone");
		}
	}
}
bool loadZoneInfo(std::map<unsigned int,std::string> & zoneInfo)
{
	std::ifstream file("zone.info",std::ios::binary);
	if(file.is_open())
	{
		cereal::BinaryInputArchive ar(file);
		unsigned int zoneID;
		std::string zoneName;
		int number;
		ar(number);
		for(int i = 0; i < number; i++)
		{
			ar(zoneID);
			ar(zoneName);
			zoneInfo.insert(std::pair<int,std::string>(zoneID,zoneName));
		}
		return true;
	}
	else
	{
		MessageBox(Engine::Window.hWnd,"Could not open file","zone.info",NULL);
		return false;
	}
}
#pragma endregion

#pragma region zoneFile
bool loadZoneFile(std::string name,Zone& zone)
{
	std::ifstream file(name,std::ios::binary);

	if(file.is_open())
	{
		cereal::BinaryInputArchive ar(file);
		ar(zone); //load(Archive &ar, Zone &zone);
		return true;
	}
	else
	{
		MessageBox(Engine::Window.hWnd,"Could not open file",name.c_str(),NULL);
		return false;
	}
}
#pragma endregion

#pragma region Message
template <class Archive>
void save(Archive& ar,const Message& msg)
{
	std::string txt = msg.text.getString();
	ar(msg.marginWidth);
	ar(msg.maxLength);
	ar(msg.size);
	ar(txt);
	ar(msg.color.r);
	ar(msg.color.g);
	ar(msg.color.b);
	ar(msg.color.a);
	ar(msg.text.getPosition().x);
	ar(msg.text.getPosition().y);
	ar(msg.duration);
}

template <class Archive>
void load(Archive& ar,Message& msg)
{
	std::string text;
	int x = 0;
	int y = 0;
	ar(msg.marginWidth);
	ar(msg.maxLength);
	ar(msg.size);
	ar(text);
	msg = text;
	ar(msg.color.r);
	ar(msg.color.g);
	ar(msg.color.b);
	ar(msg.color.a);
	ar(x);
	ar(y);
	msg.text.setPosition(x,y);
	ar(msg.duration);
}

#pragma endregion

#pragma region FloatingText
template <class Archive>
void save(Archive& ar,const FloatingText& msg)
{
	std::string txt = msg.text.getString();
	ar(msg.marginWidth);
	ar(msg.maxLength);
	ar(msg.size);
	ar(txt);
	ar(msg.color.r);
	ar(msg.color.g);
	ar(msg.color.b);
	ar(msg.color.a);
	ar(msg.iconName);
	ar(msg.text.getPosition().x);
	ar(msg.text.getPosition().y);
	ar(msg.offset.x);
	ar(msg.offset.y);
	ar(msg.duration);
}

template <class Archive>
void load(Archive& ar,FloatingText& msg)
{
	std::string iconName;
	std::string text;
	float x = 0;
	float y = 0;
	ar(msg.marginWidth);
	ar(msg.maxLength);
	ar(msg.size);
	ar(text);
	msg = text;
	ar(msg.color.r);
	ar(msg.color.g);
	ar(msg.color.b);
	ar(msg.color.a);
	ar(iconName);
#ifdef _DEBUG
	try
	{
		msg.setIcon(iconName);
	}
	catch(MissingIconException ex)
	{
		MessageBox(Engine::Window.hWnd,"Missing Dialog Texture","Error:MissingDialogTexture",NULL);

		msg.iconSprite.setTexture(*ex.what());
	}
#else

	msg.setIcon(iconName);

#endif
	ar(x);
	ar(y);
	msg.text.setPosition(x,y);
	ar(msg.offset.x);
	ar(msg.offset.y);
	ar(msg.duration);
}

#pragma endregion

#pragma region Prefab
template <class Archive>
void load(Archive& ar,Prefab& pre)
{
	// untested
	unsigned int type;
	int size;
	ar(size);
	for(size_t i = 0; i < size; ++i)
	{
		ar(type);
		if(type == IBaseComponent<RenderComponent>::typeID)
		{
			RenderComponent* tmp = new RenderComponent();
			ar(*tmp);
			pre.base.push_back(tmp);
		}
		else if(type == IBaseComponent<TransformComponent>::typeID)
		{
			TransformComponent* tmp = new TransformComponent();
			ar(*tmp);
			pre.base.push_back(tmp);
		}
		else if(type == IBaseComponent<HitboxComponent>::typeID)
		{
			HitboxComponent* tmp = new HitboxComponent();
			ar(*tmp);
			pre.base.push_back(tmp);
		}
		else if(type == IBaseComponent<DialogComponent>::typeID)
		{
			DialogComponent* tmp = new DialogComponent();
			ar(*tmp);
			pre.base.push_back(tmp);
		}
		else if(type == IBaseComponent<OverheadComponent>::typeID)
		{
			OverheadComponent* tmp = new OverheadComponent();
			ar(*tmp);
			pre.base.push_back(tmp);
		}
		/*else if (type == IBaseComponent<RigidComponent>::typeID)
		{
		RigidComponent* tmp = new RigidComponent();
		ar(*tmp);
		pre.base.push_back(tmp);
		}*/
	}
	ar(pre.name);
	ar(pre.tag);
}
template <class Archive>
void save(Archive& ar,const Prefab& pre)
{
	ar(pre.base.size());
	int size = pre.base.size();
	for(int i = 0; i < size; ++i)
	{
		int type = pre.base[i]->getType();
		ar(type);
		if(type == IBaseComponent<RenderComponent>::typeID)
		{
			RenderComponent* rcp = (RenderComponent*)pre.base[i];
			ar(*rcp);
		}
		else if(type == IBaseComponent<TransformComponent>::typeID)
		{
			TransformComponent* tcp = (TransformComponent*)pre.base[i];
			ar(*tcp);
		}
		else if(type == IBaseComponent<HitboxComponent>::typeID)
		{
			HitboxComponent* hcp = (HitboxComponent*)pre.base[i];
			ar(*hcp);
		}
		else if(type == IBaseComponent<DialogComponent>::typeID)
		{
			DialogComponent* dcp = (DialogComponent*)pre.base[i];
			ar(*dcp);
		}
		else if(type == IBaseComponent<OverheadComponent>::typeID)
		{
			OverheadComponent* ohd = (OverheadComponent*)pre.base[i];
			ar(*ohd);
		}
		/*else if (type == IBaseComponent<RigidComponent>::typeID)
		{
		RigidComponent* tmp = (OverheadComponent*)pre.base[i];
		ar(*tmp);
		}*/
	}
	ar(pre.name);
	ar(pre.tag);
}
#pragma endregion

#pragma region PrefabContainer

template<class Archive>
void load(Archive& ar,PrefabContainer& con)
{
	int size;
	ar(size);
	for(size_t i = 0; i < size; ++i)
	{
		Prefab pre;
		std::string n;
		ar(n);
		ar(pre);
		con.mapOfPrefabs.insert(std::pair<std::string,Prefab>(n,pre));
	}
}

template<class Archive>
void save(Archive& ar,const PrefabContainer& con)
{
	ar(con.mapOfPrefabs.size());
	for(std::map<std::string,Prefab>::const_iterator it = con.mapOfPrefabs.begin(); it != con.mapOfPrefabs.end(); ++it)
	{
		ar(it->first);
		ar(it->second);
	}
}

#pragma endregion