#ifndef LOSE_H
#define LOSE_H

#include "Scene.h"

class Lose : public Scene
{
private:
    Texture2D mScreen;
    Sound mLoseSound;

public:
    Lose(Vector2 origin, int *lives);
    ~Lose();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LOSE_H
