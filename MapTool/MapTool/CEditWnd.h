#pragma once

#include "CBaseWnd.h"

#include <vector>

class CImage;

class CEditWnd
	: CBaseWnd
{
private:
	HWND m_hTreeView;
	std::vector<CImage*> m_vecImage;

public:
	CEditWnd();
	CEditWnd(HINSTANCE _hInst);
	~CEditWnd();

	bool Create(int _w, int _h, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

