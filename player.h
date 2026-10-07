#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "projectile.h"
#include <vector>

class Map;

class Player {
    public:
        float x;
        float y;
        int width;
        int height;
        int blood = 500;
        float skill = 300.0f;

        float speed;
        bool isRight;

        bool isGrounded = true;
        int jumpCount = 2;
        float jumpBeginSpeed = -500;
        float speedY;
        float gravity = 1000;
        bool isCrouching = false;
        Color color;

        int keyUp;
        int keyDown;
        int keyLeft;
        int keyRight;
        int keyAttack;
        int keyDefend;
        int keySkill1;
        int keySkill2;

        bool isDefending = false;
        float defendTime = 0.0f;
        float defendCoolTime = 0.0f;
        float defendMaxTime = 3.0f;
        float defendMaxCoolTime = 0.5f;

        void move(const Map& map);

        void attack(Player& enemy);
        void defend();
        Rectangle getBodyRect();
        Rectangle getAttackRect();

        std::vector<Projectile> projectiles;
        ProjectileStyle projectileStyle;
        double skill1Interval = 0.2;
        double nextSkill1Time = 0.0;
        double skill2Interval = 1.0;
        double nextSkill2Time = 0.0;
        void skill1(Player& enemy);
        void skill2();
        void recoverSkill();
        void draw();
};
#endif
