#include "Board.h"
#include "Settings.h"
#include "CMouse.h"
#include "CResourceManager.h"
#include "Camera.h"
#include <istream>

#include <stack>

Board* Board::m_inst = nullptr;

Board::Board()
{
}

Board::~Board()
{
}

void Board::RenderBoard(ID2D1RenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _pBlackBrush)
{
	if (m_vecLayer == nullptr) return;

	m_vecLayer->at(0).Render(_pRenderTarget, _pBlackBrush, m_gridX, m_gridY);

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			CSprite* sprite = m_vecLayer->at(1).GetSprite(j, i);
			if (sprite != nullptr)
				sprite->Render(_pRenderTarget, j, i);

			sprite = m_vecLayer->at(2).GetSprite(j, i);
			if (sprite != nullptr)
				sprite->Render(_pRenderTarget, j, i);
		}
	}

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			tMenuEvent menuEvent = m_pVecBoardEvent->at(i)->at(j);
			switch (menuEvent)
			{
			case tMenuEvent::Blocked:
			{
				ID2D1SolidColorBrush* brush = nullptr;
				_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush);
				brush->SetOpacity(0.8);
				DrawRect(_pRenderTarget, brush, j, i);
				brush->Release();
				break;
			}
			case tMenuEvent::Spawn_Character:
			case tMenuEvent::Spawn_Monster:
			{
				ID2D1SolidColorBrush* brush = nullptr;
				CSprite* sprite = nullptr;
				if (menuEvent == tMenuEvent::Spawn_Character)
				{
					_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua), &brush);
					sprite = CResourceManager::GetInst()->GetImage("Character", 0);
				}
				else
				{
					_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &brush);
					sprite = CResourceManager::GetInst()->GetImage("Character", 1);
				}

				if(sprite != nullptr)
					sprite->Render(_pRenderTarget, j, i, 0.625f, 0.7f);

				brush->SetOpacity(0.8);
				DrawRect(_pRenderTarget, brush, j, i, 5);
				brush->Release();
				break;
			}
			}
		}
	}
}

void Board::PutSprite(int _xpos, int _ypos, CMouse* _mouse)
{
	int cameraX = Camera::GetInst()->GetXPos();
	int cameraY = Camera::GetInst()->GetYPos();

	float cameraScale = Camera::GetInst()->GetScale();
	int xpos = (_xpos - cameraX) ;
	int ypos = (_ypos - cameraY) ;

	if (xpos < PALETTE_WIDTH) return;
	if (ypos < 0) return;
	if (xpos >= (m_gridX * BOARD_BOX_SIZE * cameraScale) + PALETTE_WIDTH ) return;
	if (ypos >= (m_gridY * BOARD_BOX_SIZE * cameraScale)) return;

	CSprite *sprite = _mouse->GetMousePointer();
	if (sprite == nullptr) return;
	

	int x = (xpos - PALETTE_WIDTH) / (BOARD_BOX_SIZE * cameraScale);
	int y = ypos / (BOARD_BOX_SIZE * cameraScale);

	// ����� ��� ����
	if (sprite->GetSize().width < 30)
	{
		m_vecLayer->at(2).AddSprite(x, y, sprite);
		return;
	}

	switch (sprite->GetType())
	{
	case eType::Tile:
		m_vecLayer->at(0).AddSprite(x, y, sprite);
		break;
	case eType::Block:
		m_vecLayer->at(1).AddSprite(x, y, sprite);
		break;
	//case Type::Character:
	//	m_vecLayer->at(2).AddSprite(x, y, sprite);
	//	break;
	}
}

void Board::RemoveSprite(int _xpos, int _ypos)
{
	int cameraX = Camera::GetInst()->GetXPos();
	int cameraY = Camera::GetInst()->GetYPos();

	float cameraScale = Camera::GetInst()->GetScale();
	int xpos = (_xpos - cameraX);
	int ypos = (_ypos - cameraY);

	if (xpos < PALETTE_WIDTH) return;
	if (ypos < 0) return;
	if (xpos >= (m_gridX * BOARD_BOX_SIZE * cameraScale) + PALETTE_WIDTH) return;
	if (ypos >= (m_gridY * BOARD_BOX_SIZE * cameraScale)) return;

	int x = (xpos - PALETTE_WIDTH) / (BOARD_BOX_SIZE * cameraScale);
	int y = ypos / (BOARD_BOX_SIZE * cameraScale);


	// ��ߺ���
	CSprite* sprite = m_vecLayer->at(2).GetSprite(x, y);

	if (sprite != nullptr)
	{
		m_vecLayer->at(2).AddSprite(x, y, nullptr);
		return;
	}

	sprite = m_vecLayer->at(1).GetSprite(x, y);

	if (sprite != nullptr)
	{
		m_vecLayer->at(1).AddSprite(x, y, nullptr);
		return;
	}

	sprite = m_vecLayer->at(0).GetSprite(x, y);
	if (sprite != nullptr)
	{
		m_vecLayer->at(0).AddSprite(x, y, nullptr);
		return;
	}
}

