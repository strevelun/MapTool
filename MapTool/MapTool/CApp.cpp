#include "CApp.h"
#include "CCore.h"

#include <windowsx.h>

CApp* CApp::m_inst = nullptr;

HRESULT CApp::Init(HINSTANCE hInstance, int nCmdShow)
{
	CCore::GetInst()->Init();

	return S_OK;
}

int CApp::Run()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CCore::GetInst()->CleanupDevice();
	CCore::DeleteInst();

	return (int)msg.wParam;
}

