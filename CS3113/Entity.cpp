#include "Entity.h"

Entity::Entity()
    : mPosition{ 0.0f, 0.0f },
      mMovement{ 0.0f, 0.0f },
      mVelocity{ 0.0f, 0.0f },
      mAcceleration{ 0.0f, 0.0f },
      mScale{ DEFAULT_SIZE, DEFAULT_SIZE },
      mColliderDimensions{ DEFAULT_SIZE, DEFAULT_SIZE },
      mTexture{},
      mTextureType{ SINGLE },
      mSpriteSheetDimensions{},
      mAnimationAtlas{},
      mAnimationIndices{},
      mAnimationState{ IDLE_RIGHT },
      mDirection{ RIGHT },
      mFrameSpeed{ DEFAULT_FRAME_SPEED },
      mCurrentFrameIndex{ 0 },
      mAnimationTime{ 0.0f },
      mIsJumping{ false },
      mJumpingPower{ 0.0f },
      mSpeed{ DEFAULT_SPEED },
      mIsCollidingBottom{ false },
      mIsCollidingRight{ false },
      mIsCollidingLeft{ false },
      mEntityType{ NONE },
      mAIType{ WANDERER },
      mHasPatrolLimits{ false },
      mLeftPatrolLimit{ 0.0f },
      mRightPatrolLimit{ 0.0f }
{}

Entity::Entity(
    Vector2 position,
    Vector2 scale,
    const char *textureFilepath,
    EntityType entityType
)
    : Entity()
{
    mPosition = position;
    mScale = scale;
    mColliderDimensions = scale;
    mTexture = LoadTexture(textureFilepath);
    mEntityType = entityType;
}

Entity::Entity(
    Vector2 position,
    Vector2 scale,
    const char *textureFilepath,
    TextureType textureType,
    Vector2 spriteSheetDimensions,
    std::map<AnimationState, std::vector<int>> animationAtlas,
    EntityType entityType
)
    : Entity()
{
    mPosition = position;
    mScale = scale;
    mColliderDimensions = scale;
    mTexture = LoadTexture(textureFilepath);
    mTextureType = textureType;
    mSpriteSheetDimensions = spriteSheetDimensions;
    mAnimationAtlas = animationAtlas;
    mAnimationIndices = animationAtlas.at(IDLE_RIGHT);
    mEntityType = entityType;
}

Entity::~Entity()
{
    UnloadTexture(mTexture);
}

void Entity::resetColliderFlags()
{
    mIsCollidingBottom = false;
    mIsCollidingRight = false;
    mIsCollidingLeft = false;
}

