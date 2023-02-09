#include "CFileManager.h"
#include "CImage.h"

#include <vector>
#include <string>
#include <fstream>
#include <Windows.h>

CFileManager* CFileManager::m_inst = nullptr;

CFileManager::CFileManager()
{
}

CFileManager::~CFileManager()
{
}

void CFileManager::LoadFile(ID2D1HwndRenderTarget* _pRenderTarget)
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

                fread(&clipSize, sizeof(int), 1, pFile);

                for (int i = 0; i < clipSize; i++)
                {
                    CImage* image = new CImage;
                    fread(image, sizeof(CImage), 1, pFile);
                    fread(&width, sizeof(int), 1, pFile);
                    fread(&height, sizeof(int), 1, pFile);
                    DWORD* pixel = (DWORD*)malloc(sizeof(DWORD) * width * height);

                    fread(pixel, sizeof(DWORD) * width, height, pFile);

                    D2D1_BITMAP_PROPERTIES bpp;
                    bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
                    bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
                    bpp.dpiX = (FLOAT)0;
                    bpp.dpiY = (FLOAT)0;
                    ID2D1Bitmap* bitmap;
                    image->SetPixel(pixel);
                    _pRenderTarget->CreateBitmap(D2D1::SizeU(width, height), pixel, width * 4, &bpp, &bitmap);
                    image->SetBitmap(bitmap);

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
