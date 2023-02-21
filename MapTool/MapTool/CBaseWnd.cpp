#include "CBaseWnd.h"
#include "CCore.h"
#pragma comment(lib, "dwrite.lib")

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

CBaseWnd::CBaseWnd()
{
}

CBaseWnd::CBaseWnd(HINSTANCE hInstance)
{
	m_hInst = hInstance;

}

CBaseWnd::~CBaseWnd()
{
}

bool CBaseWnd::Create(LPCWSTR _lpszClassName, int _width, int _height, int nCmdShow, DWORD _dwStyle, int _menu)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(_menu);
	wcex.lpszClassName = _lpszClassName;
	wcex.hIconSm = LoadIcon(m_hInst, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, _width, _height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(_lpszClassName, L"MapTool", _dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, m_hInst,
		this);
	if (!m_hWnd) return false;

	m_pRenderTarget = CCore::GetInst()->CreateRenderTarget(m_hWnd);


	return true;
}

void CBaseWnd::Render()
{
}


LRESULT CBaseWnd::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	CBaseWnd* pWnd = nullptr;

	if (_message == WM_NCCREATE)
	{
		LPCREATESTRUCT pCS = (LPCREATESTRUCT)_lParam;
		SetLastError(0);
		pWnd = (CBaseWnd*)pCS->lpCreateParams;

		if (!SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)pWnd))
		{
			if (GetLastError() != 0)
				return E_FAIL;
		}
	}
	else
	{
		pWnd = reinterpret_cast<CBaseWnd*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));
	}

	if (pWnd)
		return pWnd->Proc(_hWnd, _message, _wParam, _lParam);

	return DefWindowProc(_hWnd, _message, _wParam, _lParam);
}
