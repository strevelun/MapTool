#include "CEditWnd.h"
#include "CImage.h"
#include "CFileManager.h"

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
		EndPaint(hWnd, &ps);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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

	CFileManager::GetInst()->LoadFile(m_pRenderTarget);

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

	HWND hInnerWnd = CreateWindowEx(0, L"InnerWndClass", L"Inner Window",
		WS_CHILD | WS_VISIBLE,
		0, 0, 300, 300, m_hWnd, NULL, m_hInst, NULL);

	if (!hInnerWnd)
	{
		return 1;
	}

	ShowWindow(hInnerWnd, nCmdShow);
	UpdateWindow(hInnerWnd);
	InvalidateRgn(m_hWnd, NULL, true);
	InvalidateRgn(hInnerWnd, NULL, true);

	return true;
}

LRESULT CEditWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static bool bExpanded = false;
	TVINSERTSTRUCT tvis;
	HTREEITEM hParent, hChild1, hChild2;

	switch (message)
	{
	case WM_NOTIFY:
	{
		LPNMHDR pnmhdr = (LPNMHDR)lParam;
		LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;

		if (pnmtv->action == TVE_EXPAND)
		{
			TVITEM tvi;
			tvi.mask = TVIF_STATE;
			tvi.hItem = pnmtv->itemNew.hItem;
			SendMessage(m_hTreeView, TVM_GETITEM, 0, (LPARAM)&tvi);

			if (pnmhdr->code == TVN_ITEMEXPANDING)
			{
				if ((tvi.state & TVIS_EXPANDEDONCE))
				{
					bExpanded = true;
					TreeView_Expand(m_hTreeView, pnmtv->itemNew.hItem, TVE_EXPAND);
				}
			}
		}
		else if (pnmtv->action == TVE_COLLAPSE)
		{
			if (bExpanded == true)
			{
				bExpanded = false;
				TreeView_Expand(m_hTreeView, pnmtv->itemNew.hItem, TVE_COLLAPSE);
			}
		}
		else if (pnmhdr->hwndFrom == m_hTreeView && pnmhdr->code == NM_CLICK)
		{
			TVHITTESTINFO ht = { 0 };
			DWORD dwPos = GetMessagePos();
			ht.pt.x = GET_X_LPARAM(dwPos);
			ht.pt.y = GET_Y_LPARAM(dwPos);
			ScreenToClient(pnmhdr->hwndFrom, &ht.pt);
			TreeView_HitTest(pnmhdr->hwndFrom, &ht);
			
			if (ht.flags & TVHT_ONITEMLABEL)
			{
			}
			
		}
		break;
	}

	case WM_CREATE:
	{
		m_hTreeView = CreateWindowEx(0, WC_TREEVIEW, TEXT("Tree View"),
			WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
			0, 0, 200, 300, hWnd, (HMENU)ID_TREEVIEW, m_hInst, NULL);

		tvis.hParent = NULL;
		tvis.hInsertAfter = TVI_LAST;
		tvis.item.mask = TVIF_TEXT;
		tvis.item.pszText = LPWSTR(L"Parent Item 1");
		hParent = (HTREEITEM)SendMessage(m_hTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvis);

		tvis.hParent = hParent;
		tvis.hInsertAfter = TVI_LAST;
		tvis.item.pszText = LPWSTR(L"Child Item 1");
		hChild1 = (HTREEITEM)SendMessage(m_hTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvis);

		tvis.hParent = hParent;
		tvis.hInsertAfter = hChild1;
		tvis.item.pszText = LPWSTR(L"Child Item 2");
		hChild2 = (HTREEITEM)SendMessage(m_hTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvis);

		tvis.hParent = NULL;
		tvis.hInsertAfter = hParent;
		tvis.item.pszText = LPWSTR(L"Parent Item 2");
		hParent = (HTREEITEM)SendMessage(m_hTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvis);

		tvis.hParent = hParent;
		tvis.hInsertAfter = TVI_LAST;
		tvis.item.pszText = LPWSTR(L"Child Item 1");
		hChild1 = (HTREEITEM)SendMessage(m_hTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvis);

		tvis.hParent = hParent;
		tvis.hInsertAfter = hChild1;
		tvis.item.pszText = LPWSTR(L"Child Item 2");
		hChild2 = (HTREEITEM)SendMessage(m_hTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvis);

		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);



		return 0;
	}
}