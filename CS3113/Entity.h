#ifndef ENTITY_H
#define ENTITY_H

#include "Map.h"

enum Direction { LEFT, RIGHT };
enum EntityType { PLAYER, NPC, GOAL, NONE };
enum AIType { WANDERER, FOLLOWER, JUMPER };
enum AnimationState
{
    IDLE_RIGHT,
    RUN_RIGHT,
    JUMP_RIGHT,
    IDLE_LEFT,
    RUN_LEFT,
    JUMP_LEFT
};

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D mTexture;
    TextureType mTextureType;
    Vector2 mSpriteSheetDimensions;

    std::map<AnimationState, std::vector<int>> mAnimationAtlas;
    std::vector<int> mAnimationIndices;
    AnimationState mAnimationState;
    Direction mDirection;

    int mFrameSpeed;
    int mCurrentFrameIndex;
    float mAnimationTime;

    bool mIsJumping;
    float mJumpingPower;

    int mSpeed;

    bool mIsCollidingBottom;
    bool mIsCollidingRight;
    bool mIsCollidingLeft;

    EntityType mEntityType;
    AIType mAIType;

    bool mHasPatrolLimits;
    float mLeftPatrolLimit;
    float mRightPatrolLimit;

    void checkCollisionY(Map *map);
    void checkCollisionX(Map *map);

    void resetColliderFlags();
    void animate(float deltaTime);
    void updateAnimationState();
    void setAnimationState(AnimationState state);

    void AIActivate(Entity *target, Map *map);
    void AIWander(Map *map);
    void AIFollow(Entity *target);
    void AIJump(Map *map);
    bool hasGroundAhead(Map *map);

public:
    static constexpr int DEFAULT_SIZE = 64;
    static constexpr int DEFAULT_SPEED = 180;
    static constexpr int DEFAULT_FRAME_SPEED = 10;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    Entity();
    Entity(
        Vector2 position,
        Vector2 scale,
        const char *textureFilepath,
        EntityType entityType
    );
    Entity(
        Vector2 position,
        Vector2 scale,
        const char *textureFilepath,
        TextureType textureType,
        Vector2 spriteSheetDimensions,
        std::map<AnimationState, std::vector<int>> animationAtlas,
        EntityType entityType
    );
    ~Entity();

    void update(float deltaTime, Entity *player, Map *map);
    void render();

    bool isColliding(Entity *other) const;

    void jump() { mIsJumping = true; }

    void moveLeft() { mMovement.x = -1.0f; mDirection = LEFT; }
    void moveRight() { mMovement.x = 1.0f; mDirection = RIGHT; }
    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    Vector2 getPosition() const { return mPosition; }
    bool isCollidingBottom() const { return mIsCollidingBottom; }

    void setPosition(Vector2 position) { mPosition = position; }
    void setVelocity(Vector2 velocity) { mVelocity = velocity; }
    void setAcceleration(Vector2 acceleration)
        { mAcceleration = acceleration; }
    void setScale(Vector2 scale) { mScale = scale; }
    void setTexture(const char *textureFilepath)
        { mTexture = LoadTexture(textureFilepath); }
    void setColliderDimensions(Vector2 dimensions)
        { mColliderDimensions = dimensions; }
    void setSpeed(int speed) { mSpeed = speed; }
    void setJumpingPower(float power) { mJumpingPower = power; }
    void setEntityType(EntityType type) { mEntityType = type; }
    void setAIType(AIType type) { mAIType = type; }
    void setDirection(Direction direction) { mDirection = direction; }
    void setPatrolLimits(float leftLimit, float rightLimit)
    {
        mHasPatrolLimits = true;
        mLeftPatrolLimit = leftLimit;
        mRightPatrolLimit = rightLimit;
    }
    void setAnimationAtlas(
        Vector2 dimensions,
        std::map<AnimationState, std::vector<int>> animationAtlas
    );
};

#endif // ENTITY_H
