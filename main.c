#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#define STB_DS_IMPLEMENTATION
#include "libs/stb/stb_ds.h"

#include "settings.h"
#include "renderer.h"

int main()
{
    //Setup raylib, project and reserve space for window title
    InitWindow((int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, WINDOW_TITLE);
    Setup();

    char* windowTitle = (char *) malloc(WINDOW_TITLE_MAX_LENGTH);

    //reset window title and redraw screen
    while(!WindowShouldClose())
    {
        sprintf_s(windowTitle, WINDOW_TITLE_MAX_LENGTH,"%s   FPS: %5d Frame Time: %5f", WINDOW_TITLE, GetFPS(), GetFrameTime());
        SetWindowTitle(windowTitle);

        BeginDrawing();

        Update();

        EndDrawing();
    }

    //Cleanup
    free(windowTitle);
    CloseWindow();
    Exit();
    return 0;
}
