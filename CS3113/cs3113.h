#ifndef CS3113_H
#define CS3113_H

#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>
#include <map>
#include <vector>

enum AppStatus   { TERMINATED, RUNNING };
enum TextureType { SINGLE, ATLAS };
enum SceneID
{
    NO_SCENE = -1,
    MENU,
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LOSE,
    WIN
};

constexpr int STARTING_LIVES = 3;

Color ColorFromHex(const char *hex);
Rectangle getUVRectangle(
    const Texture2D *texture,
    int index,
    int rows,
    int columns
);
void panCamera(Camera2D *camera, const Vector2 *targetPosition);

#endif // CS3113_H
