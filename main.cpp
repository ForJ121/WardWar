#if defined(_WIN32)
// Keep Windows declarations from conflicting with raylib names.
#define WIN32_LEAN_AND_MEAN
#define NOUSER
#define Rectangle Win32Rectangle
#include <windows.h>
#include <imm.h>
#undef Rectangle
#undef NOUSER
#undef WIN32_LEAN_AND_MEAN
#endif

#include "raylib.h"
#include "player.h"
#include "projectile.h"
#include "map.h"

enum class Scene {
    Menu,
    MapSelect,
    Battle
};

enum class GameMode {
    PVP,
    PVE
};

Rectangle GetMapButtonRect(int row) {
    return {60, 135.0f + row * 64, 330, 54};
}

void ResetPlayer(Player& player1, Player& player2, const Map& map) {
    player1 = Player{};
    player2 = Player{};
    player1.x = map.player1Spawn.x;
    player1.width = 50;
    player1.height = 100;
    player1.y = map.player1Spawn.y;
    player1.speed = 200;
    player1.color = RED;
    player1.projectileStyle = ProjectileStyle::Fireball;
    player1.keyUp = KEY_W;
    player1.keyDown = KEY_S;
    player1.keyLeft = KEY_A;
    player1.keyRight = KEY_D;
    player1.keyAttack = KEY_J;
    player1.keyDefend = KEY_SPACE;
    player1.keySkill1 = KEY_K;
    player1.keySkill2 = KEY_L;
    player1.isRight = true;
    
    player2.x = map.player2Spawn.x;
    player2.width = 50;
    player2.height = 100;
    player2.y = map.player2Spawn.y;
    player2.speed = 200;
    player2.color = BLUE;
    player2.projectileStyle = ProjectileStyle::Crystal;
    player2.keyUp = KEY_UP;
    player2.keyDown = KEY_DOWN;
    player2.keyLeft = KEY_LEFT;
    player2.keyRight = KEY_RIGHT;
    player2.keyAttack = KEY_KP_1;
    player2.keyDefend = KEY_KP_0;
    player2.keySkill1 = KEY_KP_2;
    player2.keySkill2 = KEY_KP_3;
    player2.isRight = false;
}

