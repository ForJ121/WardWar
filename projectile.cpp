#include "projectile.h"

void Projectile::launch(float startX, float startY, int dir) {
    x = startX;
    y = startY;

    direction = dir;
    active = true;
}

void Projectile::update() {
    if (!active) {
        return;
    }

    x += speed * direction * GetFrameTime();

    // 飞出屏幕就消失
    if (x > GetScreenWidth() || x + width < 0) {
        active = false;
    }
}

void Projectile::draw() {
    if (!active) {
        return;
    }

    float centerY = y + height / 2;

    if (style == ProjectileStyle::Fireball) {
        float radius = height / 2;
        Vector2 center = {direction > 0 ? x + width - radius : x + radius, centerY};
        float tailX = direction > 0 ? x : x + width;

        DrawLineEx({tailX, centerY}, center, height * 0.5f, Fade(color, 0.6f));
        DrawCircleV(center, radius, color);
        DrawCircleV(center, radius * 0.65f, ORANGE);
        DrawCircleV({center.x + direction * radius * 0.25f, centerY - radius * 0.15f},
                    radius * 0.3f, YELLOW);
    }
    else if (style == ProjectileStyle::Crystal) {
        Vector2 left = {x, centerY};
        Vector2 right = {x + width, centerY};
        Vector2 top = {x + width / 2, y};
        Vector2 bottom = {x + width / 2, y + height};

        DrawTriangle(left, bottom, top, direction < 0 ? SKYBLUE : color);
        DrawTriangle(top, bottom, right, direction > 0 ? SKYBLUE : color);
        DrawLineEx(left, top, 1.5f, color);
        DrawLineEx(top, right, 1.5f, color);
        DrawLineEx(right, bottom, 1.5f, color);
        DrawLineEx(bottom, left, 1.5f, color);
        DrawCircleV({x + width / 2, centerY}, height * 0.12f, WHITE);
    }
}

Rectangle Projectile::getRect() {
    return {
        x,
        y,
        width,
        height
    };
}
