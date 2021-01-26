#pragma once
#include "config.h"

#define PI 3.14159265
#define SQRT2 1.414213562
#define EPSILON 1E-10

#define WIDTH 1152
#define HEIGHT 648

using namespace std;

struct Vector2
{
    float x;
    float y;
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

const Vector2 center = Vector2(WIDTH / 2.0f, HEIGHT / 2.0f);

class raceSimulator {
private:
    // KOEF - koeficijent za ubrzanje ili usporenje simulacije
    float KOEF;

    Vector2 pos;
    // end su koordinate donjeg desnog vrha prozora, pretpostavimo da prozor pocinje od (0,0)
    Vector2 end;
    double v;
    double angle;
    double acc;
    double traction;
    int t;
    float scaledT;
    float angleDistance;

    static float vectorAngle(Vector2 pos, Vector2 center) {
        float x = pos.x - center.x;
        float y = center.y - pos.y;

        if (abs(x) < 1e-6) {
            if (y > 0) return (PI / 2);
            else if (y < 0) return ((PI * 3) / 2);
            else return 0.f;
        }

        float angle = atan2(y, x);
        if (y < 0)
            angle = (angle + (2 * PI));

        return angle;
    }

    static float calcAngle(Vector2 a, Vector2 b, Vector2 c) {
        float diff = (vectorAngle(c, a) - vectorAngle(b, a)) * 180 / PI;
        //cout << "diff " << diff << " " << endl;

        if (diff < -180) {
            //cout << "diff < -180" << endl;
            diff += 360;
        }
        else if (diff > 180) {
            //cout << "diff > 180" << endl;
            diff = 360 - diff;
        }

        return diff;
    }

public:
    raceSimulator(float x1, float y1, const sf::Image& image);

    void update();

    float getAngle() const { return angle; }
    float getV() const { return v; }
    float getX() const { return pos.x; }
    float getY() const { return pos.y; }
    int getT() const { return t; }
    float getScaledT() const { return scaledT; }
    float getAngleDistance() const { return angleDistance; }
    void setAngleDistance(float x) { angleDistance = x; }

    void setKOEF(float KOEF) {
        this->KOEF = KOEF;
    }

    void setV(float v) {
        this->v = v;
    }
    void setX(float x) {
        this->pos.x = x;
    }
    void setY(float y) {
        this->pos.y = y;
    }
    void setAngle(float angle) {
        this->angle = angle;
    }
    void setT(float t) {
        this->t = t;
    }
    void setScaledT(float scaledT) {
        this->scaledT = scaledT;
    }
    void rotateLeft() {
        angle += (globalConfig.rotationIdx1 * getV() + globalConfig.rotationIdx0) * KOEF;
    }
    void rotateRight() {
        angle -= (globalConfig.rotationIdx1 * getV() + globalConfig.rotationIdx0) * KOEF;
    }
    void gas() {
        traction = globalConfig.gasAcc;
    }
    void brake() {
        traction = globalConfig.brakeAcc;
    }
    void idle() {
        traction = 0.0;
    }

};

