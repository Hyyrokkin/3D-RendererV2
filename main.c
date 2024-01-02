#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "renderer.h"
#include "settings.h"

int main()
{
    InitWindow((int) WINDOW_WIDTH, (int)WINDOW_HEIGHT, WINDOW_TITLE);
    Setup();

    char* windowTitle = (char *) malloc(WINDOW_TITLE_MAX_LENGTH);

    while(!WindowShouldClose())
    {
        sprintf(windowTitle, "%s   FPS: %5d Frame Time: %5f", WINDOW_TITLE, GetFPS(), GetFrameTime());
        SetWindowTitle(windowTitle);

        BeginDrawing();

        Update();

        EndDrawing();
    }

    CloseWindow();
    Exit();
    free(windowTitle);

    return 0;
}
