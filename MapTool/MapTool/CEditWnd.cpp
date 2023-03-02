#include "CEditWnd.h"
#include "CSprite.h"
#include "CResourceManager.h"
#include "Settings.h"
#include "resource.h"
#include "CApp.h"
#include "Board.h"
#include "Camera.h"

#include <commctrl.h>
#include <windowsx.h>

#pragma comment(lib, "comctl32.lib")

int s_gridX, s_gridY;

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void CEditWnd::RenderPalette()
{
	CSprite* sprite = nullptr;

	int xpos = 0, ypos = 0;
	int w = 40, h = 40;
	RECT rect = {0};
	GetClientRect(m_hWnd, &rect);
	m_pRenderTarget->FillRectangle(D2D1::RectF(0, 0, PALETTE_WIDTH, rect.bottom), m_pWhiteBrush);

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
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, IDR_MENU1) == false)
		return false;

	HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
	if (FAILED(hr)) return false;

	hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
	if (FAILED(hr)) return false;

	hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhiteBrush);
	if (FAILED(hr)) return false;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	if (FAILED(hr)) return hr;

	static const WCHAR fontName[] = L"Arial";
	const FLOAT fontSize = 30.0f;

	hr = m_pDWriteFactory->CreateTextFormat(fontName, NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"en-us", &m_pDWTextFormat);
	if (FAILED(hr)) return hr;

	CResourceManager::GetInst()->LoadFiles(m_pRenderTarget, L".\\resource\\");

	InvalidateRgn(m_hWnd, NULL, true);

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return true;
}

