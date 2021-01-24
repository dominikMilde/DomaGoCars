#ifndef SCREEN_H
#define SCREEN_H
class Screen
{
public:
    virtual int Run(sf::RenderWindow& App) = 0;
};
#endif