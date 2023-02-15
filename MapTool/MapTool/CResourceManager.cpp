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
}

void CResourceManager::LoadFile(ID2D1HwndRenderTarget* _pRenderTarget)
{
    std::wstring folder = L".\\resource\\";
    std::wstring searchPath = folder + L"*.*";

    int size = WideCharToMultiByte(CP_UTF8, 0, searchPath.c_str(), -1, NULL, 0, NULL, NULL);
    char* str = new char[size];
    WideCharToMultiByte(CP_UTF8, 0, searchPath.c_str(), -1, str, size, NULL, NULL);
    std::string result(str);
    delete[] str;
    std::string fileName = result;

    int clipSize;
    int width, height;

    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile((LPCWSTR)searchPath.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                int size = WideCharToMultiByte(CP_UTF8, 0, fd.cFileName, -1, NULL, 0, NULL, NULL);
                
                std::wstring fileName(fd.cFileName);
                std::wstring filePath = folder + fileName;

                FILE* pFile = nullptr;
                errno_t errNum = _wfopen_s(&pFile, filePath.c_str(), L"rb");

                if (pFile == nullptr || errNum != 0)
                    return;

                std::stack<char> s;
                int i;

                for (i = fileName.size(); i >= 0; i--)
                {
                    if (fileName[i] == '.')
                        break;
                    s.push(fileName[i]);
                }

                char* str = new char[fileName.size() - i];
                i = 0;
                while (!s.empty())
                {
                    str[i++] = s.top();
                    s.pop();
                }

                std::string name(str);

                delete[] str;

                //Type type;

                fread(&clipSize, sizeof(int), 1, pFile);
                //fread(&type, sizeof(Type), 1, pFile);

                for (int i = 0; i < clipSize; i++)
                {
                    CSprite* image = new CSprite;
                    fread(image, sizeof(CSprite), 1, pFile);
                    fread(&width, sizeof(int), 1, pFile);
                    fread(&height, sizeof(int), 1, pFile);
                    DWORD* pixel = (DWORD*)malloc(sizeof(DWORD) * width * height);

                    fread(&pixel[0], sizeof(DWORD) * width, height, pFile);

                    D2D1_BITMAP_PROPERTIES bpp;
                    bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
                    bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
                    bpp.dpiX = (FLOAT)0;
                    bpp.dpiY = (FLOAT)0;
                    ID2D1Bitmap* bitmap;
                    image->SetPixel(pixel);
                    _pRenderTarget->CreateBitmap(D2D1::SizeU(width, height), pixel, width * 4, &bpp, &bitmap);
                    image->SetBitmap(bitmap);
                    image->SetSize(D2D1::SizeF(width, height));

                    switch(image->GetType())
                    {
                    case Type::Tile:
                        m_mapImage["Tile"].push_back(image);
                        break;
                    case Type::Block:
                        m_mapImage["Block"].push_back(image);
                        break;
                    case Type::Character:
                        m_mapImage["Character"].push_back(image);
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

CSprite* CResourceManager::GetBlockTopImage(CSprite* _sprite)
{
    std::vector<CSprite*>::iterator iter = m_mapImage["Block"].begin();
    std::vector<CSprite*>::iterator end = m_mapImage["Block"].end();

    for (; iter != end; iter++)
    {
        if (_sprite->GetBitmap() == (*iter)->GetBitmap())
            return *(iter-1);
    }
}