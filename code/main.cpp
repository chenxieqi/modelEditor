//--------------------------------------------------------------------------------
//  ƒƒCƒ“ŠÖ”
//--------------------------------------------------------------------------------
#include <Windows.h>
#include "library\ImGui\imgui_impl_dx9.h"
#include "game_system.h"
#include "timer.h"



int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd
)
{
    MSG msg;


    Timer* timer = Timer::Create();
   

    GameSystem gamesystem(hInstance);
    gamesystem.Init();

    do {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            if (timer->CanUpdateFrame())
            {
                gamesystem.Update();
                gamesystem.Draw();
            }
        }

    } while (msg.message!=WM_QUIT);

    gamesystem.Uninit();

    timer->Release();

    return (int)msg.wParam;
}