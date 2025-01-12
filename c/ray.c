#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define WIDTH 1000
#define HEIGHT 1000

typedef struct point { int x; int y; } point;

point xy2screen(point p) {
    /* xy coords are centered on the screen */
    return (point){p.x + WIDTH/2, p.y + HEIGHT/2};
}

void draw_rect(point center, int size, Color color) {
    point screen = xy2screen(center);
    DrawRectangle(screen.x - size/2, screen.y - size/2, size, size, color);
    DrawRectangleLines(screen.x - size/2, screen.y - size/2, size, size, DARKGRAY);
}

void draw_figure(point center, int size, Color color) {
    if (size < 2) {
        return; 
    }
    draw_figure((point){center.x+size/2, center.y+size/2}, size/2, color);
    draw_figure((point){center.x+size/2, center.y-size/2}, size/2, color);
    draw_figure((point){center.x-size/2, center.y+size/2}, size/2, color);
    draw_figure((point){center.x-size/2, center.y-size/2}, size/2, color);
    draw_rect(center, size, color);
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Canvas");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_figure((point){0, 0}, 500, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
