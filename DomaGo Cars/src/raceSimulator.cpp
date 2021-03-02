#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "raceSimulator.h"

using namespace std;

raceSimulator::raceSimulator(float x1, float y1, const sf::Image& image) {
    t = 0;
    scaledT = 0.0;
    pos.x = x1;
    pos.y = y1;
    // end su koordinate donjeg desnog vrha prozora, pretpostavimo da prozor pocinje od (0,0)
    end = { 1152.f, 648.f };
    v = 0.0;
    angle = 0.0;
    acc = 0.0;
    traction = 0.0;
}

void raceSimulator::update() {
    t += 1;
    scaledT += KOEF;
    acc = traction - globalConfig.drag * v * v - globalConfig.rr * v;
    v = v + acc * KOEF;
    if (v < 0) v = 0;

    angle = fmod(angle + 360, 360);

    Vector2 oldPos = pos;
    pos.x = pos.x + cos(angle * PI / 180) * v * KOEF;
    pos.y = pos.y - sin(angle * PI / 180) * v * KOEF;

    float newAngle = calcAngle(center, oldPos, pos);
    // zanemaruje posebne sluèajeve koje nastaju kada vozilo prolazi preko x ili y osi
    if (abs(newAngle) > 0.1)
        newAngle = 0;
    if (isfinite(newAngle))
        angleDistance += newAngle;
}