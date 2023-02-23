#pragma once

#include "CSprite.h"
#include <vector>

class Layer
{
	std::vector<std::vector<CSprite*>*>* m_vecSprite;

public:
	void Render(ID2D1RenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _pBlackBrush, int _gridX, int _gridY);
	void SetVecSprite(int _gridX, int _gridY);
	void AddSprite(int _x, int _y, CSprite* _sprite);

	std::vector<std::vector<CSprite*>*>* GetVecSprite() { return m_vecSprite; }
	void DestroyVecSprite();
	CSprite* GetSprite(int _x, int _y) { return m_vecSprite->at(_y)->at(_x); }
};

