#include <math.h>

#define PI 3.14159265
#define GAS_ACC 0.001
#define BRAKE_ACC -0.01
#define DRAG_K 0.999
#define T 1.0

class simulator {
private:
    double x;
    double y;
    double v;
    double angle;
    double acc;

public:
    simulator(double x1, double y1) {
        x = x1;
        y = y1;
        v = 0.0;
        angle = 0.0;
        acc = 0.0;
    }

    void update() {
        v = (v + acc * T) * DRAG_K;
        if (v < 0) v = 0;
        if (angle <= 0) { angle += 360; }
        x = x + cos(angle * PI / 180) * v * T;
        y = y - sin(angle * PI / 180) * v * T;
    }

    double getAngle() { return angle; }
    double getV() { return v; }
    double getX() { return x; }
    double getY() { return y; }
    void setV(double v) {
        this->v = v;
    }
    void setX(double x) {
        this->x = x;
    }
    void setY(double y) {
        this->y = y;
    }
    void setAngle(double angle) {
        this->angle = angle;
    }
    void rotateLeft() {
        this->angle += (getV() / 4);
    }
    void rotateRight() {
        this->angle -= (getV() / 4);
    }
    void gas() {
        acc = GAS_ACC;
    }
    void brake() {
        acc = BRAKE_ACC;
    }
    void idle() {
        acc = 0.0;
    }
};