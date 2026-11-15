#ifndef WIN_H
#define WIN_H

#include "Scene.h"

class Win : public Scene
{
private:
    Texture2D mScreen;
    Sound mCompleteSound;

public:
    Win(Vector2 origin, int *lives);
    ~Win();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // WIN_H
