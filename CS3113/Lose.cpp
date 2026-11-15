#include "Lose.h"

Lose::Lose(Vector2 origin, int *lives)
    : Scene{ origin, "#4D0A0A", lives },
      mScreen{},
      mLoseSound{}
{}

Lose::~Lose()
{
    shutdown();
}

void Lose::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mScreen = LoadTexture("assets/ui/lose.png");
    mLoseSound = LoadSound("assets/sounds/lose.wav");
    PlaySound(mLoseSound);
}

void Lose::update(float)
{}

void Lose::render()
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

void Lose::shutdown()
{
    if (mScreen.id > 0)
        UnloadTexture(mScreen);
    mScreen = {};

    UnloadSound(mLoseSound);
    mLoseSound = {};
}
