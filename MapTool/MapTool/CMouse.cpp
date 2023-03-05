#include "CMouse.h"
#include "CResourceManager.h"
#include "CSprite.h"
#include "Settings.h"
#include "CBitmap.h"

CMouse::CMouse()
{
}

CMouse::~CMouse()
{
}

void CMouse::Render(ID2D1HwndRenderTarget* _pRenderTarget)
{;
	if (m_mousePointer == nullptr) return;

	_pRenderTarget->DrawBitmap(
		CResourceManager::GetInst()->GetIdxBitmap(m_mousePointer->GetIdx())->GetBitmap(),
		D2D1::RectF(m_xpos - 20, m_ypos - 20, m_xpos + 20, m_ypos + 20),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		m_mousePointer->GetRect());
}

void CMouse::SetMousePointer(int _xpos, int _ypos)
{
	if (_xpos >= PALETTE_WIDTH) return;

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Tile"); i++)
	{
		CSprite* sprite = CResourceManager::GetInst()->GetImage("Tile", i);
		D2D1_RECT_F rect = sprite->GetPaletteRect();
		if (_xpos > rect.left && _xpos < rect.right && _ypos > rect.top && _ypos < rect.bottom)
		{
			m_mousePointer = sprite;
			return;
		}
	}

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Block"); i++)
	{
		CSprite* sprite = CResourceManager::GetInst()->GetImage("Block", i);
		D2D1_RECT_F rect = sprite->GetPaletteRect();
		if (_xpos > rect.left && _xpos < rect.right && _ypos > rect.top && _ypos < rect.bottom)
		{
			sprite = CResourceManager::GetInst()->GetImage("Block", i);
			m_mousePointer = sprite;
			return;
		}
	}
	/*
	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Character"); i++)
	{
		CSprite* sprite = CResourceManager::GetInst()->GetImage("Character", i);
		D2D1_RECT_F rect = sprite->GetRect();
		if (_xpos > rect.left && _xpos < rect.right && _ypos > rect.top && _ypos < rect.bottom)
		{
			m_mousePointer = sprite;
			return;
		}
	}
	*/
}