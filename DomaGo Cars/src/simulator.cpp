#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "simulator.h"

using namespace std;

float simulator::getDistanceToBound(Vector2 pos, float angle, bool onTrack) const {
    float dirX = cos(angle / 180 * PI);
    float dirY = -sin(angle / 180 * PI); // Y se smanjuje prema gore

    int dist = globalConfig.blok;
    while (true) {
        int x = round(pos.x + dist * dirX);
        int y = round(pos.y + dist * dirY);
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) break;
        if (onTrack ? mat[x][y] : !mat[x][y]) break;
        dist += globalConfig.blok;
    }
    dist -= globalConfig.blok;
    while (true) {
        int x = round(pos.x + dist * dirX);
        int y = round(pos.y + dist * dirY);
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) break;
        if (onTrack ? mat[x][y] : !mat[x][y]) break;
        dist += 1;
    }

    return dist;
}

float simulator::getDistanceToBound(Vector2 pos, float angle) const {
    return getDistanceToBound(pos, angle, true);
}


simulator::simulator(float x1, float y1, const sf::Image& image) {
    for (int i = 0; i < WIDTH; ++i) for (int j = 0; j < HEIGHT; ++j) {
        mat[i][j] = (image.getPixel(i, j) == sf::Color::Black);
    }
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
    topDistance = 0.0f;
    leftDistance = 0.0f;
    rightDistance = 0.0f;
    topLeftDistance = 0.0f;
    topRightDistance = 0.0f;
    angleDistance = 0.0f;
}

void simulator::update(bool isLearning) {
    t += 1;
    scaledT += KOEF;
    acc = traction - globalConfig.drag * v * v - globalConfig.rr * v;
    v = v + acc * KOEF;
    if (v < 0) v = 0;

    angle = fmod(angle + 360, 360);

    Vector2 oldPos = pos;
    pos.x = pos.x + cos(angle * PI / 180) * v * KOEF;
    pos.y = pos.y - sin(angle * PI / 180) * v * KOEF;

    if (isLearning) {
        topDistance = getTopBoundDistance(pos, angle);
        leftDistance = getLeftBoundDistance(pos, angle);
        rightDistance = getRightBoundDistance(pos, angle);
        topLeftDistance = getTopLeftBoundDistance(pos, angle);
        topRightDistance = getTopRightBoundDistance(pos, angle);
    }
    float newAngle = calcAngle(center, oldPos, pos);
    // zanemaruje posebne sluèajeve koje nastaju kada vozilo prolazi preko x ili y osi
    if (abs(newAngle) > 10)
        newAngle = 0;
    if (isfinite(newAngle))
        angleDistance += newAngle;
}

Vector2 simulator::calculateNewPosition(Vector2 oldPos, float direction, float distance) const {
    float dirX = cos(direction / 180 * PI);
    float dirY = -sin(direction / 180 * PI);

    int x = round(oldPos.x + distance * dirX);
    int y = round(oldPos.y + distance * dirY);

    return Vector2(x, y);
}

void simulator::calculateCrashReturn() {
    angleDistance -= 15;

    float dir = fmod(angleDistance - 90, 360);
    float dist = getDistanceToBound(center, dir, false) + 2;
    Vector2 newPos = calculateNewPosition(center, dir, dist);
    dist += getDistanceToBound(newPos, dir, true) / 2;
    newPos = calculateNewPosition(center, dir, dist);

    pos.x = newPos.x;
    pos.y = newPos.y;
    angle = angleDistance;
    v = 0.0;
    acc = 0.0;
    traction = 0.0;
}