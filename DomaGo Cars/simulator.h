#pragma once

#define PI 3.14159265
#define SQRT2 1.414213562
#define EPSILON 1E-10


#define KOEF 1
#define GAS_ACC 0.0005
#define BRAKE_ACC -0.001
#define DRAG_K 0.994
#define T 1.0
#define ROTATION_IDX 0.7

#define WIDTH 1152
#define HEIGHT 648

#define BLOK 20

struct Point
{
    float x;
    float y;
    Point() : x(0), y(0) {}
    Point(float x, float y) : x(x), y(y) {}
};

const Point center = Point(WIDTH / 2.0f, HEIGHT / 2.0f);

class simulator {
private:
    bool mat[WIDTH][HEIGHT];

    Point pos;
    // end su koordinate donjeg desnog vrha prozora, pretpostavimo da prozor pocinje od (0,0)
    Point end;
    double v;
    double angle;
    double acc;
    int t;
    float topDistance;
    float leftDistance;
    float rightDistance;
    float topLeftDistance;
    int topRightDistance;
    float angleDistance;

    float getDistanceToBound(Point pos, float angle) const;

    static float calcAngle(Point a, Point b, Point c) {
        float ab = hypot(a.x - b.x, a.y - b.y);
        float ac = hypot(a.x - c.x, a.y - c.y);
        float bc = hypot(b.x - c.x, b.y - c.y);
        return acos((ab * ab + ac * ac - bc * bc) / (2 * ab * ac)) * 180 / PI;
    }
    

public:
    simulator(float x1, float y1, const sf::Image& image);

    void update();

    float getAngle() const { return angle; }
    float getV() const { return v; }
    float getX() const { return pos.x; }
    float getY() const { return pos.y; }
    int getT() const { return t; }
    float getTopDistance() const { return topDistance; }
    float getLeftDistance() const { return leftDistance; }
    float getRightDistance() const { return rightDistance; }
    float getTopLeftDistance() const { return topLeftDistance; }
    float getTopRightDistance() const { return topRightDistance; }
    float getAngleDistance() const { return angleDistance; }
    void setAngleDistance(float x) { angleDistance = x; }


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
    void setT(int t) {
        this->t = t;
    }
    void rotateLeft() {
        this->angle += (getV() * ROTATION_IDX);
    }
    void rotateRight() {
        this->angle -= (getV() * ROTATION_IDX);
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

    float getTopBoundDistance(Point pos, float angle) const {
        return getDistanceToBound(pos, angle);
    }

    float getLeftBoundDistance(Point pos, float angle) const {
        return getDistanceToBound(pos, fmod(angle + 90, 360));
    }

    float getRightBoundDistance(Point pos, float angle) const {
        return getDistanceToBound(pos, fmod(angle - 90, 360));
    }

    float getTopLeftBoundDistance(Point pos, float angle) const {
        return getDistanceToBound(pos, fmod(angle + 45, 360));
    }

    float getTopRightBoundDistance(Point pos, float angle) const {
        return getDistanceToBound(pos, fmod(angle - 45, 360));
    }
};
