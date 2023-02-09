#pragma once

#include <d2d1.h>

class CImage
{
	float m_pivotX = 0.5f, m_pivotY = 0.5f;

	D2D1_RECT_F m_size = {};
	DWORD* m_pixel = nullptr;
	ID2D1Bitmap* m_bitmap = nullptr;

public:
	CImage();
	~CImage();
	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }
	void SetPixel(DWORD* _pixel) { m_pixel = _pixel; }
};

