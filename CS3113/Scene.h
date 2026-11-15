#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

struct GameState
{
    Entity *player = nullptr;
    Entity *enemies = nullptr;
    int enemyCount = 0;
    Entity *goal = nullptr;
    Map *map = nullptr;

    Music bgm = {};
    Sound jumpSound = {};
    Sound hitSound = {};

    SceneID nextSceneID = NO_SCENE;
};

class Scene
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode;
    int *mLives;

public:
    Scene(Vector2 origin, const char *bgHexCode, int *lives);
    virtual ~Scene() {}

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState getState() const { return mGameState; }
};

#endif // SCENE_H
