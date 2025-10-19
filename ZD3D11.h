#pragma once

// ������ / D3D ��� ����

// Windows includes
#include <windows.h>
#include <tchar.h>

// Standard ANSI-C includes
#include <time.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <random>

// DirectX includes
#include <d3d11.h>
#include <DirectXMath.h>
#include "dxerr.h"
#include "ChiliException.h"

//------------------------------------------------------------------------------

#define randomize() srand((unsigned)time(nullptr))
#define SAFE_DELETE(p)            { if(p) { delete (p);        (p)=nullptr; }}
#define SAFE_DELETE_ARRAY(p)    { if(p) { delete[] (p);        (p)=nullptr; }}
#define SAFE_RELEASE(p)            { if(p) { (p)->Release();    (p)=nullptr; }}

inline int RectWidth(RECT& rc) { return ((rc).right - (rc).left); }
inline int RectHeight(RECT& rc) { return ((rc).bottom - (rc).top); }

//------------------------------------------------------------------------------
// Graphics
// D3D Ȱ�� �⺻ �׷��� Ŭ������
#include "ZGraphics.h"
#include "ZGraphicsResource.h"

//------------------------------------------------------------------------------
// Framework
// ���� ������(ZApplication) ���� �� ���� ������ ��ü ����
#include "ZApplication.h"