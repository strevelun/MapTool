#pragma once


#include <d2d1.h>
#include <map>
#include <vector>
#include <string>

class CSprite;

class CResourceManager
{
private:
	static CResourceManager* m_inst;

	CResourceManager();
	~CResourceManager();
	std::map<std::string, std::vector<CSprite*>> m_mapImage; // todo : delete

public:
	static CResourceManager* GetInst()
	{
		if (m_inst == nullptr)
			m_inst = new CResourceManager();
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

	CSprite* GetImage(std::string key, int idx) 
	{
		return m_mapImage.at(key).at(idx);
	}

	CSprite* GetBlockTopImage(CSprite* _sprite);

	int GetVecSize(std::string key)
	{
		if (m_mapImage.find(key) == m_mapImage.end()) return -1;
		return m_mapImage.at(key).size();
	}
};

