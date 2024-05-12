#include "../lib/game.h"

tile::tile(sf::Vector2f position)
{
    init(position);
}

tile::~tile()
{
}

void tile::init(sf::Vector2f position)
{
    numberOfBombs.setFont(font);
    numberOfBombs.setCharacterSize(24);
    numberOfBombs.setPosition(sf::Vector2f(position.x + (tileSize/4) + 2.4,position.y + (tileSize/4) ));
    
    sprite.setSize(sf::Vector2f(tileSize,tileSize));
    sprite.setOutlineColor(black);
    sprite.setOutlineThickness(1.f);
    sprite.setPosition(position);

    clicked = false;
    flaged = false;
}
