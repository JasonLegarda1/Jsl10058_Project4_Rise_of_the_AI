#ifndef LEVEL2_H
#define LEVEL2_H

#include "Scene.h"

constexpr int LEVEL2_WIDTH = 38;
constexpr int LEVEL2_HEIGHT = 10;
constexpr int LEVEL2_ENEMY_COUNT = 3;

class Level2 : public Scene
{
private:
    unsigned int mLevelData[LEVEL2_WIDTH * LEVEL2_HEIGHT];
    Vector2 mStartPosition;

    void buildLevelData();
    void resetPlayer();
    void loseLife();

public:
    static constexpr float TILE_SIZE = 64.0f;
    static constexpr float GRAVITY = 900.0f;
    static constexpr float PLAYER_WIDTH = 32.0f;
    static constexpr float PLAYER_HEIGHT = 52.0f;

    Level2(Vector2 origin, int *lives);
    ~Level2();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL2_H
