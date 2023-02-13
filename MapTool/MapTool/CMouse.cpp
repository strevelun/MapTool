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

	_pRenderTarget->DrawBitmap(m_mousePointer, D2D1::RectF(m_xpos-20,m_ypos-20, m_xpos + 20, m_ypos + 20));
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
			m_mousePointer = sprite->GetBitmap();
			m_spriteType = Type::Tile;
			return;
		}
	}

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Block"); i++)
	{
		CSprite* sprite = CResourceManager::GetInst()->GetImage("Block", i);
		D2D1_RECT_F rect = sprite->GetRect();
		if (_xpos > rect.left && _xpos < rect.right && _ypos > rect.top && _ypos < rect.bottom)
		{
			m_mousePointer = sprite->GetBitmap();
			m_spriteType = Type::Block;
			return;
		}
	}

	for (int i = 0; i < CResourceManager::GetInst()->GetVecSize("Character"); i++)
	{
		CSprite* sprite = CResourceManager::GetInst()->GetImage("Character", i);
		D2D1_RECT_F rect = sprite->GetRect();
		if (_xpos > rect.left && _xpos < rect.right && _ypos > rect.top && _ypos < rect.bottom)
		{
			m_mousePointer = sprite->GetBitmap();
			m_spriteType = Type::Character;
			return;
		}
	}
}

void CMouse::PutSprite(int _xpos, int _ypos, std::vector<std::vector<int>>* _vecBoard)
{
	if (_xpos < PALETTE_WIDTH) return;
	if (m_mousePointer == nullptr) return;

	
}
