#ifndef EngineFocusWindow_HPP
#define EngineFocusWindow_HPP
#include <Windows.h>
#include <string>
#include <map>
#include "../../Editor/EditorFieldGroup.hpp"
#include "../../../Vector.h"
class BaseField;
class GameObject;
class EngineFocus
{
public:
	EngineFocus();

	GameObject* gameObject;
	HWND hWnd;
	void start();
	void addExtraField(HWND phWnd,std::string text,BaseField* theVariable,int x,int y,int width,int height);

	HWND addButton(HWND phWnd,std::string buttonDisplayName,int x,int y,int width,int height,int buttonID);
	HWND addTextbox(HWND phWnd,std::string textBoxName,int x,int y,int width,int height,int textboxID);
	HWND addLabel(HWND phWnd,std::string labelDisplay,int x,int y,int width,int height,int labelID);
	HWND addTextboxInt(HWND phWnd,std::string number,int x,int y,int width,int height,int textboxID);
	//HWND addComponentGroup(HWND phWnd,std::string name,int x,int y,int width,int height,int groupID);
	HWND addEditor(HWND phWnd,std::string name,int x,int y,int width,int height,int groupID);
	HWND addTextboxMulti(HWND phWnd,std::string text,int x,int y,int width,int height,int textboxID);

	std::map<HWND,BaseField*> extraFields;
	std::map<std::string,EditorGroup> componentFieldGroup;
	Vector2 scrollPosition;
	Vector2 size;
	BaseField* nameField;

	void cleanse();
	HWND hWndField;

private:
	WNDCLASSEX wc;
	LPCTSTR windowDefinedName;
	LPCTSTR titleBarDisplay;
};

#endif