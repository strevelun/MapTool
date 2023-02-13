#pragma once

#include "CBaseWnd.h"
#include "CMouse.h"

#include <vector>

class CSprite;

class CEditWnd
	: CBaseWnd
{
private:
	//std::vector<CSprite*> m_vecImage;
	CMouse m_mouse;
	std::vector<std::vector<CSprite*>*>* m_pVecBoard;

private:
	void RenderPalette();
	void RenderBoard();

public:
	CEditWnd();
	CEditWnd(HINSTANCE _hInst);
	~CEditWnd();

	bool Create(int _w, int _h, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetBoard(int _gridX, int _gridY);
	void DestroyBoard();

};

