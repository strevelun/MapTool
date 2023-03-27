#include "CResourceManager.h"
#include "CSprite.h"
#include "CBitmap.h"

#include <vector>
#include <string>
#include <fstream>
#include <Windows.h>
#include <stack>

CResourceManager* CResourceManager::m_inst = nullptr;

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
    for (auto& pair : m_mapImage)
    {
        for (auto* sprite : pair.second)
            delete sprite;
        pair.second.clear();
    }
    m_mapImage.clear();
}

void CResourceManager::LoadFiles(ID2D1HwndRenderTarget* _pRenderTarget, std::wstring folderName)
{
    int tileIdx = 0;
    int blockIdx = 0;
    int characterIdx = 0;

    int bitmapIdx = 0;

    std::wstring searchPath = folderName + L"*.*";

    int size = WideCharToMultiByte(CP_UTF8, 0, searchPath.c_str(), -1, NULL, 0, NULL, NULL);
    char* str = new char[size];
    WideCharToMultiByte(CP_UTF8, 0, searchPath.c_str(), -1, str, size, NULL, NULL);
    std::string result(str);
    delete[] str;
    std::string fileName = result;

    D2D1_BITMAP_PROPERTIES bpp;
    bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    bpp.dpiX = (FLOAT)0;
    bpp.dpiY = (FLOAT)0;

    int clipSize = 0;

    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile((LPCWSTR)searchPath.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                int size = WideCharToMultiByte(CP_UTF8, 0, fd.cFileName, -1, NULL, 0, NULL, NULL);
                
                std::wstring fileName(fd.cFileName);
                std::wstring filePath = folderName + fileName;

                FILE* pFile = nullptr;
                errno_t errNum = _wfopen_s(&pFile, filePath.c_str(), L"rb");

                if (pFile == nullptr || errNum != 0)
                    return;

                fread(&clipSize, sizeof(int), 1, pFile);
                D2D1_SIZE_F bitmapSize;
                fread(&bitmapSize, sizeof(D2D1_SIZE_F), 1, pFile);
                
                CBitmap* bitmap = new CBitmap();
                
                bitmap->SetSize(bitmapSize);
                DWORD* pixel = new DWORD[sizeof(DWORD) * bitmapSize.width * bitmapSize.height];
                fread(&pixel[0], sizeof(DWORD) * bitmapSize.width, bitmapSize.height, pFile);
                ID2D1Bitmap* d2dBitmap;
                _pRenderTarget->CreateBitmap(D2D1::SizeU(bitmapSize.width, bitmapSize.height), pixel, bitmapSize.width * 4, &bpp, &d2dBitmap);

                delete[] pixel;

                bitmap->SetBitmap(d2dBitmap);

                CSpriteData* arr = new CSpriteData[clipSize];

                fread(arr, sizeof(CSpriteData), clipSize, pFile);

                for (int i = 0; i < clipSize; i++)
                {
                    D2D1_SIZE_F size = arr[i].GetSize();
                    CSprite* sprite = new CSprite(&arr[i]);

                    switch(sprite->GetType())
                    {
                    case eType::Tile:
                        m_mapImage["Tile"].push_back(sprite);
                        sprite->SetVecIdx(tileIdx++);
                        break;
                    case eType::Block:
                        m_mapImage["Block"].push_back(sprite);
                        sprite->SetVecIdx(blockIdx++);
                        break;
                    case eType::Character:
                        m_mapImage["Character"].push_back(sprite);
                        break;
                    }
                    sprite->SetIdx(bitmapIdx);
                }
                
                CResourceManager::GetInst()->SetIdxBitmap(bitmap);
                delete[] arr;
                bitmapIdx++;
                fclose(pFile);
            }
        } while (::FindNextFile(hFind, &fd));

        ::FindClose(hFind);
    }
    else {
        //cout << "Unable to open the folder." << endl;
        return;
    }
    
}