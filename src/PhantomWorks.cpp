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


class DocumentWindow
{
public:
    bool Open = false;

    RenderTexture ViewTexture;
    DocumentWindow() {}
    virtual void Setup() = 0;
    virtual void Shutdown() = 0;
    virtual void Show() = 0;
    virtual void Update() = 0;

    bool Focused = false;

    Rectangle ContentRect = { 0 };
};

class ExampleWindow : public DocumentWindow
{
public:
    Camera camera = { 0 };
    Vector3 cubePosition = { 0.0f, 1.0f, 0.0f };
    Vector3 cubeSize = { 2.0f, 2.0f, 2.0f };

    Ray ray = { 0 };                    // Picking line ray
    RayCollision collision = { 0 };     // Ray collision hit info
    void Setup() override
    {

        ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        // Define the camera to look into our 3d world
        camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
        camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
        camera.fovy = 45.0f;                                // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    }

    void Update() override
    {
        if (!Open)
            return;

        if (IsWindowResized())
        {
            UnloadRenderTexture(ViewTexture);
            ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        }



        // Update
        //----------------------------------------------------------------------------------
        if (IsCursorHidden()) UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // Toggle camera controls
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
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


        BeginTextureMode(ViewTexture);
        ClearBackground(SKYBLUE);


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

            if (collision.hit) DrawText("BOX SELECTED", (GetScreenWidth() - MeasureText("BOX SELECTED", 30)) / 2, (int)(GetScreenHeight() * 0.1f), 30, GREEN);

            DrawText("Right click mouse to toggle camera controls", 10, 430, 10, GRAY);

            DrawFPS(10, 10);

        //----------------------------------------------------------------------------------
    }

    void Show() override
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

        Focused = false;


        if (ImGui::Begin("Example View", &Open, ImGuiWindowFlags_NoScrollbar))
        {
            Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

            ImVec2 size = ImGui::GetContentRegionAvail();

            Rectangle viewRect = { 0 };
            viewRect.x = ViewTexture.texture.width / 2 - size.x / 2;
            viewRect.y = ViewTexture.texture.height / 2 - size.y / 2;
            viewRect.width = size.x;
            viewRect.height = -size.y;
            // draw the view
            rlImGuiImageRect(&ViewTexture.texture, (int)size.x, (int)size.y, viewRect);
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void Shutdown() override
    {
        UnloadRenderTexture(ViewTexture);
    }
};

class SceneViewWindow : public DocumentWindow
{
public:
	Camera3D Camera = { 0 };

    Texture2D GridTexture = { 0 };
	void Setup() override
	{
		ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

		Camera.fovy = 45;
		Camera.up.y = 1;
		Camera.position.y = 3;
		Camera.position.z = -25;

		Image img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
		GridTexture = LoadTextureFromImage(img);
		UnloadImage(img);
		GenTextureMipmaps(&GridTexture);
		SetTextureFilter(GridTexture, TEXTURE_FILTER_ANISOTROPIC_16X);
		SetTextureWrap(GridTexture, TEXTURE_WRAP_CLAMP);
	}

	void Shutdown() override
	{
		UnloadRenderTexture(ViewTexture);
		UnloadTexture(GridTexture);
	}

	void Show() override
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

		if (ImGui::Begin("3D View", &Open, ImGuiWindowFlags_NoScrollbar))
		{
			Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
			// draw the view
			rlImGuiImageRenderTextureFit(&ViewTexture, true);
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Update() override
	{
		if (!Open)
			return;

		if (IsWindowResized())
		{
			UnloadRenderTexture(ViewTexture);
			ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
		}

		float period = 10;
		float magnitude = 25;

		Camera.position.x = (float)(sinf((float)GetTime() / period) * magnitude);

		BeginTextureMode(ViewTexture);
		ClearBackground(SKYBLUE);

		BeginMode3D(Camera);

		// grid of cube trees on a plane to make a "world"
		DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE); // simple world plane
		float spacing = 4;
		int count = 5;

		for (float x = -count * spacing; x <= count * spacing; x += spacing)
		{
			for (float z = -count * spacing; z <= count * spacing; z += spacing)
			{
				Vector3 pos = { x, 0.5f, z };

				Vector3 min = { x - 0.5f,0,z - 0.5f };
				Vector3 max = { x + 0.5f,1,z + 0.5f };

				DrawCube(Vector3{ x, 1.5f, z }, 1, 1, 1, GREEN);
				DrawCube(Vector3{ x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
			}
		}

		EndMode3D();
		EndTextureMode();
	}

};



class PhantomWorks : public Phantom::Application
{
public:
bool CloseApplication = false;

    PhantomWorks()
    {

    }

    ~PhantomWorks()
    {
    }
    void Run() override;

    SceneViewWindow SceneView;
    ExampleWindow example;

};


void PhantomWorks::Run()
{
    int screenWidth = 1900;
    int screenHeight = 900;
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - ImGui Demo");
    SetTargetFPS(144);
    rlImGuiSetup(true);
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    example.Setup();
    example.Open = false;

    SceneView.Setup();
    SceneView.Open = true;



    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        SceneView.Update();
        example.Update();

        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();
        example.Show();
        SceneView.Show();
        rlImGuiEnd();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    rlImGuiShutdown();

    example.Shutdown();
    SceneView.Shutdown();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------


}

Phantom::Application* Phantom::CreateApplication()
{


    return new PhantomWorks();
}

