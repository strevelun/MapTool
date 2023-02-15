#include "CMouse.h"
#include "CResourceManager.h"
#include "CSprite.h"
#include "Settings.h"


CMouse::CMouse()
{
}

CMouse::~CMouse()
{
}

void CMouse::Render(ID2D1HwndRenderTarget* _pRenderTarget)
{;
	if (m_mousePointer == nullptr) return;

	_pRenderTarget->DrawBitmap(m_mousePointer->GetBitmap(), D2D1::RectF(m_xpos - 20, m_ypos - 20, m_xpos + 20, m_ypos + 20));
}

void CMouse::SetMousePointer(int _xpos, int _ypos)
{
	if (_xpos >= PALETTE_WIDTH) return;

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Tile"); i++)
	{
		CSprite* sprite = CResourceManager::GetInst()->GetImage("Tile", i);
		D2D1_RECT_F rect = sprite->GetRect();
		if (_xpos > rect.left && _xpos < rect.right && _ypos > rect.top && _ypos < rect.bottom)
		{
			m_mousePointer = sprite;
			return;
		}
	}

	// 홀수번째만 선택하도록 한다.
	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Block"); i++)
	{
		CSprite* sprite = CResourceManager::GetInst()->GetImage("Block", i);
		D2D1_RECT_F rect = sprite->GetRect();
		if (_xpos > rect.left && _xpos < rect.right && _ypos > rect.top && _ypos < rect.bottom)
		{
			if (i % 2 == 0)
				sprite = CResourceManager::GetInst()->GetImage("Block", i+1);
			m_mousePointer = sprite;
			return;
		}
	}

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
}