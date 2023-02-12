#pragma once

#include <d2d1.h>

enum class Type
{
	Tile,
	Block,
	Character
};

class CSprite
{
	float m_pivotX = 0.5f, m_pivotY = 0.5f;

	D2D1_RECT_F m_size = {};
	DWORD* m_pixel = nullptr;
	ID2D1Bitmap* m_bitmap = nullptr;

public:
	CSprite();
	~CSprite();
	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }
	void SetPixel(DWORD* _pixel) { m_pixel = _pixel; }
	void SetRect(D2D1_RECT_F _rect) { m_size = _rect; }

	ID2D1Bitmap* GetBitmap() const { return m_bitmap; }
	int GetWidth() const { return m_size.right - m_size.left; }
	int GetHeight() const { return m_size.bottom - m_size.top; }
	D2D1_RECT_F GetRect() const { return m_size; }
};

