#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

#define PI 3.14159265
#define SQRT2 1.414213562
#define EPSILON 1E-10

#define GAS_ACC 0.001
#define BRAKE_ACC -0.05
#define DRAG_K 0.999
#define T 1.0

#define WIDTH 1152
#define HEIGHT 648

#define BLOK 20

using namespace std;
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

    float getDistanceToBound(Point pos, float angle) const {
        float dirX = cos(angle / 180 * PI);
        float dirY = -sin(angle / 180 * PI); // Y se smanjuje prema gore

        int dist = BLOK;
        while (true) {
            int x = round(pos.x + dist * dirX);
            int y = round(pos.y + dist * dirY);
            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || mat[x][y]) break;
            dist += BLOK;
        }
        dist -= BLOK;
        while (true) {
            int x = round(pos.x + dist * dirX);
            int y = round(pos.y + dist * dirY);
            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || mat[x][y]) { break; }
            dist += 1;
        }

        //cout << angle << " " << dirX << " " << dirY << " " << dist << endl;

        return dist;
    }

    static float calcAngle(Point a, Point b, Point c) {
        float ab = hypot(a.x - b.x, a.y - b.y);
        float ac = hypot(a.x - c.x, a.y - c.y);
        float bc = hypot(b.x - c.x, b.y - c.y);
        return acos((ab * ab + ac * ac - bc * bc) / (2 * ab * ac)) * 180 / PI;
    }

public:
    simulator(float x1, float y1, const sf::Image& image) {
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
        topDistance = 0.0f;
        leftDistance = 0.0f;
        rightDistance = 0.0f;
        topLeftDistance = 0.0f;
        topRightDistance = 0.0f;
        angleDistance = 0.0f;
    }

    void update() {
        t += 1;
        v = (v + acc * T) * DRAG_K;
        if (v < 0) v = 0;
        if (angle <= 0) { angle += 360; }
        Point oldPos = pos;
        pos.x = pos.x + cos(angle * PI / 180) * v * T;
        pos.y = pos.y - sin(angle * PI / 180) * v * T;
        topDistance = getTopBoundDistance(pos, angle);
        leftDistance = getLeftBoundDistance(pos, angle);
        rightDistance = getRightBoundDistance(pos, angle);
        topLeftDistance = getTopLeftBoundDistance(pos, angle);
        topRightDistance = getTopRightBoundDistance(pos, angle);
        float newAngle = calcAngle(center, oldPos, pos);
        if (isfinite(newAngle))
            angleDistance += newAngle;
    }

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