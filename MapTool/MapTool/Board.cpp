#include "Board.h"
#include "Settings.h"
#include "CMouse.h"
#include "CResourceManager.h"

Board* Board::m_inst = nullptr;

void Board::RenderBoard(ID2D1RenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _pBlackBrush)
{
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			_pRenderTarget->DrawRectangle(
				D2D1::RectF(PALETTE_WIDTH + j * BOARD_BOX_SIZE,
					i * BOARD_BOX_SIZE,
					PALETTE_WIDTH + j * BOARD_BOX_SIZE + BOARD_BOX_SIZE,
					i * BOARD_BOX_SIZE + BOARD_BOX_SIZE), _pBlackBrush);
			CSprite* sprite = &m_pVecBoardTile->at(i)->at(j);
			if (sprite->GetBitmap() == nullptr)
				continue;
			_pRenderTarget->DrawBitmap(sprite->GetBitmap(), 
				D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE,
					j * BOARD_BOX_SIZE + BOARD_BOX_SIZE + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE + BOARD_BOX_SIZE));
		}
	}
	
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			CSprite* sprite = &m_pVecBoardObject->at(i)->at(j);
			if (sprite->GetBitmap() != nullptr)
			{
				_pRenderTarget->DrawBitmap(sprite->GetBitmap(),
					D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH,
						i * BOARD_BOX_SIZE,
						j * BOARD_BOX_SIZE + BOARD_BOX_SIZE + PALETTE_WIDTH,
						i * BOARD_BOX_SIZE + BOARD_BOX_SIZE));
				_pRenderTarget->DrawBitmap(CResourceManager::GetInst()->GetBlockTopImage(sprite)->GetBitmap(),
					D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH,
						(i - 1) * BOARD_BOX_SIZE,
						j * BOARD_BOX_SIZE + BOARD_BOX_SIZE + PALETTE_WIDTH,
						(i - 1) * BOARD_BOX_SIZE + BOARD_BOX_SIZE));
			}

			sprite = &m_pVecBoardCharacter->at(i)->at(j);
			if (sprite->GetBitmap() == nullptr)
				continue;

			_pRenderTarget->DrawBitmap(sprite->GetBitmap(),
				D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE - (sprite->GetHeight() - BOARD_BOX_SIZE),
					j * BOARD_BOX_SIZE + BOARD_BOX_SIZE + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE + BOARD_BOX_SIZE));
		}
	}

	
}

void Board::PutSprite(int _xpos, int _ypos, CMouse* _mouse)
{
	if (_xpos < PALETTE_WIDTH) return;
	if (_xpos >= (m_gridX * BOARD_BOX_SIZE) + PALETTE_WIDTH) return;
	if (_ypos >= (m_gridY * BOARD_BOX_SIZE)) return;

	CSprite *sprite = _mouse->GetMousePointer();
	if (sprite == nullptr) return;
	
	int x = (_xpos - PALETTE_WIDTH) / BOARD_BOX_SIZE;
	int y = _ypos / BOARD_BOX_SIZE;

	switch (sprite->GetType())
	{
	case Type::Tile:
		m_pVecBoardTile->at(y)->at(x) = *sprite;
		break;
	case Type::Block:
		m_pVecBoardObject->at(y)->at(x) = *sprite;
		break;
	case Type::Character:
		m_pVecBoardCharacter->at(y)->at(x) = *sprite;
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
	m_pVecBoardCharacter = new std::vector<std::vector<CSprite>*>(_gridY);
	for (int i = 0; i < _gridY; i++)
	{
		(*m_pVecBoardTile)[i] = new std::vector<CSprite>(_gridX);
		(*m_pVecBoardObject)[i] = new std::vector<CSprite>(_gridX);
		(*m_pVecBoardCharacter)[i] = new std::vector<CSprite>(_gridX);
	}
}

void Board::DestroyBoard()
{
	if (m_pVecBoardTile != nullptr)
	{
		for (int i = 0; i < m_pVecBoardTile->size(); i++)
			delete (*m_pVecBoardTile)[i];
		delete m_pVecBoardTile;
		m_pVecBoardTile = nullptr;
	}

	if (m_pVecBoardObject != nullptr)
	{
		for (int i = 0; i < m_pVecBoardObject->size(); i++)
			delete (*m_pVecBoardObject)[i];
		delete m_pVecBoardObject;
		m_pVecBoardObject = nullptr;
	}

	if (m_pVecBoardCharacter != nullptr)
	{
		for (int i = 0; i < m_pVecBoardCharacter->size(); i++)
			delete (*m_pVecBoardCharacter)[i];
		delete m_pVecBoardCharacter;
		m_pVecBoardCharacter = nullptr;
	}
}