#pragma once

#include "CBaseWnd.h"
#include "CMouse.h"
#include "Board.h"
#include "Settings.h"

#include <vector>

class CSprite;

// ������â�� �ʿ��� ��ҵ��� ����. 
// �ȷ�Ʈ�� ��������
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

