#include <math.h>

#define PI 3.14159265

class simulator {
private:
    float x;
    float y;
    float v;
    float angle;

public:
    simulator(float x1, float y1, float v1, float angle1) {
        x = x1;
        y = y1;
        v = v1;
        angle = angle1;
    }

    void update(float acc, float T, float k, float newAngle) {
        v = (v + acc * T) * k;
        if (v < 0) v = 0;
        angle = 90 - newAngle;
        if (angle <= 0) { angle += 360; }
        x = x + cos(angle * PI / 180) * v * T;
        y = y - sin(angle * PI / 180) * v * T;
    }

    float getAngle() { return angle; }
    float getV() { return v; }
    float getX() { return x; }
    float getY() { return y; }
    void setV(float v) {
        this->v = v;
    }
    void setX(float x) {
        this->x = x;
    }
    void setY(float y) {
        this->y = y;
    }
    void setAngle(float angle) {
        this->angle= angle;
    }
};