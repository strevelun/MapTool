#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>


// 모든 창들이 가져야할 공통적인 요소들이 포함
class CBaseWnd
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInst;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;


public:
	CBaseWnd();
	CBaseWnd(HINSTANCE _hInstance);
	virtual ~CBaseWnd();

	bool Create(LPCWSTR _lpszClassName, int _width, int _height, int nCmdShow, DWORD _dwStyle, int _menu = NULL);
	virtual void Render();

	virtual LRESULT Proc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}

	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
};

