#include "CResourceManager.h"
#include "CSprite.h"

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
        {
            delete sprite;
        }
    }
}

// 폴더 이름을 매개변수로 넘겨주면 그 폴더 안에 있는 파일을 로드해준다.
void CResourceManager::LoadFiles(ID2D1HwndRenderTarget* _pRenderTarget, std::wstring folderName)
{
    int tileIdx = 0;
    int blockIdx = 0;
    int characterIdx = 0;

    std::wstring searchPath = folderName + L"*.*";

    int size = WideCharToMultiByte(CP_UTF8, 0, searchPath.c_str(), -1, NULL, 0, NULL, NULL);
    char* str = new char[size];
    WideCharToMultiByte(CP_UTF8, 0, searchPath.c_str(), -1, str, size, NULL, NULL);
    std::string result(str);
    delete[] str;
    std::string fileName = result;

    int clipSize;

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

                D2D1_BITMAP_PROPERTIES bpp;
                bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
                bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
                bpp.dpiX = (FLOAT)0;
                bpp.dpiY = (FLOAT)0;

                fread(&clipSize, sizeof(int), 1, pFile);

                CSprite* arr = new CSprite[clipSize];
                fread(arr, sizeof(CSprite), clipSize, pFile);

                for (int i = 0; i < clipSize; i++)
                {
                    D2D1_SIZE_F size = arr[i].GetSize();
                    DWORD* pixel = new DWORD[sizeof(DWORD) * size.width * size.height]; 
                    fread(&pixel[0], sizeof(DWORD) * size.width , size.height, pFile);
                    
                    ID2D1Bitmap* bitmap;
                    //arr[i].SetPixel(pixel);
                    _pRenderTarget->CreateBitmap(D2D1::SizeU(size.width, size.height), pixel, size.width * 4, &bpp, &bitmap);

                    delete[] pixel;

                     arr[i].SetBitmap(bitmap);
                     arr[i].SetSize(D2D1::SizeF(size.width, size.height));

                    switch(arr[i].GetType())
                    {
                    case tType::Tile:
                        arr[i].SetIdx(tileIdx++);
                        m_mapImage["Tile"].push_back(&arr[i]);
                        break;
                    case tType::Block:
                        arr[i].SetIdx(blockIdx++);
                        m_mapImage["Block"].push_back(&arr[i]);
                        break;
                    case tType::Character:
                        arr[i].SetIdx(characterIdx++);
                        m_mapImage["Character"].push_back(&arr[i]);
                        break;
                    }
                }

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