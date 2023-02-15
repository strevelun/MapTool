#pragma once

#include "CBaseWnd.h"
#include "CMouse.h"
#include "Board.h"
#include "Settings.h"

#include <vector>

class CSprite;

class CEditWnd
	: CBaseWnd
{
private:
	//std::vector<CSprite*> m_vecImage;
	CMouse m_mouse;
	MenuEvent m_menuEvent;

private:
	void RenderPalette();

public:
	CEditWnd();
	CEditWnd(HINSTANCE _hInst);
	~CEditWnd();

	bool Create(int _w, int _h, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

