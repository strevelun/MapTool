#include "CMouse.h"
#include "CResourceManager.h"
#include "CSprite.h"

CMouse::CMouse()
{
}

CMouse::~CMouse()
{
}

void CMouse::Render(ID2D1HwndRenderTarget* _pRenderTarget)
{;
	if (m_mousePointer == nullptr) return;

	_pRenderTarget->DrawBitmap(m_mousePointer, D2D1::RectF(0,0,0,0));
}

void CMouse::SetMousePointer(std::string key, int idx)
{
	m_mousePointer = CResourceManager::GetInst()->GetImage(key, idx)->GetBitmap();
}
