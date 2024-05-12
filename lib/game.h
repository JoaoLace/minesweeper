#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>
#include <cstdlib>

#define window_width 800
#define window_height 800
#define window_size sf::VideoMode(window_width, window_height)
#define field_width 15
#define field_height 15
#define light_green sf::Color(167, 217, 72, 255)
#define green sf::Color(142, 204, 57, 255)
#define light_brown sf::Color(215, 184, 153, 255)
#define brown sf::Color(229, 194, 159, 255)
#define black sf::Color::Black
#define pink sf::Color(210,122,245,255)
#define orange sf::Color(245,80,0,255)
#define light_green sf::Color(245,221,0,255)

extern sf::Font font;
void initFont();
const float tileSize = window_width / field_width;

class tile
{
private:
    void init(sf::Vector2f position);

public:
    tile(sf::Vector2f position);
    ~tile();
    sf::RectangleShape sprite;
    sf::Text numberOfBombs;
    bool isBomb;
    bool clicked;
    bool flaged;
    int bombsArround;
    int x;
    int y;
};

class game
{
private:
    sf::RenderWindow *window;
    char field[field_width][field_height];
    bool gameRunning;
    bool gameStarted;
    std::vector<tile> *tiles;
    std::vector<sf::Sprite> *flags;
    // sf::Font font;
    sf::Texture flagTexture;
    sf::Text guiTexts [7];
    sf::SoundBuffer sb_bomb;
    sf::SoundBuffer sb_win;
    sf::Sound s_win;
    sf::Sound s_bombS;
    sf::Sprite flagControl;
    sf::Text tx_numberOfFlags;
    int maxBombs;
    int currentBombs;
    int gameMode;
    int maxFlags;
    int currentFlags;
    bool changeColor;
    bool lostBool;
    void init();
    void initWindow();
    void initSound();
    void initVars();
    void initField();
    void initTiles();
    void initFlag();
    void initInitalScreen();
    void update();
    void updateEvents();
    void updateInitalScreen();
    void updateTiles();
    void updateClick(int x, int y);
    void render();
    void renderTiles();
    void renderFlags();
    void renderNumbers();
    void renderNumberOfFlags();
    void renderTextInitalScreen();
    void renderInitalScreen();
    int calculateBombs(int x, int y);
    void calculateAllBombs();
    void handleClick(int x, int y);
    bool tileIsBomb(int x, int y);
    int returnIndex(int x, int y);
    void setTileTextColor(int index);
    void createFlag(int x, int y);
    void testFlag(int x, int y);
    bool testWin();
    void removeFlag(int x, int y);
    void debug();
    void inicialScreen();
    void win();
    void lost();
    void resetField();
    void showBombs();
     // lmao
public:
    game();
    ~game();
    void run();
    void debugMode();
};
