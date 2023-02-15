#pragma once

#include "CSprite.h"
#include "Settings.h"

#include <vector>

class CMouse;

class Board
{
	static Board* m_inst;
	std::vector<std::vector<CSprite>*>* m_pVecBoardTile;
	std::vector<std::vector<CSprite>*>* m_pVecBoardObject;
	std::vector<std::vector<CSprite>*>* m_pVecBoardCharacter;
	std::vector<std::vector<MenuEvent>*>* m_pVecBoardEvent;

	int m_gridX = 0, m_gridY = 0;

public:
	static Board* GetInst() {
		if (m_inst == nullptr)
			m_inst = new Board;
		return m_inst;
	}

	static void DestroyInst() {
		if (m_inst != nullptr)
			delete m_inst;
		m_inst = nullptr;
	}

	void RenderBoard(ID2D1RenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _pBlackBrush);

	void PutSprite(int _xpos, int _ypos, CMouse* _mouse);
	void PutEvent(int _xpos, int _ypos, MenuEvent _event);
	void SetBoard(int _gridX, int _gridY);
	void DestroyBoard();
};