#pragma once

#include "CBaseWnd.h"
#include "CMouse.h"

#include <vector>

class CSprite;

class CEditWnd
	: CBaseWnd
{
private:
	HWND m_hTreeView;
	std::vector<CSprite*> m_vecImage;
	CMouse m_mouse;

private:
	void RenderPalette();
	void RenderBoard();

public:
	CEditWnd();
	CEditWnd(HINSTANCE _hInst);
	~CEditWnd();

	bool Create(int _w, int _h, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

