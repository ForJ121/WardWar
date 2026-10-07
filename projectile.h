#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "raylib.h"

enum class ProjectileStyle {
    Fireball,
    Crystal
};

class Projectile {
public:
    float x;
    float y;

    float width = 30;
    float height = 20;

    float speed = 200;
    int damage = 10;

    int direction = 1;   // 1向右，-1向左
    bool active = false;

    Color color = RED;
    ProjectileStyle style = ProjectileStyle::Fireball;

    void launch(float startX, float startY, int dir);
    void update();
    void draw();

    Rectangle getRect();
};

#endif
