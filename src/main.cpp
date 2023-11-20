#pragma once

// STL
#include <random>
#include <math.h>
// GUI

// User-Defined
#include "./common/common.hpp"
#include "./common/globaldata.hpp"
#include "flock.hpp"
#include "world.hpp"

#include "gui.hpp"
#include "imgui.h"
#include "./menu/menu.hpp"
//#include "simulation.hpp"

#define SIZE_OF_FLOCK 1

int main(int argc, char const* argv[])
{
	int FlockSize = SIZE_OF_FLOCK;


	GUI::init_GUI();

    rlImGuiSetup(true); // true/false for dark/light mode

    GlobalData& globalData = GlobalData::getInstance();
    Menu SimMenu;

    World& testWorld = World::getInstance();
    testWorld.setWorldDims(0, 0, 20 , 20);
    WorldMenu WorldMenuObj(testWorld);
    SimMenu.addModules(WorldMenuObj);

    Flock testflock(FlockSize, testWorld);
    FlockMenu FlockMenuObj(testflock);
    SimMenu.addModules(FlockMenuObj);

    while (!WindowShouldClose())
    {

        float mouseWheelDelta = GetMouseWheelMove();
        Vector2 const mousePos = GetMousePosition();
        int const mousePosX = GetMouseX();
        int const mousePosY = GetMouseY();

        // Update global stuff
        globalData.updateMouseRel(calcMouseRel(mousePos, gridCamera.zoom, gridCamera.target));
        //Vector2 mousePosGrid = getCellCoords(mouseRelPos);

        // Camera Stuff
        //if (IsKeyDown(KEY_LEFT_CONTROL))
        if (1)
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

        testWorld.handleInputs();
        testflock.update();

        // Draw
        BeginDrawing();
            ClearBackground(DARKGRAY);

            BeginMode2D(gridCamera);
                GUI::drawGird();
                DrawCircle(orgin.x, orgin.y, 5, RED);

                testflock.render();
                testWorld.render();

            EndMode2D();

            Vector2 mouseRelPos = globalData.getMouseRel();
            Vector2 mouseGridPos = calcCellCoords(mouseRelPos);
            DrawText(TextFormat("Mouse Pos: [%.2f, %.2f]", mouseRelPos.x, mouseRelPos.y), 0, 0, 10, RED);
            DrawText(TextFormat("Grid Pos: [%.2f, %.2f]", mouseGridPos.x, mouseGridPos.y), 0, 10, 10, RED);

        rlImGuiBegin();
            SimMenu.render();
        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}