#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "renderer.h"
#include "settings.h"

int main()
{
    //Setup raylib, project and reserve space for window title
    InitWindow((int) WINDOW_WIDTH, (int)WINDOW_HEIGHT, WINDOW_TITLE);
    Setup();

    char* windowTitle = (char *) malloc(WINDOW_TITLE_MAX_LENGTH);

    //Clear reset window title and redraw screen
    while(!WindowShouldClose())
    {
        sprintf(windowTitle, "%s   FPS: %5d Frame Time: %5f", WINDOW_TITLE, GetFPS(), GetFrameTime());
        SetWindowTitle(windowTitle);

        BeginDrawing();

        Update();

        EndDrawing();
    }

    //Cleanup raylib and project and free window title
    CloseWindow();
    Exit();
    free(windowTitle);

    return 0;
}
