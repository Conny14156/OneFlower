#ifndef BaseField_HPP
#define BaseField_HPP
#include <Windows.h>
#include <string>
enum FieldFlag
{
  Decimal		= (1+0),
  Numbers_Only	= (1+1),
  Right			= (1+2),
  Left			= (1+3)


};
class BaseComponent;
class BaseField
{
public:
	virtual ~BaseField() = 0{};
	virtual	std::string getType() = 0;

	//Name Label
	HWND label;

	//Variable name
	std::string name;

	//The variable control 
	HWND hwnd;

	//Control flags
	int flags;

	//ID for Hwnd
	int ID;

	//Owner of Variable
	BaseComponent* holder;

	//Auto call EditorComponentFunction if true
	bool autoUpdate;

};
#endif