#include "CApp.h"
#include "CCore.h"
#include "CEditWnd.h"
#include "CFileManager.h"

#include <windowsx.h>

CApp* CApp::m_inst = nullptr;

HRESULT CApp::Init(HINSTANCE hInstance, int nCmdShow)
{
	CCore::GetInst()->Init();


	m_editWnd = new CEditWnd(hInstance);
	if (m_editWnd->Create(800, 600, nCmdShow) == false)
		return E_FAIL;

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

