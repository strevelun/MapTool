#include "Board.h"
#include "Settings.h"
#include "CMouse.h"
#include "CResourceManager.h"

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
				/*
				_pRenderTarget->DrawBitmap(CResourceManager::GetInst()->GetBlockTopImage(sprite)->GetBitmap(),
					D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH,
						(i - 1) * BOARD_BOX_SIZE,
						j * BOARD_BOX_SIZE + BOARD_BOX_SIZE + PALETTE_WIDTH,
						(i - 1) * BOARD_BOX_SIZE + BOARD_BOX_SIZE));
						*/
			}

			sprite = &m_pVecBoardCharacter->at(i)->at(j);
			if (sprite->GetBitmap() == nullptr)
				continue;

			_pRenderTarget->DrawBitmap(sprite->GetBitmap(),
				D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE - (sprite->GetHeight() - BOARD_BOX_SIZE),
					j * BOARD_BOX_SIZE + sprite->GetWidth() + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE + BOARD_BOX_SIZE));
		}
	}

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			MenuEvent menuEvent = m_pVecBoardEvent->at(i)->at(j);
			switch (menuEvent)
			{
			case MenuEvent::Blocked:
			{
				ID2D1SolidColorBrush* brush = nullptr;
				_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush);
				brush->SetOpacity(0.8);
				_pRenderTarget->DrawRectangle(D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE,
					j * BOARD_BOX_SIZE + BOARD_BOX_SIZE + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE + BOARD_BOX_SIZE), brush, 5);
				brush->Release();
				break;
			}
			case MenuEvent::Spawn_Character:
			case MenuEvent::Spawn_Monster:
			{
				ID2D1SolidColorBrush* brush = nullptr;
				CSprite* sprite = nullptr;
				if (menuEvent == MenuEvent::Spawn_Character)
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
					_pRenderTarget->DrawBitmap(sprite->GetBitmap(), D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH,
						i * BOARD_BOX_SIZE - (sprite->GetHeight() - BOARD_BOX_SIZE),
						j * BOARD_BOX_SIZE + sprite->GetWidth() + PALETTE_WIDTH,
						i * BOARD_BOX_SIZE + BOARD_BOX_SIZE));

				brush->SetOpacity(0.8);
				_pRenderTarget->DrawRectangle(D2D1::RectF(j * BOARD_BOX_SIZE + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE,
					j * BOARD_BOX_SIZE + BOARD_BOX_SIZE + PALETTE_WIDTH,
					i * BOARD_BOX_SIZE + BOARD_BOX_SIZE), brush, 5);
				brush->Release();
				break;
			}
			}
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

void Board::RemoveEvent(int _xpos, int _ypos, MenuEvent _event)
{
	if (_xpos < PALETTE_WIDTH) return;
	if (_xpos >= (m_gridX * BOARD_BOX_SIZE) + PALETTE_WIDTH) return;
	if (_ypos >= (m_gridY * BOARD_BOX_SIZE)) return;

	int x = (_xpos - PALETTE_WIDTH) / BOARD_BOX_SIZE;
	int y = _ypos / BOARD_BOX_SIZE;

	m_pVecBoardEvent->at(y)->at(x) = MenuEvent::Default;
}

void Board::PutEvent(int _xpos, int _ypos, MenuEvent _event)
{
	if (_xpos < PALETTE_WIDTH) return;
	if (_xpos >= (m_gridX * BOARD_BOX_SIZE) + PALETTE_WIDTH) return;
	if (_ypos >= (m_gridY * BOARD_BOX_SIZE)) return;

	int x = (_xpos - PALETTE_WIDTH) / BOARD_BOX_SIZE;
	int y = _ypos / BOARD_BOX_SIZE;

	if (_event == MenuEvent::Spawn_Character)
		for (int i = 0; i < m_gridY; i++)
			for (int j = 0; j < m_gridX; j++)
				if (m_pVecBoardEvent->at(i)->at(j) == MenuEvent::Spawn_Character)
					m_pVecBoardEvent->at(i)->at(j) = MenuEvent::Default;
	m_pVecBoardEvent->at(y)->at(x) = _event;
}