int main(){
    InitWindow(1000, 600, "WardWar-v1.0");
#if defined(_WIN32)
    // Gameplay uses raw keys; IME text composition can consume attack keys.
    HWND gameWindow = static_cast<HWND>(GetWindowHandle());
    HIMC previousInputContext = ImmAssociateContext(gameWindow, nullptr);
#endif
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Scene scene = Scene::Menu;
    GameMode mode = GameMode::PVP;

    const std::vector<Map> maps = createMaps();
    const int mapCount = (int)maps.size();
    const int mapsPerPage = 5;
    const int pageCount = (mapCount + mapsPerPage - 1) / mapsPerPage;
    int selectedMap = 0;
    int mapPage = 0;

    Rectangle pvpButton   = {280, 220, 200, 60};
    Rectangle pveButton   = {520, 220, 200, 60};
    Rectangle nextButton  = {400, 340, 200, 60};
    Rectangle backButton = {60, 520, 180, 50};
    Rectangle startButton = {740, 520, 200, 50};
    Rectangle previousPageButton = {60, 475, 90, 30};
    Rectangle nextPageButton = {300, 475, 90, 30};
    Rectangle mapPreview = {450, 175, 480, 288};

    bool gameOver = false;
    int winner = 0;
    Player player1{};
    Player player2{};
    

    while (!WindowShouldClose()) {
        if (scene == Scene::Menu) {
            Vector2 mouse = GetMousePosition();

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mouse, pvpButton)) {
                    mode = GameMode::PVP;
                }
                else if (CheckCollisionPointRec(mouse, pveButton)) {
                    mode = GameMode::PVE;
                }
                else if (CheckCollisionPointRec(mouse, nextButton) && mode == GameMode::PVP) {
                    mapPage = selectedMap / mapsPerPage;
                    scene = Scene::MapSelect;
                }
            }
        }
        else if (scene == Scene::MapSelect) {
            Vector2 mouse = GetMousePosition();
            bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if (IsKeyPressed(KEY_ESCAPE) || (clicked && CheckCollisionPointRec(mouse, backButton))) {
                scene = Scene::Menu;
            }
            else if (clicked) {
                if (mapCount > 0 && CheckCollisionPointRec(mouse, startButton)) {
                    ResetPlayer(player1, player2, maps[selectedMap]);
                    gameOver = false;
                    winner = 0;
                    scene = Scene::Battle;
                }
                else if (mapPage > 0 && CheckCollisionPointRec(mouse, previousPageButton)) {
                    mapPage--;
                }
                else if (mapPage + 1 < pageCount && CheckCollisionPointRec(mouse, nextPageButton)) {
                    mapPage++;
                }
                else {
                    for (int row = 0; row < mapsPerPage; row++) {
                        int index = mapPage * mapsPerPage + row;
                        if (index >= mapCount) break;
                        if (CheckCollisionPointRec(mouse, GetMapButtonRect(row))) {
                            selectedMap = index;
                            break;
                        }
                    }
                }
            }
        }
        else if (scene == Scene::Battle) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                mapPage = selectedMap / mapsPerPage;
                scene = Scene::MapSelect;
            }
            if (gameOver && IsKeyPressed(KEY_R)) {
                ResetPlayer(player1, player2, maps[selectedMap]);
                gameOver = false;
                winner = 0;
            }
        }


        if (scene == Scene::Battle && !gameOver) {
            player1.move(maps[selectedMap]);
            player2.move(maps[selectedMap]);

            player1.defend();
            player2.defend();

            player1.attack(player2);
            player2.attack(player1);

            player1.skill1(player2);
            player2.skill1(player1);
            player1.skill2();
            player2.skill2();
            player1.recoverSkill();
            player2.recoverSkill();

            if (player1.blood <= 0 && player2.blood <= 0) {
                gameOver = true;
                winner = 0;
            }
            else if (player1.blood <= 0 ){
                gameOver = true;
                winner = 2;
            }
            else if (player2.blood <= 0 ){
                gameOver = true;
                winner = 1;
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (scene == Scene::Menu) {
            DrawText("WARD WAR", 370, 100, 40, BLACK);

            DrawRectangleRec(pvpButton, mode == GameMode::PVP ? BLUE : LIGHTGRAY);
            DrawRectangleRec(pveButton, mode == GameMode::PVE ? BLUE : LIGHTGRAY);
            DrawText("PVP", 350, 238, 24, mode == GameMode::PVP ? WHITE : BLACK);
            DrawText("PVE", 590, 238, 24, mode == GameMode::PVE ? WHITE : BLACK);

            bool canContinue = mode == GameMode::PVP;
            DrawRectangleRec(nextButton, canContinue ? GREEN : GRAY);
            DrawText("NEXT", 465, 358, 24, canContinue ? BLACK : DARKGRAY);

            if (mode == GameMode::PVE) {
                DrawText("PVE is not available yet.", 340, 430, 22, DARKGRAY);
            }
        }
        else if (scene == Scene::MapSelect) {
            DrawText("SELECT MAP", 350, 45, 36, BLACK);
            DrawText("Mode: PVP", 60, 95, 20, DARKGRAY);

            for (int row = 0; row < mapsPerPage; row++) {
                int index = mapPage * mapsPerPage + row;
                if (index >= mapCount) break;

                Rectangle button = GetMapButtonRect(row);
                bool selected = index == selectedMap;
                DrawRectangleRec(button, selected ? BLUE : LIGHTGRAY);
                DrawText(TextFormat("%d. %s", index + 1, maps[index].name.c_str()),
                         (int)button.x + 14, (int)button.y + 16, 22,
                         selected ? WHITE : BLACK);
            }

            if (mapCount > 0) {
                DrawText(maps[selectedMap].name.c_str(), 450, 130, 26, BLACK);
                maps[selectedMap].drawPreview(mapPreview);
                DrawText("Red / blue: starting positions", 450, 475, 20, DARKGRAY);
            }
            else {
                DrawText("No maps available.", 450, 230, 24, DARKGRAY);
            }

            if (pageCount > 1) {
                DrawRectangleRec(previousPageButton, mapPage > 0 ? LIGHTGRAY : GRAY);
                DrawText("PREV", 78, 482, 18, mapPage > 0 ? BLACK : DARKGRAY);
                DrawText(TextFormat("%d / %d", mapPage + 1, pageCount), 195, 482, 18, BLACK);
                DrawRectangleRec(nextPageButton, mapPage + 1 < pageCount ? LIGHTGRAY : GRAY);
                DrawText("NEXT", 320, 482, 18, mapPage + 1 < pageCount ? BLACK : DARKGRAY);
            }

            DrawRectangleRec(backButton, LIGHTGRAY);
            DrawText("BACK", 120, 535, 22, BLACK);
            DrawRectangleRec(startButton, mapCount > 0 ? GREEN : GRAY);
            DrawText("START", 795, 535, 22, mapCount > 0 ? BLACK : DARKGRAY);
        }
        else if (scene == Scene::Battle) {
            maps[selectedMap].draw();
            player1.draw();
            player2.draw();
            DrawText(TextFormat("P1 Blood: %d", player1.blood), 20, 20, 20, RED);
            DrawText(TextFormat("P2 Blood: %d", player2.blood), 20, 50, 20, RED);
            DrawText(TextFormat("P1 Skill: %.2f", player1.skill), 20, 80, 20, BLUE);
            DrawText(TextFormat("P2 Skill: %.2f", player2.skill), 20, 110, 20, BLUE);

            DrawText("ESC: Map selection", 730, 20, 20, DARKGRAY);
            DrawText(maps[selectedMap].name.c_str(), 400, 20, 20, DARKGRAY);

            if (gameOver) {
                if (winner == 1) {
                    DrawText("PLAYER 1 WINS!", 300, 200, 40, RED);
                }
                else if (winner == 2) {
                    DrawText("PLAYER 2 WINS!", 300, 200, 40, RED);
                }
                else {
                    DrawText("DRAW!", 350, 200, 40, RED);
                }
                DrawText("Press R to restart", 320, 260, 30, DARKGRAY);
            }
        }
        EndDrawing();
    }
#if defined(_WIN32)
    ImmAssociateContext(gameWindow, previousInputContext);
#endif
    CloseWindow();
    return 0;
}
