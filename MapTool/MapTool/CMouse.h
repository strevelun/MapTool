#pragma once

#include <d2d1.h>
#include <string>

class CSprite;

class CMouse
{
	ID2D1Bitmap* m_mousePointer = nullptr;

public:
	CMouse();
	~CMouse();

	void Render(ID2D1HwndRenderTarget* _pRenderTarget);
	void SetMousePointer(std::string key, int idx);
};

