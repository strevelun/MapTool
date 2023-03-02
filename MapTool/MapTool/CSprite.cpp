#include "CSprite.h"
#include "Camera.h"

CSprite::CSprite()
{
}

CSprite::~CSprite()
{
	m_bitmap->Release();
}

void CSprite::Render(ID2D1RenderTarget* _pRenderTarget, int _x, int _y, float _widthScale, float _heightScale)
{
	int cameraX = Camera::GetInst()->GetXPos();
	int cameraY = Camera::GetInst()->GetYPos();
	float cameraScale = Camera::GetInst()->GetScale();

	int width = GetWidth() < 40 && GetWidth() > 30 ? GetRect().right - GetRect().left : GetWidth() ; // 블록과 깃발

	float left = (_x * BOARD_BOX_SIZE * cameraScale + PALETTE_WIDTH + cameraX);
	float top = (_y * BOARD_BOX_SIZE * cameraScale - (GetHeight() * _heightScale * cameraScale - BOARD_BOX_SIZE * cameraScale) + cameraY) ;
	float right = (_x * BOARD_BOX_SIZE * cameraScale + (width * _widthScale * cameraScale) + PALETTE_WIDTH + cameraX);
	float bottom = (_y * BOARD_BOX_SIZE * cameraScale + (BOARD_BOX_SIZE * cameraScale) + cameraY);

	if (GetWidth() < 30) // 깃발 위치 조정
	{
		bottom -= 3 * cameraScale;
		top -= 3 * cameraScale;
	}
	_pRenderTarget->DrawBitmap(m_bitmap, D2D1::RectF(left, top, right, bottom));
}
