#include "CSprite.h"
#include "Camera.h"

CSprite::CSprite()
{
}

CSprite::~CSprite()
{
	if (m_pixel) delete[] m_pixel; // TODO 
}

void CSprite::CreateAndSetBitmap(ID2D1RenderTarget* _pRenderTarget, DWORD* _pixel)
{
	ID2D1Bitmap* bitmap;
	D2D1_BITMAP_PROPERTIES bpp;
	bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bpp.dpiX = (FLOAT)0;
	bpp.dpiY = (FLOAT)0;
	m_pixel = _pixel;
	_pRenderTarget->CreateBitmap(D2D1::SizeU(m_size.width, m_size.height), _pixel, m_size.width * 4, &bpp, &bitmap);
	m_bitmap = bitmap;
}

void CSprite::Render(ID2D1RenderTarget* _pRenderTarget, int _x, int _y, float _widthScale, float _heightScale)
{
	int cameraX = Camera::GetInst()->GetXPos();
	int cameraY = Camera::GetInst()->GetYPos();
	float cameraScale = Camera::GetInst()->GetScale();

	float left = (_x * BOARD_BOX_SIZE * cameraScale + PALETTE_WIDTH + cameraX);
	float top = (_y * BOARD_BOX_SIZE * cameraScale - (GetHeight() * _heightScale * cameraScale - BOARD_BOX_SIZE * cameraScale) + cameraY);
	float right = (_x * BOARD_BOX_SIZE * cameraScale + (GetWidth() * _widthScale * cameraScale) + PALETTE_WIDTH + cameraX);
	float bottom = (_y * BOARD_BOX_SIZE * cameraScale + (BOARD_BOX_SIZE * cameraScale) + cameraY);

	_pRenderTarget->DrawBitmap(m_bitmap, D2D1::RectF(left, top, right, bottom));
}
