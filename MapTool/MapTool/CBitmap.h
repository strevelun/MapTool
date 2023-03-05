#pragma once

#include <d2d1.h>
#include <vector>

class CSprite;

class CBitmap
{
private:
	ID2D1Bitmap* m_bitmap;
	D2D1_SIZE_F m_size;

public:
	CBitmap();
	~CBitmap();

	void SetSize(D2D1_SIZE_F _size) { m_size = _size; }
	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }
	
	D2D1_SIZE_F GetSize() const { return m_size; }
	ID2D1Bitmap* GetBitmap() const { return m_bitmap; }
};

