#include "map.h"

void Map::draw() const {
    for (const Rectangle& platform : platforms) {
        DrawRectangleRec(platform, GRAY);
    }
}

void Map::drawPreview(Rectangle bounds) const {
    float scaleX = bounds.width / GetScreenWidth();
    float scaleY = bounds.height / GetScreenHeight();

    DrawRectangleRec(bounds, WHITE);
    for (const Rectangle& platform : platforms) {
        DrawRectangleRec({
            bounds.x + platform.x * scaleX,
            bounds.y + platform.y * scaleY,
            platform.width * scaleX,
            platform.height * scaleY
        }, GRAY);
    }

    DrawRectangleRec({bounds.x + player1Spawn.x * scaleX,
                      bounds.y + player1Spawn.y * scaleY,
                      50 * scaleX, 100 * scaleY}, RED);
    DrawRectangleRec({bounds.x + player2Spawn.x * scaleX,
                      bounds.y + player2Spawn.y * scaleY,
                      50 * scaleX, 100 * scaleY}, BLUE);
    DrawRectangleLinesEx(bounds, 2, LIGHTGRAY);
}

Map createTrainingMap() {
    Map map;
    map.name = "Training Ground";

    map.platforms.push_back({
        0, 550, 1000, 50
    });

    map.player1Spawn = {100, 450};
    map.player2Spawn = {850, 450};

    return map;
}

Map createPlatformMap() {
    Map map;
    map.name = "Platforms";

    // 地面
    map.platforms.push_back({
        0, 550, 1000, 50
    });

    // 左平台
    map.platforms.push_back({
        200, 400, 100, 20
    });

    // 右平台
    map.platforms.push_back({
        700, 400, 100, 20
    });

    // 中间平台
    map.platforms.push_back({
        350, 250, 300, 20
    });

    map.player1Spawn = {100, 450};
    map.player2Spawn = {850, 450};

    return map;
}

std::vector<Map> createMaps() {
    // Register new maps here; the selection menu and its pages use this list.
    return {createTrainingMap(), createPlatformMap()};
}
