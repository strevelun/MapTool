#include "CEditWnd.h"
#include "CSprite.h"
#include "CResourceManager.h"

#include <commctrl.h>
#include <windowsx.h>

#pragma comment(lib, "comctl32.lib")

#define ID_TREEVIEW 100

LRESULT CALLBACK InnerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		COLORREF color = RGB(255,255,255);
		SetBkColor(hdc, color);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_LBUTTONDOWN:
		MessageBox(NULL, L"FD54SA", L"FDSA", MB_OK);
		break;

	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

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
		//w = sprite->GetWidth();
		//h = sprite->GetHeight();
		m_pRenderTarget->DrawBitmap(sprite->GetBitmap(), D2D1::RectF(xpos, ypos, xpos + w, ypos + h));
		xpos += w;
		if (xpos >= 400 - w)
		{
			xpos = 0;
			ypos += h;
		}
	}
	ypos += h;
	wstr = L"블록";
	m_pRenderTarget->DrawTextW(wstr.c_str(), wstr.length(), m_pDWTextFormat, D2D1::RectF(50, ypos, 250, ypos + 40), m_pBlackBrush);
	ypos += 40;

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Block"); i++)
	{
		sprite = CResourceManager::GetInst()->GetImage("Block", i);
		//w = sprite->GetWidth();
		//h = sprite->GetHeight();
		m_pRenderTarget->DrawBitmap(sprite->GetBitmap(), D2D1::RectF(xpos, ypos, xpos + w, ypos + h));
		xpos += w;
		if (xpos >= 400 - w)
		{
			xpos = 0;
			ypos += h;
		}
	}

	ypos += h;
	wstr = L"캐릭터";
	m_pRenderTarget->DrawTextW(wstr.c_str(), wstr.length(), m_pDWTextFormat, D2D1::RectF(50, ypos, 250, ypos + 40), m_pBlackBrush);
	ypos += 40;

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Character"); i++)
	{
		sprite = CResourceManager::GetInst()->GetImage("Character", i);
		//w = sprite->GetWidth();
		//h = sprite->GetHeight();
		m_pRenderTarget->DrawBitmap(sprite->GetBitmap(), D2D1::RectF(xpos, ypos, xpos + w, ypos + h));
		xpos += w;
		if (xpos >= 400 - w)
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

	/*
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = InnerWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(0 + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"InnerWndClass";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		return 1;
	}
	*/
	//HWND hInnerWnd = CreateWindowEx(0, L"InnerWndClass", L"Inner Window",WS_CHILD | WS_VISIBLE, 0, 0, 400, 800, m_hWnd, NULL, m_hInst, NULL);

	//if (!hInnerWnd)
	//{
	//	return 1;
	//}

	//ShowWindow(hInnerWnd, nCmdShow);
	//UpdateWindow(hInnerWnd);
	InvalidateRgn(m_hWnd, NULL, true);
	//InvalidateRgn(hInnerWnd, NULL, true);

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
		
		m_pRenderTarget->EndDraw();

		break;
	}

	case WM_LBUTTONUP:

		break;

	case WM_MOUSEMOVE:
		m_mouse.Render(m_pRenderTarget);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

		return 0;
	}
}