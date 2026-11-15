#ifndef LEVEL3_H
#define LEVEL3_H

#include "Scene.h"

constexpr int LEVEL3_WIDTH = 46;
constexpr int LEVEL3_HEIGHT = 10;
constexpr int LEVEL3_ENEMY_COUNT = 4;

class Level3 : public Scene
{
private:
    unsigned int mLevelData[LEVEL3_WIDTH * LEVEL3_HEIGHT];
    Vector2 mStartPosition;

    void buildLevelData();
    void resetPlayer();
    void loseLife();

public:
    static constexpr float TILE_SIZE = 64.0f;
    static constexpr float GRAVITY = 900.0f;
    static constexpr float PLAYER_WIDTH = 32.0f;
    static constexpr float PLAYER_HEIGHT = 52.0f;

    Level3(Vector2 origin, int *lives);
    ~Level3();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVEL3_H