void Board::RemoveEvent(int _xpos, int _ypos, tMenuEvent _event)
{
	float cameraScale = Camera::GetInst()->GetScale();
	int xpos = _xpos - Camera::GetInst()->GetXPos();
	int ypos = _ypos - Camera::GetInst()->GetYPos();

	if (xpos < PALETTE_WIDTH) return;
	if (ypos < 0) return;
	if (xpos >= (m_gridX * BOARD_BOX_SIZE * cameraScale) + PALETTE_WIDTH) return;
	if (ypos >= (m_gridY * BOARD_BOX_SIZE * cameraScale)) return;

	int x = (xpos - PALETTE_WIDTH) / (BOARD_BOX_SIZE * cameraScale);
	int y = ypos / (BOARD_BOX_SIZE * cameraScale);

	m_pVecBoardEvent->at(y)->at(x) = tMenuEvent::Default;
}

void Board::PutEvent(int _xpos, int _ypos, tMenuEvent _event)
{
	float cameraScale = Camera::GetInst()->GetScale();
	int xpos = _xpos - Camera::GetInst()->GetXPos();
	int ypos = _ypos - Camera::GetInst()->GetYPos();

	if (xpos < PALETTE_WIDTH) return;
	if (ypos < 0) return;
	if (xpos >= (m_gridX * BOARD_BOX_SIZE * cameraScale) + PALETTE_WIDTH) return;
	if (ypos >= (m_gridY * BOARD_BOX_SIZE * cameraScale)) return;

	int x = (xpos  - PALETTE_WIDTH) / (BOARD_BOX_SIZE * cameraScale);
	int y = ypos / (BOARD_BOX_SIZE * cameraScale);

	if (_event == tMenuEvent::Spawn_Character)
		for (int i = 0; i < m_gridY; i++)
			for (int j = 0; j < m_gridX; j++)
				if (m_pVecBoardEvent->at(i)->at(j) == tMenuEvent::Spawn_Character)
					m_pVecBoardEvent->at(i)->at(j) = tMenuEvent::Default;
	m_pVecBoardEvent->at(y)->at(x) = _event;
}

void Board::SetBoard(int _gridX, int _gridY)
{
	DestroyBoard();

	m_gridX = _gridX;
	m_gridY = _gridY;

	m_vecLayer = new std::vector<Layer>(3);

	for (int i = 0; i < m_vecLayer->size(); i++)
	{
		m_vecLayer->at(i).SetVecSprite(_gridX, _gridY);
	}
	m_pVecBoardEvent = new std::vector<std::vector<tMenuEvent>*>(_gridY);	
	for (int i = 0; i < _gridY; i++)
		(*m_pVecBoardEvent)[i] = new std::vector<tMenuEvent>(_gridX);
}

void Board::DestroyBoard()
{
	if (m_vecLayer == nullptr) return;

	m_gridX = 0;
	m_gridY = 0;

	for (int i = 0; i < m_vecLayer->size(); i++)
	{
		m_vecLayer->at(i).DestroyVecSprite();
	}
}

