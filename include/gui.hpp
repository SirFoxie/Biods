#pragma once

#include <string>


// User-defined
#include "./common/common.hpp"
#include "world.hpp"
//#include "simulation.hpp"

struct render_window {
    int screenWidth;
    int screenHeight;
    int framerate;
    std::string windowName;
};

const int screenWidth = 1280;
const int screenHeight = 720;
const Vector2 orgin = { 0.0, 0.0 };

Camera2D gridCamera = { 0 };
bool isPanning = false;
Vector2 panStart = { 0.0, 0.0 };

// Radial Menu Init
bool ShowRadialMenu = false;

namespace GUI {
    void init_GUI()
    {
        InitWindow(screenWidth, screenHeight, "Ant Colony Optimization");
        SetTargetFPS(60);

        gridCamera.zoom = 1.0f;
        gridCamera.target = { -(screenWidth/(gridCamera.zoom*10)) , -(screenHeight/(gridCamera.zoom * 10)) };

        rlImGuiSetup(true);
    }

    void resetCamera(Camera2D& camera)
    {
        camera = Camera2D{ 0 };
        camera.zoom = 0.5f;
    }

    void zoomCamera(float mouseWheelDelta)
    {
        
    }

    void drawGird()
    {
        // Draw the 3d grid, rotated 90 degrees and centered around 0,0 
            // just so we have something in the XY plane
        rlPushMatrix();
            rlTranslatef(0, 125 * CELL_SIZE, 0);
            rlRotatef(90, 1, 0, 0);
            DrawGrid(500, CELL_SIZE);
        rlPopMatrix();
    }

    void drawMaze()
    {
        for (int i = 0; i < screenWidth / CELL_SIZE; i++)
        {
            for (int j = 0; j < screenHeight / CELL_SIZE; j++)
            {
                DrawText(TextFormat("[%i,%i]", i, j), 10 + CELL_SIZE * i, 15 + CELL_SIZE * j, 12, LIGHTGRAY);
            }
        }
    }

    void startGui()
    {
        while (!WindowShouldClose())
        {
            
            float mouseWheelDelta = GetMouseWheelMove();
            Vector2 const mousePos = GetMousePosition();
            int const mousePosX = GetMouseX();
            int const mousePosY = GetMouseY();

            //Vector2 mouseRelPos = getMouseRel(mousePos, gridCamera.zoom, gridCamera.target);
            //Vector2 mousePosGrid = getCellCoords(mouseRelPos);

            // Camera Stuff
            //if (IsKeyDown(KEY_LEFT_CONTROL))
            if(1)
            {
                gridCamera.zoom += (mouseWheelDelta * 0.05f);

                if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
                    panStart = mousePos; // Start panning
                    isPanning = true;
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) { isPanning = false; }

            if (isPanning) {
                Vector2 panDelta = Vector2Subtract(mousePos, panStart);

                gridCamera.target.x -= panDelta.x / gridCamera.zoom;
                gridCamera.target.y -= panDelta.y / gridCamera.zoom;

                panStart = mousePos;
            }



            // Draw
            BeginDrawing();
                ClearBackground(RAYWHITE);

                BeginMode2D(gridCamera);
                GUI::drawGird();
                DrawCircle(orgin.x, orgin.y, 5, RED);

            EndMode2D();

            EndDrawing();
        }
        CloseWindow();
    }
}
