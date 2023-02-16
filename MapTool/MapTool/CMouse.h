#pragma once

#include <d2d1.h>
#include <string>
#include <vector>

class CSprite;

class CMouse
{
	CSprite* m_mousePointer = nullptr;
	int m_xpos, m_ypos; // ±Ù∫˝¿” «ÿ∞·B
	bool m_isPressed;

public:
	CMouse();
	~CMouse();

	void Render(ID2D1HwndRenderTarget* _pRenderTarget);
	void SetMousePointer(int _xpos, int _ypos);
	void ResetMousePointer() { m_mousePointer = nullptr; }
	void SetXPos(int _xpos) { m_xpos = _xpos; }
	void SetYPos(int _ypos) { m_ypos = _ypos; }
	void SetPress(bool _isPressed) { m_isPressed = _isPressed; }

	bool IsPressed() const { return m_isPressed; }
	CSprite* GetMousePointer() const { return m_mousePointer; }
};

