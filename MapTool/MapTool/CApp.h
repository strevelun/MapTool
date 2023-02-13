#pragma once

#include <windows.h>

#define MAX_LOADSTRING 100
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

class CEditWnd;

class CApp
{
private:
	static CApp* m_inst;

	WCHAR szTitle[MAX_LOADSTRING] = {};
	WCHAR szWindowClass[MAX_LOADSTRING] = {};

	HINSTANCE m_hInst = nullptr;
	HWND m_hWnd = nullptr;

	CEditWnd* m_editWnd;

private:
	CApp() { }
	~CApp() {}


public:
	static CApp* GetInst()
	{
		if (m_inst == nullptr)  m_inst = new CApp();
		return m_inst;
	}

	CEditWnd* GetEditWnd() const { return m_editWnd; }

	HRESULT Init(HINSTANCE hInstance, int nCmdShow);
	int Run();

	LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};