LRESULT CEditWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_NEW_MAP:
		{
			HWND dialog = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
			ShowWindow(dialog, SW_SHOW);
			InvalidateRgn(hWnd, NULL, false);
			break;
		}
		case ID_LOAD_MAP:
			Board::GetInst()->LoadMap(hWnd, m_pRenderTarget);
			break;

		case ID_SAVE_MAP:
			Board::GetInst()->SaveMap(hWnd);
			break;

		case ID_BLOCKED:
			m_menuEvent = tMenuEvent::Blocked;
			m_mouse.ResetMousePointer();
			break;
		case ID_SPAWN_CHARACTER:
			m_menuEvent = tMenuEvent::Spawn_Character;
			m_mouse.ResetMousePointer();
			break;
		case ID_SPAWN_MONSTER:
			m_menuEvent = tMenuEvent::Spawn_Monster;
			m_mouse.ResetMousePointer();
			break;
		case ID_DEFAULT:
			m_menuEvent = tMenuEvent::Default;
			m_mouse.ResetMousePointer();
			break;
		}
	}
	case WM_CREATE:
	{

		break;
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		
		if (m_pRenderTarget)
		{
			m_pRenderTarget->BeginDraw();
			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

			Board::GetInst()->RenderBoard(m_pRenderTarget, m_pBlackBrush);
			RenderPalette();

			m_mouse.Render(m_pRenderTarget);
			m_pRenderTarget->EndDraw();
		}
		EndPaint(hWnd, &ps);

		break;
	}

	case WM_MOUSEWHEEL:
	{
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		if(delta > 0)
			Camera::GetInst()->SetScale(0.1f);
		else if(delta < 0)
			Camera::GetInst()->SetScale(-0.1f);
			
		InvalidateRect(hWnd, NULL, false);
		break;
	}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			Camera::GetInst()->UpdateXPos(30);
			break;
		case VK_RIGHT:
			Camera::GetInst()->UpdateXPos(-30);
			break;
		case VK_UP:
			Camera::GetInst()->UpdateYPos(30);
			break;
		case VK_DOWN:
			Camera::GetInst()->UpdateYPos(-30);
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_LBUTTONDOWN:
		m_mouse.SetPress(true);
		break;

	case WM_LBUTTONUP:
	{
		
		if (m_menuEvent == tMenuEvent::Default)
		{
			m_mouse.SetMousePointer(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			Board::GetInst()->PutSprite(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), &m_mouse);
		}
		else
		{
			Board::GetInst()->PutEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), m_menuEvent);
		}
		
		m_mouse.SetPress(false);
		InvalidateRgn(hWnd, NULL, false);
		break;
	}

	case WM_RBUTTONUP:
		if (m_menuEvent == tMenuEvent::Blocked || m_menuEvent == tMenuEvent::Spawn_Character || m_menuEvent == tMenuEvent::Spawn_Monster)
		{
			Board::GetInst()->RemoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), m_menuEvent);
		}
		
		else if (m_menuEvent == tMenuEvent::Default)
		{
			Board::GetInst()->RemoveSprite(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_MOUSEMOVE:
		m_mouse.SetXPos(GET_X_LPARAM(lParam));
		m_mouse.SetYPos(GET_Y_LPARAM(lParam));

		if (m_mouse.IsPressed())
		{
			if (m_menuEvent == tMenuEvent::Default)
			{
				Board::GetInst()->PutSprite(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), &m_mouse);
			}
			else
			{
				Board::GetInst()->PutEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), m_menuEvent);
			}
		}

		InvalidateRgn(hWnd, NULL, false);
		break;

	case WM_SIZE:
	{
		int w = LOWORD(lParam);
		int h = HIWORD(lParam);
		D2D1_SIZE_U size;
		size.width = w;
		size.height = h;
		m_pRenderTarget->Resize(size);
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

		//return 0;
	}
	return 0;

}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR gridX[5], gridY[5];
	WORD gridXLength = 0, gridYLength = 0;

	switch (message)
	{
	case WM_INITDIALOG:
		// Set the default push button to "Cancel." 
		SendMessage(hDlg,
			DM_SETDEFID,
			(WPARAM)IDCANCEL,
			(LPARAM)0);

		return TRUE;

	case WM_COMMAND:
		// Set the default push button to "OK" when the user enters text. 
		if (HIWORD(wParam) == EN_CHANGE)//&&
			//LOWORD(wParam) == IDE_PASSWORDEDIT)
		{
			SendMessage(hDlg,
				DM_SETDEFID,
				(WPARAM)IDOK,
				(LPARAM)0);
		}
		switch (wParam)
		{
		case IDOK:
		{
			// Get number of characters. 
			gridXLength = (WORD)SendDlgItemMessage(hDlg,
				IDC_EDIT,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);

			gridYLength = (WORD)SendDlgItemMessage(hDlg,
				IDC_EDIT2,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);

			if (gridXLength >= 5 || gridYLength >= 5)
			{
				MessageBox(hDlg,
					L"Too many characters.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}
			else if (gridXLength == 0 || gridYLength == 0)
			{
				MessageBox(hDlg,
					L"No characters entered.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}

			// Put the number of characters into first word of buffer. 
			*((LPWORD)gridX) = gridXLength;
			*((LPWORD)gridY) = gridYLength;

			// Get the characters. 
			SendDlgItemMessage(hDlg,
				IDC_EDIT,
				EM_GETLINE,
				(WPARAM)0,       // line 0 
				(LPARAM)gridX);

			SendDlgItemMessage(hDlg,
				IDC_EDIT2,
				EM_GETLINE,
				(WPARAM)0,       // line 0 
				(LPARAM)gridY);

			// Null-terminate the string. 
			gridX[gridXLength] = 0;
			gridY[gridYLength] = 0;


			char input[5];
			WideCharToMultiByte(CP_ACP, 0, gridX, 5, input, 5, NULL, NULL);
			int n = atoi(input);
			s_gridX = n;
			WideCharToMultiByte(CP_ACP, 0, gridY, 5, input, 5, NULL, NULL);
			n = atoi(input);
			s_gridY = n;

			EndDialog(hDlg, TRUE);

			Board::GetInst()->SetBoard(s_gridX, s_gridY);

			return TRUE;
		}
		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		return 0;
	}
	return FALSE;

	UNREFERENCED_PARAMETER(lParam);
}