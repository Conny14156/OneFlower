#include "EditorField.hpp"
#include "UIAddon.hpp"
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include "../Component/TransformComponent.hpp"
#include "../Component/HitboxComponent.hpp"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "EditorField.hpp"
#include "../Component/GameObject.h"
#include "../Component/RenderComponent.h"
#include "../Engine.hpp"
std::map<std::string,std::map<std::string,std::string>> EditorUI:: compiledClass;

void EditorUI::Field::addGroup(EditorGroup* name)
{


}
void EditorUI::Field::addField(BaseField* variable,EditorGroup* group)	
{
	std::string value = "";
	int height = 16;
	//int width = GetLocalCoordinates(group->hwnd).right-GetLocalCoordinates(group->hwnd).left;
	const int space = 16;
	const int start = 32;
	int size = group->variableCount;

	int x = variable->name.size() * 8;
	int y = start + (space + 16) * size;
	
	if(x < 64)
		x = 64;
	x += 8;

	//variable->name[0] = variable->name[0] + ('A' - 'a');
	variable->ID = RequestID();
	
	//LOW: Keep adding diffrent types
	if(variable->getType() == EditorField<std::string>::type)
		#pragma region std::string
	{
		EditorField<std::string>* a = static_cast<EditorField<std::string>*>(variable);
		value = a->variable->c_str();
		size_t multi = value.find("\n");
		size_t multiNigo = value.find("\\n");
		if(multi == std::string::npos && multiNigo == std::string::npos)
			variable->hwnd = addTextbox(group->hwnd,value,x,y,128,height,variable->ID);
		else	
			variable->hwnd = addTextboxMulti(group->hwnd,value,x,y,128,height+16,variable->ID);

		
		
		
		group->field.insert(std::make_pair(variable->ID,a));
	}
		#pragma endregion
	else if(variable->getType() == EditorField<int>::type)
		#pragma region int
	{
		EditorField<int>* a = static_cast<EditorField<int>*>(variable);
		value = std::to_string(*a->variable);
		variable->hwnd = addTextboxInt(group->hwnd,value,x,y,64,height,variable->ID);
		variable->flags = FieldFlag::Numbers_Only;
		group->field.insert(std::make_pair(variable->ID,a));
	}
		#pragma endregion
	else
	{
		//LOW Put y = inside vector2
		y = start + (space + 16) * size / 2;
		if(variable->getType() == EditorField<Vector2>::type)
			#pragma region Vector2
		{
			EditorField<Vector2>* a = static_cast<EditorField<Vector2>*>(variable);
			EditorField<double>* varX = new EditorField<double>(&a->variable->x,a->name + ".x");
			EditorField<double>* varY = new EditorField<double>(&a->variable->y,a->name + ".y");
			std::string valueNigo = "";
				
			value = std::to_string(a->variable->x);
			valueNigo = std::to_string(a->variable->y);


			//variable->hwnd = addComponentGroup(group->hwnd,"",0,y,width,height +4,variable->ID);
			//variable->label = addLabel(group->hwnd,variable->name,0,0,width,16,RequestID());

			//Variable name	
			varX->ID = RequestID();
			varY->ID = RequestID();

			varX->hwnd = addTextbox(group->hwnd,value,x,y,64,height,varX->ID);
			varY->hwnd = addTextbox(group->hwnd,valueNigo,x + 64 + 8,y,64,height,varY->ID);


			varX->flags = FieldFlag::Decimal | FieldFlag::Left;
			varX->flags = FieldFlag::Decimal | FieldFlag::Right;


			varX->variable = &a->variable->x;
			varY->variable = &a->variable->y;
				
			varX->holder = a->holder;
			varY->holder = a->holder;

			group->field.insert(std::make_pair(varX->ID,varX));
			group->field.insert(std::make_pair(varY->ID,varY));
			group->variableCount++;

		}
			#pragma endregion
	}
	if(!value.empty())
	{
		group->variableCount++;
		variable->label = addLabel(group->hwnd,variable->name,0,y,variable->name.size() * 8,height,RequestID());
	}
	#pragma endregion
}