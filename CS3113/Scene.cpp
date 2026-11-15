#include "Scene.h"

Scene::Scene(Vector2 origin, const char *bgHexCode, int *lives)
    : mGameState{},
      mOrigin{ origin },
      mBGColourHexCode{ bgHexCode },
      mLives{ lives }
{}
