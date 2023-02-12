#pragma once

#include <d2d1.h>
#include <string>

class CSprite;

class CMouse
{
	ID2D1Bitmap* m_mousePointer = nullptr;
	int m_xpos, m_ypos; // ±ôºýÀÓ ÇØ°á

public:
	CMouse();
	~CMouse();

	void Render(ID2D1HwndRenderTarget* _pRenderTarget);
	void SetMousePointer(int _xpos, int _ypos);
	void SetXPos(int _xpos) { m_xpos = _xpos; }
	void SetYPos(int _ypos) { m_ypos = _ypos; }
};

