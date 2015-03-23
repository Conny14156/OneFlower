#if defined _DEBUG
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#include "Resource.h"
#include "EngineWindow.hpp"
#include "Engine.hpp"
#include "Vector.h"
#include "EditorUI\UIAddon.hpp"
#include <vector>
#include "EditorUI\EditorFieldGroup.hpp"
#include <Windows.h>

#include "InputDialog.hpp"

#include "Component\GameObject.h"
#include "Component\DialogComponent.hpp"
#include "Component\RenderComponent.h"
#include "Component\TransformComponent.hpp"
#include "Component\RigidComponent.hpp"
#include "Component\DialogComponent.hpp"
#include "Component\HitboxComponent.hpp"
#include "Component\StatsComponent.hpp"
#include "Component\HealthComponent.hpp"

#define SHOW_COMMAND_ID 501
#define SET_NULL_FOCUS_ID 503

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
EngineWindow::EngineWindow():size(1366,768)
{
	windowDefinedName = "EditorWindow";
	titleBarDisplay = "Editor Window";

	//The First Windows structure
	wc.lpszClassName = windowDefinedName;
	wc.lpfnWndProc = WndProc;						// This function is called by windows
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	// Catch double-clicks
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(MENU_ICON));
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(MENU_ICON),IMAGE_ICON,16,16,0);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.lpszMenuName = MAKEINTRESOURCE(MENU_ID);;							// No menu
	wc.cbClsExtra = 0;														// No extra bytes after the window class
	wc.cbWndExtra = 0;														// structure or the window instance
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);;

	if(!RegisterClassEx(&wc))
		MessageBox(NULL,"Error Registering The Window Class","Error",MB_OK | MB_ICONERROR);

	hWnd = CreateWindowEx
		(
		0,							// Extended possibilites for variation
		windowDefinedName,			// Classname
		titleBarDisplay,			// Title Text
		WS_OVERLAPPEDWINDOW,		// default window
		CW_USEDEFAULT,				// Windows decides the position
		CW_USEDEFAULT,				// where the window ends up on the screen
		size.x,						// The programs width
		size.y,						// and height in pixels
		NULL,						// The window is a child-window to desktop
		NULL,						// No menu
		hInstance,					// Program Instance handler
		NULL						// No Window Creation data
		//this
		);
	//Move this
	if(!hWnd)
	{
		MessageBox(NULL,"Error creating window","Error",MB_OK | MB_ICONERROR);
	}
	else
	{
		ShowWindow(hWnd,1);
		UpdateWindow(hWnd);
		ListViewer.start();
	}
}
EngineWindow::~EngineWindow()
{
	cleanse();
	UnregisterClass(windowDefinedName,Engine::Window.hInstance);
}
void EngineWindow::cleanse()
{
	for(auto it = focus.componentFieldGroup.begin(); it != focus.componentFieldGroup.end(); it++)
	{
		for(auto jt = it->second.field.begin(); jt != it->second.field.end(); jt++)
		{
			DestroyWindow(jt->second->label);
			DestroyWindow(jt->second->hWnd);
		}
		DestroyWindow(it->second.hWnd);
	}
	focus.componentFieldGroup.clear();
}
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				#pragma region Buttons
				case SHOW_COMMAND_ID:
				{
					EditorUI::RedirectIOToConsole();
					break;
				}
				case SET_NULL_FOCUS_ID:
				{
					Engine::Window.setGameObject(NULL);
					break;
				}
				#pragma endregion
				#pragma region ADD_MENU

				case ADD_GameObject:
				{
					GameObject* a = new GameObject();
					Engine::game.addGameObject(a);
					Engine::Window.setGameObject(a);
					break;
				}
				case ADD_DialogComponent:
				{
					if(Engine::Window.focus.gameObject)
						Engine::Window.focus.gameObject->AddComponent<DialogComponent>();
					break;
				}
				case ADD_HitboxComponent:
				{
					if(Engine::Window.focus.gameObject)
						Engine::Window.focus.gameObject->AddComponent<HitboxComponent>();
					break;
				}
				case ADD_OverHeadComponent:
				{
					if (Engine::Window.focus.gameObject)
						Engine::Window.focus.gameObject->AddComponent<OverheadComponent>();
					break;
				}
				case ADD_RenderComponent:
				{
					if (Engine::Window.focus.gameObject)
						Engine::Window.focus.gameObject->AddComponent<RenderComponent>();
					break;
				}

				case ADD_RigidComponent:
				{
					if(Engine::Window.focus.gameObject)
						Engine::Window.focus.gameObject->AddComponent<RigidComponent>();
					break;
				}
				case ADD_TransformComponent:
				{
					if(Engine::Window.focus.gameObject)
						Engine::Window.focus.gameObject->AddComponent<TransformComponent>();
					break;
				}
				case ADD_StatsComponent:
				{
					if (Engine::Window.focus.gameObject)
						Engine::Window.focus.gameObject->AddComponent<StatsComponent>();
					break;
				}
				case ADD_HealthComponent:
				{
					if (Engine::Window.focus.gameObject)
						Engine::Window.focus.gameObject->AddComponent<HealthComponent>();
					break;
				}
				#pragma endregion

				#pragma region GUI_MENU
				case ID_SHOW_HIDE_GUI:
				{
					Engine::GUI.showHideGUI();
					break;
				}
				case ID_SHOW_HIDE_MOUSE:
				{
					Engine::GUI.showHideMouse();
					break;
				}
				case ID_SHOW_HIDE_OVERHEAD:
				{
					Engine::GUI.showHideOverhead();
					break;
				}
				#pragma endregion
				
				#pragma region FILE_MENU
				case ID_ADD_ZONE:
				{
					char buf[100] = { 0 };
					char stuff[100] = { 0 };
					InputDialog::InputBox("Add Zone", buf,stuff, 100,hWnd);
					std::string name = buf;
					unsigned int i = std::atoi(stuff);
					Engine::World.EditorAddNewZone(name,i);
					MessageBox(Engine::Window.hWnd, "Not implemented ctrl + f ZoneAdd123", "stuff", NULL);
					break;
				}
				case ID_LOAD_ZONE:
				{
					char buf[100];
					InputDialog::InputBox("Load Zone",buf,NULL, 100, hWnd);
					MessageBox(Engine::Window.hWnd, "Not implemented ctrl + f ZoneLoad123", "stuff", NULL);
					break;
				}
				case ID_SAVE_ZONE:
				{
					char buf[100];
					InputDialog::InputBox("Save Zone", buf, NULL, 100, hWnd);
					MessageBox(Engine::Window.hWnd, "Not implemented ctrl + f ZoneSave123", "stuff", NULL);
					break;
				}
				case ID_REMOVE_ZONE:
				{
					char buf[100];
					InputDialog::InputBox("Remove Zone", buf,NULL, 100, hWnd);
					MessageBox(Engine::Window.hWnd, "Not implemented ctrl + f ZoneRemove123", "stuff", NULL);
					break;
				}
				case ID_FILE_EXIT:
				{
					DestroyWindow(hWnd);
					break;
				}
				#pragma endregion
			}
			switch(HIWORD(wParam))
			{
				case LBN_SELCHANGE:
				{
					Engine::Window.ListViewer.getSelection();
					break;
				}
				default:
					break;
			}
		}
		case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		case WM_MOVE:
		{
			#pragma region WM_MOVE
			RECT a = EditorUI::GetLocalCoordinates(hWnd);
			RECT b = EditorUI::GetLocalCoordinates(Engine::View.hWnd);
			SetWindowPos
				(
				Engine::View.hWnd,hWnd,
				a.left + GetSystemMetrics(SM_CXBORDER) + (GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXEDGE) * 2),
				//LOW: Replace with GetThemeSysSize inside UxTheme.h
				a.top + GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYEDGE) * 2) + GetSystemMetrics(SM_CYMENU),
				b.right - b.left,
				b.bottom - b.top,
				SWP_NOSIZE
				);
			#pragma endregion
			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
void EngineWindow::print(std::string str)
{
	std::cout << str;
}
#endif