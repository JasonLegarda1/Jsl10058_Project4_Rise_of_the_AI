/**
* Author: Jason Legarda
* Assignment: Rise of the AI
* Date due: 11/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Menu.h"
#include "CS3113/Level1.h"
#include "CS3113/Level2.h"
#include "CS3113/Level3.h"
#include "CS3113/Lose.h"
#include "CS3113/Win.h"

constexpr int SCREEN_WIDTH = 1000,
              SCREEN_HEIGHT = 600,
              FPS = 120;

constexpr Vector2 ORIGIN = {
    SCREEN_WIDTH / 2.0f,
    SCREEN_HEIGHT / 2.0f
};

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f,
      gTimeAccumulator = 0.0f;

int gLives = STARTING_LIVES;

Camera2D gCamera = {};
Texture2D gLifeTexture = {};

Scene *gCurrentScene = nullptr;
std::map<SceneID, Scene *> gScenes = {};

void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void renderLives();
void shutdown();

void switchToScene(Scene *scene)
{
    if (gCurrentScene != nullptr)
        gCurrentScene->shutdown();

    gCurrentScene = scene;
    gCurrentScene->initialise();

    GameState state = gCurrentScene->getState();

    if (state.player != nullptr)
        gCamera.target = state.player->getPosition();
    else
        gCamera.target = ORIGIN;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stadium Escape");
    InitAudioDevice();

    gLifeTexture = LoadTexture("assets/ui/soccer_ball.png");

    gScenes[MENU] = new Menu(ORIGIN, &gLives);
    gScenes[LEVEL_1] = new Level1(ORIGIN, &gLives);
    gScenes[LEVEL_2] = new Level2(ORIGIN, &gLives);
    gScenes[LEVEL_3] = new Level3(ORIGIN, &gLives);
    gScenes[LOSE] = new Lose(ORIGIN, &gLives);
    gScenes[WIN] = new Win(ORIGIN, &gLives);

    gCamera.offset = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom = 1.0f;

    switchToScene(gScenes[MENU]);

    gPreviousTicks = static_cast<float>(GetTime());
    SetTargetFPS(FPS);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
    {
        gAppStatus = TERMINATED;
        return;
    }

    GameState state = gCurrentScene->getState();

    if (state.player == nullptr)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            if (gCurrentScene == gScenes[MENU])
                switchToScene(gScenes[LEVEL_1]);
            else
                switchToScene(gScenes[MENU]);
        }

        return;
    }

    if (IsKeyPressed(KEY_ONE))
    {
        switchToScene(gScenes[LEVEL_1]);
        return;
    }
    if (IsKeyPressed(KEY_TWO))
    {
        switchToScene(gScenes[LEVEL_2]);
        return;
    }
    if (IsKeyPressed(KEY_THREE))
    {
        switchToScene(gScenes[LEVEL_3]);
        return;
    }

    state.player->resetMovement();

    if (IsKeyDown(KEY_A))
        state.player->moveLeft();
    else if (IsKeyDown(KEY_D))
        state.player->moveRight();

    if (IsKeyPressed(KEY_W) && state.player->isCollidingBottom())
    {
        state.player->jump();
        PlaySound(state.jumpSound);
    }
}

void update()
{
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        GameState state = gCurrentScene->getState();
        if (state.nextSceneID != NO_SCENE) break;

        if (state.player != nullptr)
        {
            Vector2 cameraPosition = {
                state.player->getPosition().x,
                ORIGIN.y
            };
            panCamera(&gCamera, &cameraPosition);
        }
    }

    gTimeAccumulator = deltaTime;
}

void renderLives()
{
    for (int i = 0; i < gLives; i++)
    {
        DrawTexturePro(
            gLifeTexture,
            {
                0.0f,
                0.0f,
                static_cast<float>(gLifeTexture.width),
                static_cast<float>(gLifeTexture.height)
            },
            {
                28.0f + i * 38.0f,
                28.0f,
                30.0f,
                30.0f
            },
            { 15.0f, 15.0f },
            0.0f,
            WHITE
        );
    }
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCamera);

    gCurrentScene->render();

    EndMode2D();

    if (gCurrentScene->getState().player != nullptr)
        renderLives();

    EndDrawing();
}

void shutdown()
{
    for (std::pair<const SceneID, Scene *> &entry : gScenes)
        delete entry.second;

    gScenes.clear();
    gCurrentScene = nullptr;

    UnloadTexture(gLifeTexture);
    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        SceneID nextSceneID = gCurrentScene->getState().nextSceneID;
        if (nextSceneID != NO_SCENE)
            switchToScene(gScenes[nextSceneID]);

        render();
    }

    shutdown();
    return 0;
}
