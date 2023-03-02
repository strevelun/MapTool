#pragma once

#include "CBaseWnd.h"
#include "CMouse.h"
#include "Board.h"
#include "Settings.h"

#include <vector>

class CSprite;

// 에디터창에 필요한 요소들을 가짐. 
// 팔레트를 렌더링함
class CEditWnd
	: CBaseWnd
{
private:
	//std::vector<CSprite*> m_vecImage;
	CMouse m_mouse;
	tMenuEvent m_menuEvent;
	ID2D1SolidColorBrush* m_pBlackBrush = nullptr;
	ID2D1SolidColorBrush* m_pRedBrush = nullptr;
	ID2D1SolidColorBrush* m_pWhiteBrush = nullptr;

	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pDWTextFormat = nullptr;

private:
	void RenderPalette();

public:
	CEditWnd();
	CEditWnd(HINSTANCE _hInst);
	~CEditWnd();

	bool Create(int _w, int _h, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

