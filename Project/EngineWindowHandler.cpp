#ifdef _DEBUG
#include "EngineWindow.hpp"
#include "Component\GameObject.h"
#include "Component\RenderComponent.h"
#include "Component\TransformComponent.hpp"
#include "EditorUI\EditorField.hpp"
#include "Engine.hpp"
#include "EditorUI\UIAddon.hpp"
#include <Windows.h>
#include "EditorUI\EditorFieldGroup.hpp"
void EngineWindow::setGameObject(GameObject* t)
{
	int size = 256;
	focus.gameObject = t;
	DestroyWindow(focus.hWnd);
	cleanse();
	if(t)
	{

		RECT window = EditorUI::GetLocalCoordinates(hWnd);
		RECT screen = EditorUI::GetClientCoordinates(hWnd);
		int border_thickness = ((screen.right - screen.left) - (window.right - window.left)) / 2;

		//The focused window of the selectd game object
		focus.hWnd = Engine::Window.focus.addEditor(hWnd,"",window.right - window.left - size - 16,0,size,
			500,
			0);


		std::string lastName = "";

		//LOW: perhaps I should save the Label HWND 2015/02/13
		//This part is for the gameObject infomation 
		Engine::Window.focus.addLabel(focus.hWnd,"Name: ",0,0,std::string("Name: ").size() * 8,32,0);
			
		//All components from the gameObject, gameComponent_it = BaseComponent*
		for(std::map<int,BaseComponent*>::iterator gameComponent_it = t->GetComponents()->begin(); gameComponent_it != t->GetComponents()->end(); gameComponent_it++)
		{
			//For spacing
			int x = 0;
			int y = 32;
			//To avoid Out of range
			if(lastName != "")
			{
				x = EditorUI::GetLocalCoordinates(focus.componentFieldGroup.at(lastName).hWnd).left;
				y = EditorUI::GetLocalCoordinates(focus.componentFieldGroup.at(lastName).hWnd).bottom + 8;
			}
			
			//LOW: Remove this a and get iterator from focus.componentFieldGroup via insert. 15/02/15
			EditorGroup a;
			//= focus.componentFieldGroup.at(gameComponent_it->second->getTypeName());


			//Add a Component Group HWND that hold all the variable HWND
			a.hWnd = CreateWindowEx(0,"Static","",
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
				x,y,
				//Width
				EditorUI::GetLocalCoordinates(focus.hWnd).right / 2,
				//Height
				((gameComponent_it->second->getFields().size()) * 32) + 32,
				focus.hWnd,0 ,0,
				NULL
				);


			a.position.x = x;
			a.position.y = y;
			//Get the name of component
			a.name = gameComponent_it->second->getTypeName();

			//Add a Label with said Component name
			a.label = Engine::Window.focus.addLabel(a.hWnd,a.name,0,0,a.name.size() * 8,16,0);



			lastName = gameComponent_it->second->getTypeName();

			//Insert the Name of the component
			focus.componentFieldGroup.insert(std::pair<std::string,EditorGroup>(gameComponent_it->second->getTypeName(),a));


			//LOW Fix this, Currently using test as a middleman but its not needed atm
			//and It just a COPY of the original (Should not be a problem cause the important part is a pointer)

			//Copy of Registered editor variables from Component
			std::map<std::string,BaseField*> test = gameComponent_it->second->getFields();

			//Get pointer to avoid copy sequences
			EditorGroup* itFG = &focus.componentFieldGroup.at(gameComponent_it->second->getTypeName());

			//Add a Corresponding Field for each registered variable
			for(std::map<std::string,BaseField*>::iterator componentField_it = test.begin(); componentField_it != test.end(); componentField_it++)
				EditorUI::Field::addField(componentField_it->second,itFG,0,0);

			focus.height = EditorUI::GetLocalCoordinates(focus.componentFieldGroup.at(lastName).hWnd).bottom-500;


			//LOW Learn to subclass button without breaking the editor and make a update button
			/*
			//Add a Manual update button
			int tempID = EditorUI::RequestID();
			EditorField<int>* tempField = new EditorField<int>(NULL,"Update_Button");
			tempField->holder = gameComponent_it->second->getFields().begin()->second->holder;
			Engine::Window.focus.addLabel(itFG->hWnd,"Auto Update: ",0,(itFG->variableCount * 8) + 64 + 8,128,32,tempID);
			itFG->field.insert(std::make_pair(tempID,tempField));
			}
			//*/
		}
		SCROLLINFO si = {sizeof(SCROLLINFO),SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS,0,0,0,0,0};
		GetScrollInfo(focus.hWnd,SB_VERT,&si);
		int scrollSpeed = 16;
		int oldPos = si.nPos;
		si.nPage = 0;
		si.nMin = 0;
		si.nMax = Engine::Window.focus.height + 16;
		SetScrollInfo(focus.hWnd,SB_VERT,&si,false);
	}
}

void EngineWindow::setValue(BaseField* id,std::string value)
{
	BaseField* variable = NULL;
	variable = id;
	if(variable)
	{
		if(variable->getType() == EditorField<int>::type)
		{
			EditorField<int>* a = static_cast<EditorField<int>*>(variable);
			int b = std::atoi(value.c_str());
			*a->variable = b;
		}
		else if(variable->getType() == EditorField<double>::type)
		{
			EditorField<double>* a = static_cast<EditorField<double>*>(variable);
			int b = std::atoi(value.c_str());
			*a->variable = b;
		}
		else if(variable->getType() == EditorField<std::string>::type)
		{
			EditorField<std::string>* a = static_cast<EditorField<std::string>*>(variable);
			*a->variable = value;
		}
		else
		{
			MessageBox(Engine::Window.hWnd,"Type is Unknown","Unknown",0);

		}
		if(variable->holder)
			variable->holder->UpdateFromEditor();
		else
			MessageBox(Engine::Window.hWnd,"Empty variable Holder","Error: MissingHolder",NULL);
	}
	else 
		MessageBox(Engine::Window.hWnd, "Empty variable", "Error: Variable", NULL);

}
void EngineWindow::update()
{
















}
#endif