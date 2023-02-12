#include "CEditWnd.h"
#include "CSprite.h"
#include "CResourceManager.h"
#include "Settings.h"

#include <commctrl.h>
#include <windowsx.h>

#pragma comment(lib, "comctl32.lib")

void CEditWnd::RenderPalette()
{
	CSprite* sprite = nullptr;

	int xpos = 0, ypos = 0;
	int w = 40, h = 40;

	std::wstring wstr = L"타일";
	m_pRenderTarget->DrawTextW(wstr.c_str(), wstr.length(), m_pDWTextFormat, D2D1::RectF(50, 0, 250, 40), m_pBlackBrush);
	ypos += 40;

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Tile"); i++)
	{
		sprite = CResourceManager::GetInst()->GetImage("Tile", i);
		D2D1_RECT_F rect = D2D1::RectF(xpos, ypos, xpos + w, ypos + h);
		sprite->SetRect(rect);
		m_pRenderTarget->DrawBitmap(sprite->GetBitmap(), rect);
		xpos += w;
		if (xpos >= PALETTE_WIDTH - w)
		{
			xpos = 0;
			ypos += h;
		}
	}
	xpos = 0;
	ypos += h;
	wstr = L"블록";
	m_pRenderTarget->DrawTextW(wstr.c_str(), wstr.length(), m_pDWTextFormat, D2D1::RectF(50, ypos, 250, ypos + 40), m_pBlackBrush);
	ypos += 40;

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Block"); i++)
	{
		sprite = CResourceManager::GetInst()->GetImage("Block", i);
		D2D1_RECT_F rect = D2D1::RectF(xpos, ypos, xpos + w, ypos + h);
		sprite->SetRect(rect);
		m_pRenderTarget->DrawBitmap(sprite->GetBitmap(), rect);
		xpos += w;
		if (xpos >= PALETTE_WIDTH - w)
		{
			xpos = 0;
			ypos += h;
		}
	}

	xpos = 0;
	ypos += h;
	wstr = L"캐릭터";
	m_pRenderTarget->DrawTextW(wstr.c_str(), wstr.length(), m_pDWTextFormat, D2D1::RectF(50, ypos, 250, ypos + 40), m_pBlackBrush);
	ypos += 40;

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Character"); i++)
	{
		sprite = CResourceManager::GetInst()->GetImage("Character", i);
		D2D1_RECT_F rect = D2D1::RectF(xpos, ypos, xpos + w, ypos + h);
		sprite->SetRect(rect);
		m_pRenderTarget->DrawBitmap(sprite->GetBitmap(), rect);
		xpos += w;
		if (xpos >= PALETTE_WIDTH - w)
		{
			xpos = 0;
			ypos += h;
		}
	}
}

void CEditWnd::RenderBoard()
{
}

CEditWnd::CEditWnd()
{
}

CEditWnd::CEditWnd(HINSTANCE _hInst)
{
	m_hInst = _hInst;
}

CEditWnd::~CEditWnd()
{
}

bool CEditWnd::Create(int _w, int _h, int nCmdShow)
{
	if (CBaseWnd::Create(L"EditClass", _w, _h, nCmdShow,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) == false)
		return false;

	CResourceManager::GetInst()->LoadFile(m_pRenderTarget);

	InvalidateRgn(m_hWnd, NULL, true);

	return true;
}

LRESULT CEditWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		MessageBox(NULL, L"FDSA", L"FDSA", MB_OK);
		break;
	case WM_CREATE:
	{

		break;
	}

	case WM_PAINT:
	{
		if (!m_pRenderTarget) break;
		
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		RenderPalette();
		RenderBoard();

		m_mouse.Render(m_pRenderTarget);
		m_pRenderTarget->EndDraw();

		break;
	}

	case WM_LBUTTONUP:
	{
		m_mouse.SetMousePointer(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}
	case WM_MOUSEMOVE:
		m_mouse.SetXPos(GET_X_LPARAM(lParam));
		m_mouse.SetYPos(GET_Y_LPARAM(lParam));
		//m_mouse.Render(m_pRenderTarget, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

		return 0;
	}
}