#pragma once
#include <d3dx9.h>

const UINT SCREEN_WIDTH = 1280;
const UINT SCREEN_HEIGHT = 720;

#define PI  (3.1415f)

#define SAFE_RELEASE(p) if(p){p->Release();p=nullptr;}
#define SAFE_DELETE(p)  if(p){delete p;p=nullptr;}