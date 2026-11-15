#include "Level2.h"

Level2::Level2(Vector2 origin, int *lives)
    : Scene{ origin, "#123B27", lives },
      mLevelData{},
      mStartPosition{}
{}

Level2::~Level2()
{
    shutdown();
}

void Level2::buildLevelData()
{
    for (int i = 0; i < LEVEL2_WIDTH * LEVEL2_HEIGHT; i++)
        mLevelData[i] = 0;

    for (int column = 0; column < LEVEL2_WIDTH; column++)
    {
        mLevelData[8 * LEVEL2_WIDTH + column] = 2;
        mLevelData[9 * LEVEL2_WIDTH + column] = 1;
    }

    for (int column = 6; column <= 7; column++)
    {
        mLevelData[8 * LEVEL2_WIDTH + column] = 0;
        mLevelData[9 * LEVEL2_WIDTH + column] = 0;
    }

    for (int column = 15; column <= 17; column++)
    {
        mLevelData[8 * LEVEL2_WIDTH + column] = 0;
        mLevelData[9 * LEVEL2_WIDTH + column] = 0;
    }

    for (int column = 27; column <= 29; column++)
    {
        mLevelData[8 * LEVEL2_WIDTH + column] = 0;
        mLevelData[9 * LEVEL2_WIDTH + column] = 0;
    }

    for (int column = 5; column <= 8; column++)
        mLevelData[6 * LEVEL2_WIDTH + column] = 3;

    for (int column = 11; column <= 18; column++)
        mLevelData[5 * LEVEL2_WIDTH + column] = 4;

    for (int column = 21; column <= 24; column++)
        mLevelData[6 * LEVEL2_WIDTH + column] = 3;

    for (int column = 21; column <= 24; column++)
        mLevelData[5 * LEVEL2_WIDTH + column] = 3;

    for (int column = 26; column <= 30; column++)
        mLevelData[4 * LEVEL2_WIDTH + column] = 4;

    for (int column = 32; column <= 35; column++)
        mLevelData[6 * LEVEL2_WIDTH + column] = 3;

}

void Level2::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    buildLevelData();

    mGameState.bgm = LoadMusicStream("assets/music/background.ogg");
    SetMusicVolume(mGameState.bgm, 0.28f);
    PlayMusicStream(mGameState.bgm);
    mGameState.jumpSound = LoadSound("assets/sounds/jump.wav");
    mGameState.hitSound = LoadSound("assets/sounds/hit.wav");

    mGameState.map = new Map(
        LEVEL2_WIDTH,
        LEVEL2_HEIGHT,
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
    mGameState.player->setSpeed(225);

    mGameState.enemyCount = LEVEL2_ENEMY_COUNT;
    mGameState.enemies = new Entity[mGameState.enemyCount];

    float enemyColumns[LEVEL2_ENEMY_COUNT] = { 10.5f, 25.0f, 31.0f };
    float leftPatrolColumns[LEVEL2_ENEMY_COUNT] = { 8.5f, 18.5f, 30.5f };
    float rightPatrolColumns[LEVEL2_ENEMY_COUNT] = { 14.0f, 26.0f, 35.5f };
    AIType enemyTypes[LEVEL2_ENEMY_COUNT] = {
        WANDERER,
        FOLLOWER,
        FOLLOWER
    };

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
        enemy->setAIType(enemyTypes[i]);
        enemy->setDirection(RIGHT);
        enemy->setSpeed(enemyTypes[i] == FOLLOWER ? 180 : 110);
        enemy->setJumpingPower(600.0f);
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

void Level2::resetPlayer()
{
    mGameState.player->setPosition(mStartPosition);
    mGameState.player->setVelocity({ 0.0f, 0.0f });
}

void Level2::loseLife()
{
    PlaySound(mGameState.hitSound);
    (*mLives)--;

    if (*mLives <= 0)
        mGameState.nextSceneID = LOSE;
    else
        resetPlayer();
}

void Level2::update(float deltaTime)
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
        mGameState.nextSceneID = LEVEL_3;
}

void Level2::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.goal->render();

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].render();

    mGameState.player->render();
}

void Level2::shutdown()
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
