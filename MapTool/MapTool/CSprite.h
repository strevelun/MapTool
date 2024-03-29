#pragma once

#include <d2d1.h>
#include "Settings.h"



class CSpriteData
{
	friend class CSprite;

	float m_pivotX = 0.5f, m_pivotY = 0.5f;
	D2D1_RECT_F m_rect = {};
	D2D1_SIZE_F m_size = {};
	DWORD* m_pixel = nullptr;
	ID2D1Bitmap* m_bitmap = nullptr;
	eType m_type;

public:
	void SetSize(D2D1_SIZE_F _size) { m_size = _size; }

	D2D1_SIZE_F GetSize() const { return m_size; }
	eType GetType() const { return m_type; }
};

class CSprite
{
	float m_pivotX = 0.5f, m_pivotY = 0.5f;
	D2D1_RECT_F m_rect = {};
	D2D1_RECT_F m_paletteRect = {};
	D2D1_SIZE_F m_size = {};
	eType m_type;
	int m_idx;
	int m_vecIdx;

public:
	CSprite();
	CSprite(const CSpriteData* _sprite);
	~CSprite();

	void SetRect(D2D1_RECT_F _rect) { m_rect = _rect; }
	void SetPaletteRect(D2D1_RECT_F _rect) { m_paletteRect = _rect; }
	void SetSize(D2D1_SIZE_F _size) { m_size = _size; }
	void SetIdx(int _idx) { m_idx = _idx; }
	void SetVecIdx(int _idx) { m_vecIdx = _idx; }

	int GetWidth() const { return m_size.width; }
	int GetHeight() const { return m_size.height; }
	D2D1_RECT_F GetRect() const { return m_rect; }
	D2D1_RECT_F GetPaletteRect() const { return m_paletteRect; }
	D2D1_SIZE_F GetSize() const { return m_size; }
	eType GetType() const { return m_type; }
	int GetIdx() const { return m_idx; }
	int GetVecIdx() { return m_vecIdx; }

	void Render(ID2D1RenderTarget* _pRenderTarget, int _x, int _y, float _widthScale = 1.0f, float _heightScale = 1.0f);
};

