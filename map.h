#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include <string>
#include <vector>

class Map {
public:
    std::string name;
    std::vector<Rectangle> platforms;

    Vector2 player1Spawn;
    Vector2 player2Spawn;

    void draw() const;
    void drawPreview(Rectangle bounds) const;
};

Map createTrainingMap();
Map createPlatformMap();
std::vector<Map> createMaps();

#endif
