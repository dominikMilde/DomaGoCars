#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "simulator.h"

using namespace std;

float simulator::getDistanceToBound(Vector2 pos, float angle) const {
    float dirX = cos(angle / 180 * PI);
    float dirY = -sin(angle / 180 * PI); // Y se smanjuje prema gore

    int dist = globalConfig.blok;
    while (true) {
        int x = round(pos.x + dist * dirX);
        int y = round(pos.y + dist * dirY);
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || mat[x][y]) break;
        dist += globalConfig.blok;
    }
    dist -= globalConfig.blok;
    while (true) {
        int x = round(pos.x + dist * dirX);
        int y = round(pos.y + dist * dirY);
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || mat[x][y]) { break; }
        dist += 1;
    }

    //cout << angle << " " << dirX << " " << dirY << " " << dist << endl;

    return dist;
}


simulator::simulator(float x1, float y1, const sf::Image& image) {
    for (int i = 0; i < WIDTH; ++i) for (int j = 0; j < HEIGHT; ++j) {
        mat[i][j] = (image.getPixel(i, j) == sf::Color::Black);
    }
    t = 0;
    pos.x = x1;
    pos.y = y1;
    // end su koordinate donjeg desnog vrha prozora, pretpostavimo da prozor pocinje od (0,0)
    end = { 1152.f, 648.f };
    v = 0.0;
    angle = 0.0;
    acc = 0.0;
    traction = 0.0;
    topDistance = 0.0f;
    leftDistance = 0.0f;
    rightDistance = 0.0f;
    topLeftDistance = 0.0f;
    topRightDistance = 0.0f;
    angleDistance = 0.0f;
}

void simulator::update() {
    t += 1;
    acc = traction - globalConfig.drag * v * v - globalConfig.rr * v;
    v = v + acc * KOEF;
    if (v < 0) v = 0;

    angle = fmod(angle + 360, 360);

    Vector2 oldPos = pos;
    pos.x = pos.x + cos(angle * PI / 180) * v * KOEF;
    pos.y = pos.y - sin(angle * PI / 180) * v * KOEF;

    topDistance = getTopBoundDistance(pos, angle);
    leftDistance = getLeftBoundDistance(pos, angle);
    rightDistance = getRightBoundDistance(pos, angle);
    topLeftDistance = getTopLeftBoundDistance(pos, angle);
    topRightDistance = getTopRightBoundDistance(pos, angle);
    float newAngle = calcAngle(center, oldPos, pos);
    if (isfinite(newAngle))
        angleDistance += newAngle;
}