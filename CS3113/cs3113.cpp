#include "cs3113.h"

Color ColorFromHex(const char *hex)
{
    if (hex[0] == '#') hex++;

    unsigned int r = 0,
                 g = 0,
                 b = 0,
                 a = 255;

    if (sscanf(hex, "%02x%02x%02x", &r, &g, &b) == 3)
    {
        return Color{
            static_cast<unsigned char>(r),
            static_cast<unsigned char>(g),
            static_cast<unsigned char>(b),
            static_cast<unsigned char>(a)
        };
    }

    if (sscanf(hex, "%02x%02x%02x%02x", &r, &g, &b, &a) == 4)
    {
        return Color{
            static_cast<unsigned char>(r),
            static_cast<unsigned char>(g),
            static_cast<unsigned char>(b),
            static_cast<unsigned char>(a)
        };
    }

    return RAYWHITE;
}

Rectangle getUVRectangle(
    const Texture2D *texture,
    int index,
    int rows,
    int columns
)
{
    float uCoordinate = static_cast<float>(index % columns) /
        static_cast<float>(columns);
    uCoordinate *= texture->width;

    float vCoordinate = static_cast<float>(index / columns) /
        static_cast<float>(rows);
    vCoordinate *= texture->height;

    float sliceWidth = texture->width / static_cast<float>(columns);
    float sliceHeight = texture->height / static_cast<float>(rows);

    return {
        uCoordinate,
        vCoordinate,
        sliceWidth,
        sliceHeight
    };
}

void panCamera(Camera2D *camera, const Vector2 *targetPosition)
{
    Vector2 positionDifference = Vector2Subtract(
        *targetPosition,
        camera->target
    );

    camera->target = Vector2Add(
        camera->target,
        Vector2Scale(positionDifference, 0.1f)
    );
}
