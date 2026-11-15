#include "Win.h"

Win::Win(Vector2 origin, int *lives)
    : Scene{ origin, "#0B4D26", lives },
      mScreen{},
      mCompleteSound{}
{}

Win::~Win()
{
    shutdown();
}

void Win::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mScreen = LoadTexture("assets/ui/win.png");
    mCompleteSound = LoadSound("assets/sounds/level_complete.wav");
    PlaySound(mCompleteSound);
}

void Win::update(float)
{}

void Win::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawTexturePro(
        mScreen,
        { 0.0f, 0.0f,
          static_cast<float>(mScreen.width),
          static_cast<float>(mScreen.height) },
        { mOrigin.x, mOrigin.y, 1000.0f, 600.0f },
        { 500.0f, 300.0f },
        0.0f,
        WHITE
    );
}

void Win::shutdown()
{
    if (mScreen.id > 0)
        UnloadTexture(mScreen);
    mScreen = {};

    UnloadSound(mCompleteSound);
    mCompleteSound = {};
}
