#pragma once
#include <windows.h>

class BmpImage {
private:
    HBITMAP hBitmap;
    int width;
    int height;

public:
    BmpImage() : hBitmap(NULL), width(0), height(0) {}

    ~BmpImage() {
        if (hBitmap) {
            DeleteObject(hBitmap);
            hBitmap = NULL;
        }
    }

    // BMP ���� �ε�
    bool Load(const wchar_t* filename) {
        hBitmap = (HBITMAP)LoadImageW(
            NULL,
            filename,
            IMAGE_BITMAP,
            0, 0,
            LR_LOADFROMFILE | LR_CREATEDIBSECTION
        );
        if (!hBitmap) return false;

        // ��Ʈ�� ���� ���
        BITMAP bm;
        GetObject(hBitmap, sizeof(bm), &bm);
        width = bm.bmWidth;
        height = bm.bmHeight;
        return true;
    }

    // BMP �׸���
    void Draw(HDC hdc, int x, int y) {
        if (!hBitmap) return;

        HDC memDC = CreateCompatibleDC(hdc);
        HGDIOBJ old = SelectObject(memDC, hBitmap);

        BitBlt(hdc, x, y, width, height, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, old);
        DeleteDC(memDC);
    }

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
};