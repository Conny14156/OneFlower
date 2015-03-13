#ifdef _DEBUG
#ifndef EngineWindow_HPP
#define EngineWindow_HPP
#include <Windows.h>	
#include <map>
#include "Vector.h"
#include <string>
#include <SFML\Graphics\RenderWindow.hpp>
#include "EditorUI\EditorField.hpp"	
#include <vector>
#include "EditorUI\EditorIDAssigner.hpp"
#include "EditorUI\EngineFocusWindow.hpp"
#include "GameView.hpp"
#include "EngineObjectListViewer.hpp"
#include "EditorUI\EngineFocusWindow.hpp"
class GameObject;
class EngineWindow
{
public:
	EngineWindow();
	~EngineWindow();

	void setGameObject(GameObject* gameobject);
	void cleanse();
	void update();


//	void setValue(BaseField* var,std::string value);
	void setValue(BaseField* id,std::string value);

	void print(std::string);
	
	HINSTANCE hInstance;

	HWND hWnd;
	EngineObjectListViewer ListViewer;
	EngineFocus focus;


	//GameObject* focusedGameObject = NULL;

	EditorID editorID;
	//std::map<std::string,HWND> Fields;
private:
	Vector2 size;

	std::vector<std::string> printList;
	LPCTSTR windowDefinedName;
	LPCTSTR titleBarDisplay;
	WNDCLASSEX wc;
	//HWND focus;

	
};
#endif
#endif