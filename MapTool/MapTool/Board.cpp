#include "Board.h"
#include "Settings.h"
#include "CMouse.h"
#include "CResourceManager.h"

void Board::RenderBoard(ID2D1RenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _pBlackBrush)
{
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			_pRenderTarget->DrawRectangle(D2D1::RectF(PALETTE_WIDTH + j * 40, i * 40, PALETTE_WIDTH + j * 40 + 40, i * 40 + 40), _pBlackBrush);
			CSprite* sprite = &m_pVecBoardTile->at(i)->at(j);
			if (sprite->GetBitmap() == nullptr)
				continue;
			_pRenderTarget->DrawBitmap(sprite->GetBitmap(), D2D1::RectF(j * 40 + PALETTE_WIDTH, i * 40, j * 40 + 40 + PALETTE_WIDTH, i * 40 + 40));
		}
	}
}

void Board::RenderObject(ID2D1RenderTarget* _pRenderTarget)
{
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			CSprite* sprite = &m_pVecBoardObject->at(i)->at(j);
			if (sprite->GetBitmap() == nullptr)
				continue;
			_pRenderTarget->DrawBitmap(sprite->GetBitmap(), D2D1::RectF(j * 40 + PALETTE_WIDTH, i * 40, j * 40 + 40 + PALETTE_WIDTH, i * 40 + 40));
			_pRenderTarget->DrawBitmap(CResourceManager::GetInst()->GetBlockTopImage(sprite)->GetBitmap(), D2D1::RectF(j * 40 + PALETTE_WIDTH, (i - 1) * 40, j * 40 + 40 + PALETTE_WIDTH, (i - 1) * 40 + 40));
		}
	}
}

void Board::PutSprite(int _xpos, int _ypos, CMouse* _mouse)
{
	if (_xpos < PALETTE_WIDTH) return;
	if (_xpos >= (m_gridX * 40) + PALETTE_WIDTH) return;
	if (_ypos >= (m_gridY * 40)) return;

	CSprite *sprite = _mouse->GetMousePointer();
	if (sprite == nullptr) return;
	
	int x = (_xpos - PALETTE_WIDTH) / 40;
	int y = _ypos / 40;

	switch (sprite->GetType())
	{
	case Type::Tile:
		m_pVecBoardTile->at(y)->at(x) = *sprite;
		break;
	case Type::Block:
		m_pVecBoardObject->at(y)->at(x) = *sprite;
		break;
	case Type::Character:
		break;
	}
}

void Board::SetBoard(int _gridX, int _gridY)
{
	DestroyBoard();

	m_gridX = _gridX;
	m_gridY = _gridY;

	m_pVecBoardTile = new std::vector<std::vector<CSprite>*>(_gridY);
	m_pVecBoardObject = new std::vector<std::vector<CSprite>*>(_gridY);
	for (int i = 0; i < _gridY; i++)
	{
		(*m_pVecBoardTile)[i] = new std::vector<CSprite>(_gridX);
		(*m_pVecBoardObject)[i] = new std::vector<CSprite>(_gridX);
	}
}

void Board::DestroyBoard()
{
	if (m_pVecBoardTile == nullptr) return;
	for (int i = 0; i < m_pVecBoardTile->size(); i++)
		delete (*m_pVecBoardTile)[i];
	delete m_pVecBoardTile;
	m_pVecBoardTile = nullptr;

	if (m_pVecBoardObject == nullptr) return;
	for (int i = 0; i < m_pVecBoardObject->size(); i++)
		delete (*m_pVecBoardObject)[i];
	delete m_pVecBoardObject;
	m_pVecBoardObject = nullptr;
}