void Board::SaveMap(HWND _hWnd)
{
	if (m_gridX <= 0) return;

	OPENFILENAME ofn;
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"map\0*.map";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = _hWnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 100;
	ofn.lpstrInitialDir = L".";

	if (GetSaveFileName(&ofn) == 0) return;

	std::wstring strFilePath = lpstrFile;
	strFilePath.append(L".");
	strFilePath.append(ofn.lpstrFilter);

	FILE* pFile = nullptr;

	errno_t errNum = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	if (pFile == nullptr || errNum != 0)
		return;

	fwrite(&m_gridX, sizeof(int), 1, pFile);
	fwrite(&m_gridY, sizeof(int), 1, pFile);

	tMenuEvent event;

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			event = m_pVecBoardEvent->at(i)->at(j);
			fwrite(&event, sizeof(tMenuEvent), 1, pFile);
		}
	}

	CSprite* sprite;

	std::vector<std::vector<CSprite*>*>* vecSprite = m_vecLayer->at(0).GetVecSprite();
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			sprite = vecSprite->at(i)->at(j);
			if (sprite == nullptr) continue;
			fwrite(&j, sizeof(int), 1, pFile);
			fwrite(&i, sizeof(int), 1, pFile);
			eType type = sprite->GetType();
			int idx = sprite->GetVecIdx();
			fwrite(&type, sizeof(eType), 1, pFile);
			fwrite(&idx, sizeof(int), 1, pFile);
		}
	}

	vecSprite = m_vecLayer->at(1).GetVecSprite();
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			sprite = vecSprite->at(i)->at(j);
			if (sprite == nullptr) continue;
			fwrite(&j, sizeof(int), 1, pFile);
			fwrite(&i, sizeof(int), 1, pFile);
			eType type = sprite->GetType();
			int idx = sprite->GetVecIdx();
			fwrite(&type, sizeof(eType), 1, pFile);
			fwrite(&idx, sizeof(int), 1, pFile);
		}
	}

	vecSprite = m_vecLayer->at(2).GetVecSprite();
	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			sprite = vecSprite->at(i)->at(j);
			if (sprite == nullptr) continue;
			fwrite(&j, sizeof(int), 1, pFile);
			fwrite(&i, sizeof(int), 1, pFile);
			eType type = sprite->GetType();
			int idx = sprite->GetVecIdx();
			fwrite(&type, sizeof(eType), 1, pFile);
			fwrite(&idx, sizeof(int), 1, pFile);
		}
	}

	fclose(pFile);
}

void Board::LoadMap(HWND _hWnd, ID2D1RenderTarget* _pRenderTarget)
{
	OPENFILENAME ofn;
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"map\0*.map";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = _hWnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 100;
	ofn.lpstrInitialDir = L".";

	if (GetOpenFileName(&ofn) == 0) return;

	// strtok
	std::stack<char> s;
	int i = 0;

	for (int i = wcslen(ofn.lpstrFile); i >= 0; i--)
	{
		if (ofn.lpstrFile[i] == '\\')
			break;
		char a = ofn.lpstrFile[i];
		s.push(a);
	}

	i = 0;
	TCHAR fileName[100] = L"";
	while (s.top() != '\0')
	{
		fileName[i] = s.top();
		s.pop();
		i++;
	}

	FILE* pFile = nullptr;
	errno_t errNum = _wfopen_s(&pFile, fileName, L"rb");

	if (pFile == nullptr || errNum != 0)
		return;

	fread(&m_gridX, sizeof(int), 1, pFile);
	fread(&m_gridY, sizeof(int), 1, pFile);

	SetBoard(m_gridX, m_gridY);

	tMenuEvent event;

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			fread(&event, sizeof(tMenuEvent), 1, pFile);
			m_pVecBoardEvent->at(i)->at(j) = event;
		}
	}

	int x, y;
	int idx;
	eType type;

	while (fread(&x, sizeof(int), 1, pFile) == 1 &&
		fread(&y, sizeof(int), 1, pFile) == 1 &&
		fread(&type, sizeof(eType), 1, pFile) == 1 && 
		fread(&idx, sizeof(int), 1, pFile) == 1)
	{
		CSprite* sprite = nullptr;

		switch (type)
		{
		case eType::Tile:
			sprite = CResourceManager::GetInst()->GetImage("Tile", idx);
			m_vecLayer->at(0).AddSprite(x, y, sprite);
			break;
		case eType::Block:
			sprite = CResourceManager::GetInst()->GetImage("Block", idx);
			if (sprite->GetSize().width < 30)
				m_vecLayer->at(2).AddSprite(x, y, sprite);
			else
				m_vecLayer->at(1).AddSprite(x, y, sprite);
			break;
		}
	}

	fclose(pFile);
}

void Board::DrawRect(ID2D1RenderTarget* _pRenderTarget, ID2D1SolidColorBrush* _brush, int x, int y, int _strokeWidth)
{
	int cameraX = Camera::GetInst()->GetXPos();
	int cameraY = Camera::GetInst()->GetYPos();
	float cameraScale = Camera::GetInst()->GetScale();

	int left = (x * BOARD_BOX_SIZE * cameraScale + PALETTE_WIDTH + cameraX) ;
	int top = (y * BOARD_BOX_SIZE * cameraScale + cameraY);
	int right = (x * BOARD_BOX_SIZE * cameraScale + BOARD_BOX_SIZE * cameraScale + PALETTE_WIDTH + cameraX);
	int bottom = (y * BOARD_BOX_SIZE * cameraScale + BOARD_BOX_SIZE * cameraScale + cameraY);

	_pRenderTarget->DrawRectangle(D2D1::RectF(left, top, right, bottom), _brush, _strokeWidth);
}
