#pragma once

#include <d2d1.h>
#include "Settings.h"

class CSprite
{
	float m_pivotX = 0.5f, m_pivotY = 0.5f;
	D2D1_RECT_F m_rect = {};
	D2D1_SIZE_F m_size = {};
	DWORD* m_pixel = nullptr;
	ID2D1Bitmap* m_bitmap = nullptr;
	Type m_type;
	int m_idx;

public:
	CSprite();
	~CSprite();
	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }
	void SetPixel(DWORD* _pixel) { m_pixel = _pixel; }
	void SetRect(D2D1_RECT_F _rect) { m_rect = _rect; }
	void SetSize(D2D1_SIZE_F _size) { m_size = _size; }
	void SetIdx(int _idx) { m_idx = _idx; }

	ID2D1Bitmap* GetBitmap() const { return m_bitmap; }
	int GetWidth() const { return m_rect.right - m_rect.left; }
	int GetHeight() const { return m_size.height; }
	D2D1_RECT_F GetRect() const { return m_rect; }
	Type GetType() const { return m_type; }
	int GetIdx() const { return m_idx; }

	DWORD* GetPixel() const { return m_pixel; }
	void CreateAndSetBitmap(ID2D1RenderTarget* _pRenderTarget, DWORD* _pixel);

	void Render(ID2D1RenderTarget* _pRenderTarget, int _x, int _y, float _widthScale = 1.0f, float _heightScale = 1.0f);
};

