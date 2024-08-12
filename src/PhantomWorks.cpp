//
// Created by Brandon Friend on 8/10/24.
//
//
// Created by Brandon Friend on 8/7/24.
//

#include <iostream>
#include <raylib.h>
#include <rlgl.h>
#include "PhantomPch.h"
#include "imguiThemes.h"
#include "Core/EntryPoint.h"
#include "Core/Window.h"



class PhantomWorks : public Phantom::Application
{
public:
    PhantomWorks()
    {
        window1.ID = InitWindowPro(window1.Width, window1.Height, window1.Title.c_str(), window1.open = true);
        window2.ID = InitWindowPro(window2.Width, window2.Height, window2.Title.c_str(), window2.open = true);
    }

    ~PhantomWorks()
    {
    }

    Phantom::WindowSettings window1;
    Phantom::WindowSettings window2;

    void Run() override;
};

void PhantomWorks::Run()
{


    //setup window 1
    SetActiveWindowContext(window1.ID);
    SetWindowPosition(10, GetWindowPosition().y);
    SetTargetFPS(60);
    rlImGuiSetup(true);

    //you can use whatever imgui theme you like!
    //ImGui::StyleColorsDark();
    //imguiThemes::yellow();
    //imguiThemes::gray();
    imguiThemes::green();
    //imguiThemes::red();
    //imguiThemes::embraceTheDarkness();


    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.FontGlobalScale = 2;

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        //style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 0.5f;
        //style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
    }


    // Setup window 2
    SetActiveWindowContext(window2.ID);
    // load the texture for window 2, even though this is the same texture file we used in window 1, we have to reload it for context 2.
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 cubePosition = { 0.0f, 1.0f, 0.0f };
    Vector3 cubeSize = { 2.0f, 2.0f, 2.0f };

    Ray ray = { 0 };                    // Picking line ray
    RayCollision collision = { 0 };     // Ray collision hit info

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //----------------

    SetWindowPosition(820, GetWindowPosition().y);
    //--------------------------------------------------------------------------------------

    window1.position = (Vector2){ 300,300 };
    window1.velocity = (Vector2){ 1,1 };

    window2.position = (Vector2){ 500, 300 };
    window2.velocity = (Vector2){ 1,0 };

    while (window1.open || window2.open)  // run while any window is up
    {
        // Update

        float speed = 100;

        window1.position = Vector2Add(window1.position, Vector2Scale(window1.velocity, speed * GetFrameTime()));
        window2.position = Vector2Add(window2.position, Vector2Scale(window2.velocity, speed * GetFrameTime()));

        if(window1.open)
        {
            SetActiveWindowContext(window1.ID);

            // Update
            //----------------------------------------------------------------------------------
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                window1.focused = true;
                window2.focused = false;
            }
            //----------------------------------------------------------------------------------

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
            ClearBackground(RAYWHITE);


#pragma region imgui
            rlImGuiBegin();

            ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
            ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
            ImGui::PopStyleColor(2);
#pragma endregion


            ImGui::Begin("Test");

            ImGui::Text("Hello");
            ImGui::Button("Button");
            ImGui::Button("Button2");

            ImGui::End();


            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);


#pragma region imgui
            rlImGuiEnd();

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
#pragma endregion

            EndDrawing();
        }


        if(window2.open)
        {
            SetActiveWindowContext(window2.ID);
            if (IsCursorHidden()) UpdateCamera(&camera, CAMERA_FIRST_PERSON);


        // Toggle camera controls
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && window2.focused)
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !window2.focused)
            {
                window1.focused = false;
                window2.focused = true;
            }


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && window2.focused)
        {
                if (!collision.hit)
                {
                    ray = GetScreenToWorldRay(GetMousePosition(), camera);

                    // Check collision between ray and box
                    collision = GetRayCollisionBox(ray,
                                (BoundingBox){(Vector3){ cubePosition.x - cubeSize.x/2, cubePosition.y - cubeSize.y/2, cubePosition.z - cubeSize.z/2 },
                                              (Vector3){ cubePosition.x + cubeSize.x/2, cubePosition.y + cubeSize.y/2, cubePosition.z + cubeSize.z/2 }});
                }
                else collision.hit = false;

        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                if (collision.hit)
                {
                    DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
                    DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, MAROON);

                    DrawCubeWires(cubePosition, cubeSize.x + 0.2f, cubeSize.y + 0.2f, cubeSize.z + 0.2f, GREEN);
                }
                else
                {
                    DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, GRAY);
                    DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, DARKGRAY);
                }

                DrawRay(ray, MAROON);
                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawText("Try clicking on the box with your mouse!", 240, 10, 20, DARKGRAY);

            if (collision.hit) DrawText("BOX SELECTED", (window2.Width - MeasureText("BOX SELECTED", 30)) / 2, (int)(window2.Height * 0.1f), 30, GREEN);

            DrawText("Right click mouse to toggle camera controls", 10, 430, 10, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------

    }
    }


}

Phantom::Application* Phantom::CreateApplication()
{
    return new PhantomWorks();
}

