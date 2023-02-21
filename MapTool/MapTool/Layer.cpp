#include "Layer.h"
#include "Camera.h"

void Layer::Render(ID2D1RenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _pBlackBrush, int _gridX, int _gridY)
{
	int cameraX = Camera::GetInst()->GetXPos();
	int cameraY = Camera::GetInst()->GetYPos();

	for (int i = 0; i < _gridY; i++)
	{
		for (int j = 0; j < _gridX; j++)
		{
			_pRenderTarget->DrawRectangle(
				D2D1::RectF(PALETTE_WIDTH + j * BOARD_BOX_SIZE + cameraX,
					i * BOARD_BOX_SIZE + cameraY,
					PALETTE_WIDTH + j * BOARD_BOX_SIZE + BOARD_BOX_SIZE + cameraX,
					i * BOARD_BOX_SIZE + BOARD_BOX_SIZE + cameraY), _pBlackBrush);
			CSprite* sprite = &m_vecSprite->at(i)->at(j);
			if (sprite->GetBitmap() == nullptr)
				continue;
			_pRenderTarget->DrawBitmap(sprite->GetBitmap(),
				D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH + cameraX,
					i * BOARD_BOX_SIZE + cameraY,
					j * BOARD_BOX_SIZE + BOARD_BOX_SIZE + PALETTE_WIDTH + cameraX,
					i * BOARD_BOX_SIZE + BOARD_BOX_SIZE + cameraY));
		}
	}
}

void Layer::SetVecSprite(int _gridX, int _gridY)
{
	m_vecSprite = new std::vector<std::vector<CSprite>*>(_gridY);
	for (int i = 0; i < _gridY; i++)
		(*m_vecSprite)[i] = new std::vector<CSprite>(_gridX);
}

void Layer::PutSprite(int _x, int _y, CSprite* _sprite)
{
	m_vecSprite->at(_y)->at(_x) = *_sprite;
}

void Layer::DestroyVecSprite()
{
	if (m_vecSprite != nullptr)
	{
		for (int i = 0; i < m_vecSprite->size(); i++)
			delete (*m_vecSprite)[i];
		delete m_vecSprite;
		m_vecSprite = nullptr;
	}
}
