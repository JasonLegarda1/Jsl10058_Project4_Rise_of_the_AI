#ifndef LEVEL1_H
#define LEVEL1_H

#include "Scene.h"

constexpr int LEVEL1_WIDTH = 30;
constexpr int LEVEL1_HEIGHT = 10;
constexpr int LEVEL1_ENEMY_COUNT = 2;

class Level1 : public Scene
{
private:
    unsigned int mLevelData[LEVEL1_WIDTH * LEVEL1_HEIGHT];
    Vector2 mStartPosition;

    void buildLevelData();
    void resetPlayer();
    void loseLife();

public:
    static constexpr float TILE_SIZE = 64.0f;
    static constexpr float GRAVITY = 900.0f;
    static constexpr float PLAYER_WIDTH = 32.0f;
    static constexpr float PLAYER_HEIGHT = 52.0f;

    Level1(Vector2 origin, int *lives);
    ~Level1();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL1_H
