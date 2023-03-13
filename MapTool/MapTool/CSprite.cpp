#include "CSprite.h"
#include "CResourceManager.h"
#include "Camera.h"
#include "CBitmap.h"

CSprite::CSprite()
{
}

CSprite::CSprite(const CSpriteData* _sprite)
{
	m_pivotX = _sprite->m_pivotX;
	m_pivotY = _sprite->m_pivotY;
	m_rect = _sprite->m_rect;
	m_size = _sprite->m_size;
	m_type = _sprite->m_type;
}

CSprite::~CSprite()
{
	
}

void CSprite::Render(ID2D1RenderTarget* _pRenderTarget, int _x, int _y, float _widthScale, float _heightScale)
{
	int cameraX = Camera::GetInst()->GetXPos();
	int cameraY = Camera::GetInst()->GetYPos();
	float cameraScale = Camera::GetInst()->GetScale();

	int width;

	if (m_type == eType::Character)
		width = m_rect.right - m_rect.left;
	else
		width = m_size.width > 30 ? BOARD_BOX_SIZE : m_size.width ; // 블록과 깃발

	float left = (_x * BOARD_BOX_SIZE * cameraScale + PALETTE_WIDTH + cameraX);
	float top = (_y * BOARD_BOX_SIZE * cameraScale - (m_size.height * _heightScale * cameraScale - BOARD_BOX_SIZE * cameraScale) + cameraY) ;
	float right = (_x * BOARD_BOX_SIZE * cameraScale + (width * _widthScale * cameraScale) + PALETTE_WIDTH + cameraX);
	float bottom = (_y * BOARD_BOX_SIZE * cameraScale + (BOARD_BOX_SIZE * cameraScale) + cameraY);

	if (m_size.width < 30) // 깃발 위치 조정
	{
		bottom -= 3 * cameraScale;
		top -= 3 * cameraScale;
	}
	_pRenderTarget->DrawBitmap(
		CResourceManager::GetInst()->GetIdxBitmap(m_idx)->GetBitmap(), 
		D2D1::RectF(left, top, right, bottom),
		1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, 
		m_rect);
}
