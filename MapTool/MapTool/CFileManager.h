#pragma once

#include <d2d1.h>

class CFileManager
{
private:
	static CFileManager* m_inst;

	CFileManager();
	~CFileManager();

public:
	static CFileManager* GetInst()
	{
		if (m_inst == nullptr)
			m_inst = new CFileManager();
		return m_inst;
	}

	static void DeleteInst()
	{
		if (m_inst != nullptr)
		{
			delete m_inst;
			m_inst = nullptr;
		}
	}

	void LoadFile(ID2D1HwndRenderTarget* _pRenderTarget);

};

