#include "CEditWnd.h"
#include "CSprite.h"
#include "CResourceManager.h"
#include "Settings.h"
#include "resource.h"
#include "CApp.h"

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
	for (int i = 0; i < s_gridY; i++)
	{
		for (int j = 0; j < s_gridX; j++)
		{
			m_pRenderTarget->DrawRectangle(D2D1::RectF(PALETTE_WIDTH + j * 40, i * 40, PALETTE_WIDTH + j * 40 + 40, i * 40 + 40),m_pBlackBrush);
			
			switch (m_pVecBoard->at(i)->at(j))
			{
			case Type::Tile:
				m_pRenderTarget->DrawBitmap(m_pVecBoard->at(i)->at(j)->GetBitmap())
				break;
			}
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
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, IDR_MENU1) == false)
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
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_NEW_MAP:
			DialogBox(m_hInst,                   // application instance
				MAKEINTRESOURCE(IDD_DIALOG1), // dialog box resource
				hWnd,                          // owner window
				DialogProc // dialog box window procedure
			);
			break;
		}
	}
	case WM_LBUTTONDOWN:
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
		m_mouse.PutSprite(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), m_pVecBoard);
		break;
	}
	case WM_MOUSEMOVE:
		m_mouse.SetXPos(GET_X_LPARAM(lParam));
		m_mouse.SetYPos(GET_Y_LPARAM(lParam));
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

		return 0;
	}
}

void CEditWnd::SetBoard(int _gridX, int _gridY)
{
	if (m_pVecBoard != nullptr)
		DestroyBoard();

	m_pVecBoard = new std::vector<std::vector<int>*>(_gridY);
	for (int i = 0; i < _gridY; i++)
		(*m_pVecBoard)[i] = new std::vector<int>(_gridX);
}

void CEditWnd::DestroyBoard()
{
	for (int i = 0; i < m_pVecBoard->size(); i++)
		delete (*m_pVecBoard)[i];
	delete m_pVecBoard;
	m_pVecBoard = nullptr;
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

			CApp::GetInst()->GetEditWnd()->SetBoard(s_gridX, s_GridY);

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