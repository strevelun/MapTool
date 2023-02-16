#include "CSprite.h"

CSprite::CSprite()
{
}

CSprite::~CSprite()
{
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
