#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

const int       HIT_BY_SHIP = 0;
const int       HIT_BY_PROJECTILE = 1;

const int       START_DIRECTION = 0;
const int       DEPTH = 0;

const int       PLAYER_START_Y = 128;
const int       PLAYER_SIZE = 1.0;
const int       PLAYER_HIT_POINTS = 50;
const int       PLAYER_SPEED = 6;
const double    PLAYER_FULL_CABBAGE = 30.0;

const int       CABBAGE_DAMAGE = 2;
const int       CABBAGE_DELTA_X = 8;
const int       ROTATE = 20;
const double    PROJECTILE_SIZE = 0.5;

const double    EXPLOSION_SIZE = 1.0;

const int       ALIEN_SIZE = 1.5;

const double    ALIEN_DAMAGE = 5;
const double    ALIEN_SPEED = 2.0;
const int       ALIEN_SCORE_VALUE = 250;

const double    SNAGGLEGON_DAMAGE = 15;
const double    SNAGGLEGON_SPEED = 1.75;
const int       SNAGGLEGON_SCORE_VALUE = 1000;

const double    TURNIP_DAMAGE = 2;
const double    TURNIP_DELTA_X = 6;

const double    TORPEDO_DAMAGE = 8.0;
const double    TORPEDO_DELTA_X = 8.0;
const int       ALIEN_TORPEDO_DIRECTION = 180;

const double    GOODIE_SIZE = 0.5;
const int       GOODIE_POINTS = 100;
const double    GOODIE_SPEED = 0.75;

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* w, double startX, double startY, int imageID,
          int imageDir, double size, int depth);
    
    // Action to perform for each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    bool isDead() const;
    
    // Can this actor collide with a player-fired projectile?
    // (Note:  We originally called this isAlien, but it's not so much
    // _what_ it is that's important, but what you can _do_ to it.)
    virtual bool collidableWithPlayerFiredProjectile() const;
    
    // Get this actor's world
    StudentWorld* world() const;
    
    // Mark this actor as dead.
    void setDead();
    
    // Move this actor to x,y if onscreen; otherwise, don't move and mark
    // this actor as dead.
    virtual void moveTo(double x, double y);
    
private:
    StudentWorld* m_world;
    bool m_dead;
};

class Star : public Actor
{
public:
    Star(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Explosion : public Actor
{
public:
    Explosion(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    
private:
    int m_duration;
};

class DamageableObject : public Actor
{
public:
    DamageableObject(StudentWorld* w, double startX, double startY, int imageID, int startDir, double size, int depth, double hitPoints);
    
    // How many hit points does this actor have left?
    double hitPoints() const;
    
    // Increase this actor's hit points by amt.
    virtual void increaseHitPoints(double amt);
    
    // This actor suffers an amount of damage caused by being hit by either
    // a ship or a projectile (see constants above).
    virtual void sufferDamage(double amt, int cause);
    
private:
    double m_hitPoints;
};

class Player : public DamageableObject
{
public:
    Player(StudentWorld* w);
    virtual void doSomething();
    virtual void increaseHitPoints(double amt);
    virtual void sufferDamage(double amt, int cause);
    virtual void moveTo(double x, double y);
    
    // Incease the number of torpedoes the player has by amt.
    void increaseTorpedoes(int amt);
    
    // Get the player's health percentage.
    int healthPct() const;
    
    // Get the player's cabbage energy percentage.
    int cabbagePct() const;
    
    // Get the number of torpedoes the player has.
    int numTorpedoes() const;
    
private:
    double m_cabbagePoints;
    int m_torpedos;
};

class Alien : public DamageableObject
{
public:
    Alien(StudentWorld* w, double startX, double startY, int imageID,
          double hitPoints, double damageAmt, double deltaX,
          double deltaY, double speed, unsigned int scoreValue);
    
    virtual bool collidableWithPlayerFiredProjectile() const;
    virtual void sufferDamage(double amt, int cause, int score);
    
    // Move the player by the current speed in the direction indicated
    // by the x and y deltas.
    void move();
    
    // Set the player's y direction.
    void setDeltaY(double dy);
    
    // Set the player's speed.
    void setDeltaX(double dx);
    
    double getDeltaX();
    
    // get speed
    double getDeltaY();
    
    void setFlightPlan(double length);
    
    // If this alien collided with the player, damage the player and return
    // true; otherwise, return false.
    virtual bool damageCollidingPlayer(double amt, int scoreValue);
    
    // If this alien drops goodies, drop one with the appropriate probability.
    virtual void possiblyDropGoodie();
    
    // detemine if alien will shoot
    virtual bool shoot(int choice);
    
    // get alien score value
    unsigned int getScoreValue();
    
private:
    double          m_deltaX;
    double          m_deltaY;
    double          m_speed;
    double          m_flight_plan;
    unsigned int    m_scoreValue;
};

class Smallgon : public Alien
{
public:
    Smallgon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Smoregon : public Alien
{
public:
    Smoregon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void possiblyDropGoodie();
};

class Snagglegon : public Alien
{
public:
    Snagglegon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void possiblyDropGoodie();
    virtual bool shoot(int choice);
};

class Projectile : public Actor
{
public:
    Projectile(StudentWorld* w, double startX, double startY, int imageID, double damageAmt, double deltaX, bool rotates, int imageDir);
    virtual void moveTo(double x, double y);
    
    // if alien was hit by projectile return true apply damage
    // otherwise false
    bool alienCollidedWithProj(int damage);
    
    // if player was hit by projectile return true apply damage
    // otherwise false
    bool playerCollidedWithProj(int damage);
};

class Cabbage : public Projectile
{
public:
    Cabbage(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Turnip : public Projectile
{
public:
    Turnip(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Torpedo : public Projectile
{
public:
    Torpedo(StudentWorld* w, double startX, double startY, double deltaX, int imageDir);
};

class PlayerLaunchedTorpedo : public Torpedo
{
public:
    PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class AlienLaunchedTorpedo : public Torpedo
{
public:
    AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Goodie : public Actor
{
public:
    Goodie(StudentWorld* w, double startX, double startY, int imageID);
    
    // if player collided with goodie return true and apply goodie
    // otherswise false
    virtual bool helpCollidingPlayer();
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual bool helpCollidingPlayer();
};

class RepairGoodie : public Goodie
{
public:
    RepairGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual bool helpCollidingPlayer();
};

class TorpedoGoodie : public Goodie
{
public:
    TorpedoGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual bool helpCollidingPlayer();
};

#endif // ACTOR_H_
