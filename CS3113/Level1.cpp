#include "Level1.h"

Level1::Level1(Vector2 origin, int *lives)
    : Scene{ origin, "#174A2A", lives },
      mLevelData{},
      mStartPosition{}
{}

Level1::~Level1()
{
    shutdown();
}

void Level1::buildLevelData()
{
    for (int i = 0; i < LEVEL1_WIDTH * LEVEL1_HEIGHT; i++)
        mLevelData[i] = 0;

    for (int column = 0; column < LEVEL1_WIDTH; column++)
    {
        mLevelData[8 * LEVEL1_WIDTH + column] = 1;
        mLevelData[9 * LEVEL1_WIDTH + column] = 2;
    }

    for (int column = 8; column <= 9; column++)
    {
        mLevelData[8 * LEVEL1_WIDTH + column] = 0;
        mLevelData[9 * LEVEL1_WIDTH + column] = 0;
    }

    for (int column = 17; column <= 18; column++)
    {
        mLevelData[8 * LEVEL1_WIDTH + column] = 0;
        mLevelData[9 * LEVEL1_WIDTH + column] = 0;
    }

    for (int column = 7; column <= 10; column++)
        mLevelData[6 * LEVEL1_WIDTH + column] = 3;

    for (int column = 16; column <= 19; column++)
        mLevelData[5 * LEVEL1_WIDTH + column] = 4;

    for (int column = 22; column <= 25; column++)
        mLevelData[6 * LEVEL1_WIDTH + column] = 3;

}

void Level1::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    buildLevelData();

    mGameState.bgm = LoadMusicStream("assets/music/background.ogg");
    SetMusicVolume(mGameState.bgm, 0.25f);
    PlayMusicStream(mGameState.bgm);
    mGameState.jumpSound = LoadSound("assets/sounds/jump.wav");
    mGameState.hitSound = LoadSound("assets/sounds/hit.wav");

    mGameState.map = new Map(
        LEVEL1_WIDTH,
        LEVEL1_HEIGHT,
        mLevelData,
        "assets/map/stadium_tiles.png",
        TILE_SIZE,
        4,
        1,
        mOrigin
    );

    std::map<AnimationState, std::vector<int>> animationAtlas = {
        { JUMP_LEFT,  { 0, 1, 2, 3, 4 } },
        { RUN_LEFT,   { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { IDLE_LEFT,  { 16 } },
        { JUMP_RIGHT, { 24, 25, 26, 27, 28 } },
        { RUN_RIGHT,  { 32, 33, 34, 35, 36, 37, 38, 39 } },
        { IDLE_RIGHT, { 40 } }
    };

    float groundTop = mGameState.map->getTopBoundary() + 8 * TILE_SIZE;
    mStartPosition = {
        mGameState.map->getLeftBoundary() + 2.5f * TILE_SIZE,
        groundTop - PLAYER_HEIGHT / 2.0f
    };

    mGameState.player = new Entity(
        mStartPosition,
        { 64.0f, 64.0f },
        "assets/characters/player.png",
        ATLAS,
        { 6.0f, 8.0f },
        animationAtlas,
        PLAYER
    );
    mGameState.player->setColliderDimensions({ PLAYER_WIDTH, PLAYER_HEIGHT });
    mGameState.player->setAcceleration({ 0.0f, GRAVITY });
    mGameState.player->setJumpingPower(520.0f);
    mGameState.player->setSpeed(220);

    mGameState.enemyCount = LEVEL1_ENEMY_COUNT;
    mGameState.enemies = new Entity[mGameState.enemyCount];

    float enemyColumns[LEVEL1_ENEMY_COUNT] = { 12.5f, 26.0f };
    float leftPatrolColumns[LEVEL1_ENEMY_COUNT] = { 10.5f, 19.5f };
    float rightPatrolColumns[LEVEL1_ENEMY_COUNT] = { 16.0f, 27.5f };

    for (int i = 0; i < mGameState.enemyCount; i++)
    {
        Entity *enemy = &mGameState.enemies[i];
        enemy->setPosition({
            mGameState.map->getLeftBoundary() + enemyColumns[i] * TILE_SIZE,
            groundTop - PLAYER_HEIGHT / 2.0f
        });
        enemy->setScale({ 64.0f, 64.0f });
        enemy->setTexture("assets/characters/defender.png");
        enemy->setAnimationAtlas({ 6.0f, 8.0f }, animationAtlas);
        enemy->setColliderDimensions({ PLAYER_WIDTH, PLAYER_HEIGHT });
        enemy->setAcceleration({ 0.0f, GRAVITY });
        enemy->setEntityType(NPC);
        enemy->setAIType(WANDERER);
        enemy->setDirection(RIGHT);
        enemy->setSpeed(100);
        enemy->setPatrolLimits(
            mGameState.map->getLeftBoundary() +
                leftPatrolColumns[i] * TILE_SIZE,
            mGameState.map->getLeftBoundary() +
                rightPatrolColumns[i] * TILE_SIZE
        );
    }

    mGameState.goal = new Entity(
        {
            mGameState.map->getRightBoundary() - 1.5f * TILE_SIZE,
            groundTop - 32.0f
        },
        { 48.0f, 64.0f },
        "assets/map/goal.png",
        GOAL
    );
    mGameState.goal->setColliderDimensions({ 40.0f, 56.0f });
}

void Level1::resetPlayer()
{
    mGameState.player->setPosition(mStartPosition);
    mGameState.player->setVelocity({ 0.0f, 0.0f });
}

void Level1::loseLife()
{
    PlaySound(mGameState.hitSound);
    (*mLives)--;

    if (*mLives <= 0)
        mGameState.nextSceneID = LOSE;
    else
        resetPlayer();
}

void Level1::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.player->update(
        deltaTime,
        nullptr,
        mGameState.map
    );

    for (int i = 0; i < mGameState.enemyCount; i++)
    {
        mGameState.enemies[i].update(
            deltaTime,
            mGameState.player,
            mGameState.map
        );
    }

    for (int i = 0; i < mGameState.enemyCount; i++)
    {
        if (mGameState.player->isColliding(&mGameState.enemies[i]))
        {
            loseLife();
            return;
        }
    }

    if (mGameState.player->getPosition().y >
        mGameState.map->getBottomBoundary() + TILE_SIZE)
    {
        loseLife();
        return;
    }

    if (mGameState.player->isColliding(mGameState.goal))
        mGameState.nextSceneID = LEVEL_2;
}

void Level1::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.goal->render();

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].render();

    mGameState.player->render();
}

void Level1::shutdown()
{
    delete mGameState.player;
    mGameState.player = nullptr;

    delete[] mGameState.enemies;
    mGameState.enemies = nullptr;
    mGameState.enemyCount = 0;

    delete mGameState.goal;
    mGameState.goal = nullptr;

    delete mGameState.map;
    mGameState.map = nullptr;

    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};

    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};

    UnloadSound(mGameState.hitSound);
    mGameState.hitSound = {};
}