bool Entity::isColliding(Entity *other) const
{
    if (other == nullptr || other == this)
        return false;

    float xDistance = fabs(mPosition.x - other->mPosition.x) -
        ((mColliderDimensions.x + other->mColliderDimensions.x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->mPosition.y) -
        ((mColliderDimensions.y + other->mColliderDimensions.y) / 2.0f);

    return xDistance < 0.0f && yDistance < 0.0f;
}

void Entity::checkCollisionY(Map *map)
{
    if (map == nullptr) return;

    Vector2 topCentre = {
        mPosition.x,
        mPosition.y - mColliderDimensions.y / 2.0f
    };
    Vector2 topLeft = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f
    };
    Vector2 topRight = {
        mPosition.x + mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f
    };

    Vector2 bottomCentre = {
        mPosition.x,
        mPosition.y + mColliderDimensions.y / 2.0f
    };
    Vector2 bottomLeft = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y + mColliderDimensions.y / 2.0f
    };
    Vector2 bottomRight = {
        mPosition.x + mColliderDimensions.x / 2.0f,
        mPosition.y + mColliderDimensions.y / 2.0f
    };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    if ((map->isSolidTileAt(topCentre, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(topLeft, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(topRight, &xOverlap, &yOverlap)) &&
        mVelocity.y < 0.0f)
    {
        mPosition.y += yOverlap;
        mVelocity.y = 0.0f;
    }

    if ((map->isSolidTileAt(bottomCentre, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(bottomLeft, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(bottomRight, &xOverlap, &yOverlap)) &&
        mVelocity.y > 0.0f)
    {
        mPosition.y -= yOverlap;
        mVelocity.y = 0.0f;
        mIsCollidingBottom = true;
    }
}

void Entity::checkCollisionX(Map *map)
{
    if (map == nullptr) return;

    Vector2 leftCentre = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y
    };
    Vector2 leftTop = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f
    };
    Vector2 leftBottom = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y + mColliderDimensions.y / 2.0f
    };

    Vector2 rightCentre = {
        mPosition.x + mColliderDimensions.x / 2.0f,
        mPosition.y
    };
    Vector2 rightTop = {
        mPosition.x + mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f
    };
    Vector2 rightBottom = {
        mPosition.x + mColliderDimensions.x / 2.0f,
        mPosition.y + mColliderDimensions.y / 2.0f
    };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    if ((map->isSolidTileAt(rightCentre, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(rightTop, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(rightBottom, &xOverlap, &yOverlap)) &&
        mVelocity.x > 0.0f && yOverlap >= Y_COLLISION_THRESHOLD)
    {
        mPosition.x -= xOverlap * 1.01f;
        mVelocity.x = 0.0f;
        mIsCollidingRight = true;
    }

    if ((map->isSolidTileAt(leftCentre, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(leftTop, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(leftBottom, &xOverlap, &yOverlap)) &&
        mVelocity.x < 0.0f && yOverlap >= Y_COLLISION_THRESHOLD)
    {
        mPosition.x += xOverlap * 1.01f;
        mVelocity.x = 0.0f;
        mIsCollidingLeft = true;
    }
}

void Entity::setAnimationAtlas(
    Vector2 dimensions,
    std::map<AnimationState, std::vector<int>> animationAtlas
)
{
    mTextureType = ATLAS;
    mSpriteSheetDimensions = dimensions;
    mAnimationAtlas = animationAtlas;
    mAnimationState = IDLE_RIGHT;
    mAnimationIndices = animationAtlas.at(IDLE_RIGHT);
    mCurrentFrameIndex = 0;
    mAnimationTime = 0.0f;
}

void Entity::setAnimationState(AnimationState state)
{
    if (mAnimationState == state) return;

    mAnimationState = state;
    mAnimationIndices = mAnimationAtlas.at(state);
    mCurrentFrameIndex = 0;
    mAnimationTime = 0.0f;
}

void Entity::updateAnimationState()
{
    bool facingLeft = mDirection == LEFT;

    if (!mIsCollidingBottom)
        setAnimationState(facingLeft ? JUMP_LEFT : JUMP_RIGHT);
    else if (mMovement.x != 0.0f)
        setAnimationState(facingLeft ? RUN_LEFT : RUN_RIGHT);
    else
        setAnimationState(facingLeft ? IDLE_LEFT : IDLE_RIGHT);
}

void Entity::animate(float deltaTime)
{
    mAnimationTime += deltaTime;
    float secondsPerFrame = 1.0f / mFrameSpeed;

    if (mAnimationTime >= secondsPerFrame)
    {
        mAnimationTime = 0.0f;
        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
    }
}

bool Entity::hasGroundAhead(Map *map)
{
    if (map == nullptr) return true;

    float direction = mDirection == LEFT ? -1.0f : 1.0f;
    Vector2 groundProbe = {
        mPosition.x + direction * (mColliderDimensions.x / 2.0f + 4.0f),
        mPosition.y + mColliderDimensions.y / 2.0f + 4.0f
    };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    return map->isSolidTileAt(groundProbe, &xOverlap, &yOverlap);
}

void Entity::AIWander(Map *map)
{
    if (mHasPatrolLimits && mPosition.x <= mLeftPatrolLimit)
        mDirection = RIGHT;
    else if (mHasPatrolLimits && mPosition.x >= mRightPatrolLimit)
        mDirection = LEFT;
    else if (mIsCollidingLeft)
        mDirection = RIGHT;
    else if (mIsCollidingRight)
        mDirection = LEFT;
    else if (mIsCollidingBottom && !hasGroundAhead(map))
        mDirection = mDirection == LEFT ? RIGHT : LEFT;

    if (mDirection == LEFT)
        moveLeft();
    else
        moveRight();
}

void Entity::AIFollow(Entity *target)
{
    if (target == nullptr) return;

    if (mPosition.x > target->getPosition().x &&
        (!mHasPatrolLimits || mPosition.x > mLeftPatrolLimit))
        moveLeft();
    else if (mPosition.x < target->getPosition().x &&
        (!mHasPatrolLimits || mPosition.x < mRightPatrolLimit))
        moveRight();
    else
        resetMovement();
}

void Entity::AIJump(Map *map)
{
    AIWander(map);

    if (mIsCollidingBottom)
        jump();
}

void Entity::AIActivate(Entity *target, Map *map)
{
    switch (mAIType)
    {
    case WANDERER:
        AIWander(map);
        break;
    case FOLLOWER:
        AIFollow(target);
        break;
    case JUMPER:
        AIJump(map);
        break;
    }
}

void Entity::update(float deltaTime, Entity *player, Map *map)
{
    if (mEntityType == NPC)
        AIActivate(player, map);

    resetColliderFlags();

    mVelocity.x = mMovement.x * mSpeed;
    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    if (mIsJumping)
    {
        mIsJumping = false;
        mVelocity.y -= mJumpingPower;
    }

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(map);

    if (mTextureType == ATLAS)
    {
        updateAnimationState();
        animate(deltaTime);
    }
}

void Entity::render()
{
    Rectangle textureArea;

    if (mTextureType == ATLAS)
    {
        textureArea = getUVRectangle(
            &mTexture,
            mAnimationIndices[mCurrentFrameIndex],
            mSpriteSheetDimensions.x,
            mSpriteSheetDimensions.y
        );
    }
    else
    {
        textureArea = {
            0.0f,
            0.0f,
            static_cast<float>(mTexture.width),
            static_cast<float>(mTexture.height)
        };
    }

    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        mScale.x,
        mScale.y
    };

    Vector2 originOffset = {
        mScale.x / 2.0f,
        mScale.y / 2.0f
    };

    DrawTexturePro(
        mTexture,
        textureArea,
        destinationArea,
        originOffset,
        0.0f,
        WHITE
    );
}
