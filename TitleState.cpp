#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include "TitleState.h"
#include "MenuState.h"
#include "CPPGP2025.h"

#pragma comment(lib, "winmm.lib")

extern void ChangeState(GameState* newState);

void TitleState::Enter(HWND hWnd)
{
	std::cout << "Enter TitleState" << std::endl;
}

void TitleState::Exit()
{
	std::cout << "Exit TitleState" << std::endl;
}

void TitleState::Update(float elapsedTime)
{

}

void TitleState::Render(HDC hdc)
{
	//RECT rect;
	//GetClientRect(GetActiveWindow(), &rect);
	//const TCHAR* text = L"Title Screen - Press Enter to Start";
	//DrawText(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void TitleState::OnKeyDown(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		ChangeState(new MenuState());
	}
}

void TitleState::OnkeyUp(WPARAM wParam)
{

}

void TitleState::OnMouseDown(int x, int y, int button)
{

}

void TitleState::OnMouseUp(int x, int y, int button)
{

}

void TitleState::OnMouseMove(int x, int y)
{

}

