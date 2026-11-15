#include "Menu.h"

Menu::Menu(Vector2 origin, int *lives)
    : Scene{ origin, "#0B3D21", lives }, mScreen{}
{}

Menu::~Menu()
{
    shutdown();
}

void Menu::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    *mLives = STARTING_LIVES;
    mScreen = LoadTexture("assets/ui/menu.png");
}

void Menu::update(float)
{}

void Menu::render()
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

void Menu::shutdown()
{
    if (mScreen.id > 0)
        UnloadTexture(mScreen);

    mScreen = {};
}