void Board::SetBoard(int _gridX, int _gridY)
{
	DestroyBoard();

	m_gridX = _gridX;
	m_gridY = _gridY;

	m_pVecBoardTile = new std::vector<std::vector<CSprite>*>(_gridY);
	m_pVecBoardObject = new std::vector<std::vector<CSprite>*>(_gridY);
	m_pVecBoardCharacter = new std::vector<std::vector<CSprite>*>(_gridY);
	m_pVecBoardEvent = new std::vector<std::vector<MenuEvent>*>(_gridY);
	for (int i = 0; i < _gridY; i++)
	{
		(*m_pVecBoardTile)[i] = new std::vector<CSprite>(_gridX);
		(*m_pVecBoardObject)[i] = new std::vector<CSprite>(_gridX);
		(*m_pVecBoardCharacter)[i] = new std::vector<CSprite>(_gridX);
		(*m_pVecBoardEvent)[i] = new std::vector<MenuEvent>(_gridX);
	}
}

void Board::DestroyBoard()
{
	m_gridX = 0;
	m_gridY = 0;

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

	if (m_pVecBoardEvent != nullptr)
	{
		for (int i = 0; i < m_pVecBoardEvent->size(); i++)
			delete (*m_pVecBoardEvent)[i];
		delete m_pVecBoardEvent;
		m_pVecBoardEvent = nullptr;
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

	CSprite sprite;

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			sprite = m_pVecBoardTile->at(i)->at(j);
			fwrite(&sprite, sizeof(CSprite), 1, pFile);
			fwrite(&sprite.GetPixel()[0], sizeof(DWORD) * sprite.GetWidth(), sprite.GetHeight(), pFile);
		}
	}

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			sprite = m_pVecBoardObject->at(i)->at(j);
			fwrite(&sprite, sizeof(CSprite), 1, pFile);
			fwrite(&sprite.GetPixel()[0], sizeof(DWORD) * sprite.GetWidth(), sprite.GetHeight(), pFile);
		}
	}

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			sprite = m_pVecBoardCharacter->at(i)->at(j);
			fwrite(&sprite, sizeof(CSprite), 1, pFile);
			fwrite(&sprite.GetPixel()[0], sizeof(DWORD) * sprite.GetWidth(), sprite.GetHeight(), pFile);
		}
	}

	MenuEvent event;

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			event = m_pVecBoardEvent->at(i)->at(j);
			fwrite(&event, sizeof(MenuEvent), 1, pFile);
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
	
	CSprite sprite;

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			fread(&sprite, sizeof(CSprite), 1, pFile);
			DWORD* pixel = (DWORD*)malloc(sizeof(DWORD) * sprite.GetWidth()  * sprite.GetHeight());
			fread(pixel, sizeof(DWORD) * sprite.GetWidth(), sprite.GetHeight(), pFile);

			sprite.CreateAndSetBitmap(_pRenderTarget, pixel);
			m_pVecBoardTile->at(i)->at(j) = sprite;
		}
	}

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			fread(&sprite, sizeof(CSprite), 1, pFile);
			DWORD* pixel = (DWORD*)malloc(sizeof(DWORD) * sprite.GetWidth() * sprite.GetHeight());
			fread(pixel, sizeof(DWORD) * sprite.GetWidth(), sprite.GetHeight(), pFile);

			sprite.CreateAndSetBitmap(_pRenderTarget, pixel);
			m_pVecBoardObject->at(i)->at(j) = sprite;
		}
	}

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			fread(&sprite, sizeof(CSprite), 1, pFile);
			DWORD* pixel = (DWORD*)malloc(sizeof(DWORD) * sprite.GetWidth() * sprite.GetHeight());
			fread(pixel, sizeof(DWORD) * sprite.GetWidth(), sprite.GetHeight(), pFile);

			sprite.CreateAndSetBitmap(_pRenderTarget, pixel);
			m_pVecBoardCharacter->at(i)->at(j) = sprite;
		}
	}

	MenuEvent event;

	for (int i = 0; i < m_gridY; i++)
	{
		for (int j = 0; j < m_gridX; j++)
		{
			fread(&event, sizeof(MenuEvent), 1, pFile);
			m_pVecBoardEvent->at(i)->at(j) = event;
		}
	}

	fclose(pFile);
}
