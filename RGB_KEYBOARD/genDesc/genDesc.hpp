#ifndef _MASTER_IMGUI_
#define _MASTER_IMGUI_

#include <dinput.h>
#include <tchar.h>
#include <d3d11.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <shlwapi.h>
#include <iomanip>
#include <sstream>

#include <SDL2/SDL.h>
#include <Windows.h>
#include <imgui.h>
#include <wchar.h>

#include <iostream>
#include <string>

#include "resource.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#pragma comment (lib,"shlwapi.lib")
#pragma comment (lib, "D3D11.lib")

using namespace std;

void main_window_init (void);
void main_window_client(int w, int h);

#endif // !_MASTER_IMGUI_

