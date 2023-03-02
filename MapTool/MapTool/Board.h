#pragma once

#include "CSprite.h"
#include "Settings.h"
#include "Layer.h"

#include <vector>

class CMouse;

class Board
{
	static Board* m_inst;
	std::vector<Layer>* m_vecLayer; // 2번 레이어는 깃발 전용 레이어
	std::vector<std::vector<tMenuEvent>*>* m_pVecBoardEvent;

	int m_gridX = 0, m_gridY = 0;

	Board();
	~Board();
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
	void RemoveSprite(int _xpos, int _ypos);
	void RemoveEvent(int _xpos, int _ypos, tMenuEvent _event);
	void PutEvent(int _xpos, int _ypos, tMenuEvent _event);
	void SetBoard(int _gridX, int _gridY);
	void DestroyBoard();

	void SaveMap(HWND _hWnd);
	void LoadMap(HWND _hWnd, ID2D1RenderTarget* _pRenderTarget);

	void DrawRect(ID2D1RenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _brush, int x, int y, int _strokeWidth = 1);
};