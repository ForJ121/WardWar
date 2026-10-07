#include "player.h"
#include "map.h"
#include <algorithm>

void Player::move(const Map& map) {
    float dt = GetFrameTime();

    // Keep the feet in place when changing between standing and crouching.
    float feetY = y + height;
    bool crouching = IsKeyDown(keyDown);
    if (crouching != isCrouching) {
        height += crouching ? -20 : 20;
        speed *= crouching ? 0.5f : 2.0f;
        y = feetY - height;
        isCrouching = crouching;
    }

    if (IsKeyPressed(keyUp) && jumpCount > 0) {
        isGrounded = false;
        speedY = jumpBeginSpeed;
        jumpCount--;
    }

    if (IsKeyDown(keyLeft)) {
        x -= speed * dt;
        isRight = false;
    }
    if (IsKeyDown(keyRight)) {
        x += speed * dt;
        isRight = true;
    }

    if (x < 0) x = 0;
    if (x + width > GetScreenWidth()) x = GetScreenWidth() - width;

    // Check support every frame so walking or blinking off a platform falls.
    speedY += gravity * dt;
    y += speedY * dt;
    isGrounded = false;

    if (speedY >= 0) {
        float landingY = (float)GetScreenHeight();
        bool landed = feetY <= landingY && y + height >= landingY;

        for (const Rectangle& platform : map.platforms) {
            bool overlapsX = x < platform.x + platform.width && x + width > platform.x;
            bool crossesTop = feetY <= platform.y && y + height >= platform.y;
            if (overlapsX && crossesTop && platform.y <= landingY) {
                landingY = platform.y;
                landed = true;
            }
        }

        if (landed) {
            y = landingY - height;
            speedY = 0;
            isGrounded = true;
            jumpCount = 2;
        }
    }

    if (y < 0) {
        y = 0;
        speedY = 0;
    }
}

Rectangle Player::getBodyRect() {
    return Rectangle{ x, y, (float)width, (float)height };
}

Rectangle Player::getAttackRect() {
    float attackWidth = 70.0f;
    if (isRight) {
        return Rectangle{ x + width, y, attackWidth, (float)height };
    }
    else {
        return Rectangle{ x - attackWidth, y, attackWidth, (float)height };
    }
}


void Player::attack(Player& enemy) {
    if (!IsKeyPressed(keyAttack)) {
        return;
    }
    if (CheckCollisionRecs(getAttackRect(), enemy.getBodyRect())) {
        if (!enemy.isDefending) {
            enemy.blood -= 10;
            if (enemy.blood < 0) {
                enemy.blood = 0;
            }
        }
    }
}

void Player::defend(){
    float dt = GetFrameTime();
    if (isDefending) {
        defendTime += dt;
        if (defendTime >= defendMaxTime) {
            isDefending = false;
            defendTime = 0.0f;
            defendCoolTime = defendMaxCoolTime;
        }
        return;
    }
    if (defendCoolTime > 0.0f) {
        defendCoolTime -= dt;
        if (defendCoolTime < 0.0f) {
            defendCoolTime = 0.0f;
        }
        return;
    }
    if (IsKeyPressed(keyDefend)) {
        isDefending = true;
        defendTime = 0.0f;
    }
}

void Player::skill1(Player& enemy) {
    double now = GetTime();
    if (IsKeyPressed(keySkill1) && now >= nextSkill1Time && skill >= 10) {
        skill -= 10;
        Projectile projectile{};
        projectile.color = color;
        projectile.style = projectileStyle;
        int direction;
        if (isRight) {
            direction = 1;
        }
        else {
            direction = -1;
        }
        float startX;

        if (isRight) {
            startX = x + width;
        }
        else {
            startX = x - projectile.width;
        }

        float startY =
            y + height / 2.0f - projectile.height / 2.0f;

        projectile.launch(
            startX,
            startY,
            direction
        );
        projectiles.push_back(projectile);
        nextSkill1Time = now + skill1Interval;
    }

    // 每一发子弹分别移动并判断命中。
    for (Projectile& projectile : projectiles) {
        projectile.update();

        if (projectile.active && CheckCollisionRecs(projectile.getRect(), enemy.getBodyRect())) {
            if (!enemy.isDefending) {
                enemy.blood -= projectile.damage;
                if (enemy.blood < 0) {
                    enemy.blood = 0;
                }
            }

            // 命中或被防御挡住后，这一发子弹消失。
            projectile.active = false;
        }
    }

    // 清理已经命中或飞出屏幕的子弹。
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const Projectile& projectile) { return !projectile.active; }),
        projectiles.end()
    );
}

void Player::skill2() {
    double now = GetTime();
    if (IsKeyPressed(keySkill2) && now >= nextSkill2Time && skill >= 5 ) {
        skill -= 5;
        float flashDistance = 125.0f;
        if (isRight) {
            x += flashDistance;
            if (x + width > GetScreenWidth()) {
                x = GetScreenWidth() - width;
            }
        }
        else {
            x -= flashDistance;
            if (x < 0) {
                x = 0;
            }
        }
        nextSkill2Time = now + skill2Interval;
    }
}

void Player::recoverSkill() {
    if (!IsKeyDown(keySkill1) && !IsKeyDown(keySkill2) && skill < 200.0f) {

        float dt = GetFrameTime();
        skill += dt;
        if (skill > 200.0f) {
            skill = 200.0f;
        }
    }
}

void Player::draw() {
    DrawRectangle(x, y, width, height, color);
    if (isRight) {
        DrawCircle(x + width - 8, y + 20, 10, BLACK);
    }
    else {
        DrawCircle(x + 8, y + 20, 10, BLACK);
    }

    for (Projectile& projectile : projectiles) {
        projectile.draw();
    }
}
