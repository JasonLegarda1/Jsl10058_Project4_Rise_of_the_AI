#ifndef MENU_H
#define MENU_H

#include "Scene.h"

class Menu : public Scene
{
private:
    Texture2D mScreen;

public:
    Menu(Vector2 origin, int *lives);
    ~Menu();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // MENU_H
