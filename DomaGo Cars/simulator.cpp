#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>

#define PI 3.14159265
#define SQRT2 1.414213562
#define EPSILON 1E-10

#define GAS_ACC 0.001
#define BRAKE_ACC -0.05
#define DRAG_K 0.999
#define T 1.0

class simulator {
private:
    const sf::Image& image;
    struct Point
    {
        float x;
        float y;
        Point() : x(0), y(0) {}
        Point(float x, float y) : x(x), y(y) {}
    };
    Point pos;
    // end su koordinate donjeg desnog vrha prozora, pretpostavimo da prozor pocinje od (0,0)
    Point end;
    double v;
    double angle;
    double acc;
    int t;
    int topDistance;
    int leftDistance;
    int rightDistance;
    int topLeftDistance;
    int topRightDistance;

    // funkcija koja racuna tocku presjeka vektora senzora i ruba prozora GUI-ja, rezultat vraca pohranjen u referenci koja mu je predana kao argument
    void vectorRectangleIntersection(Point pos, double angle, Point& ret)
    {
        double tanA = tan(angle * PI / 180);
       
        if (abs(tanA) < EPSILON)
        {
            if (abs(angle) < EPSILON)
            {
                ret.x = end.x;
                ret.y = pos.y;
            }
            else if (abs(angle) - 180 < EPSILON)
            {
                ret.y = pos.y;
            }
        }

        else
        {
            if (tanA < 0)
                tanA = -tanA;
            if (angle < 90 || angle > 270)
            {
                float yRight = pos.y - (tanA * (end.x - pos.x));
                if (yRight < 0)
                {
                    ret.y = end.x + (yRight / tanA);
                }
                else if (yRight > end.y)
                {
                    float opposite = yRight - end.y;
                    ret.x = end.x - (opposite / tanA);
                    ret.y = end.y;
                }
                else
                {
                    ret.x = end.x;
                    ret.y = yRight;
                }
            }
            else
            {
                float yLeft = pos.y + (tanA * pos.x);
                if (yLeft < 0)
                {
                    ret.x = -yLeft / tanA;
                }
                else if (yLeft > end.y)
                {
                    float opposite = yLeft - end.y;
                    ret.x = opposite / tanA;
                    ret.y = end.y;
                }
                else
                {
                    ret.y = yLeft;
                }
            }
        }
    }

    Point binarySearch(int startX, int startY, int finishX, int finishY)
    {
        if (finishX > startX || finishY > startY)
        {
            int midX = startX + (finishX - startX) / 2;
            int midY = startY + (finishY - startY) / 2;

            sf::Color color = image.getPixel(midX, midY);

            if (color == sf::Color::Black)
                return binarySearch(startX, startY, midX, midY);

            return binarySearch(midX, midY, finishX, finishY);
        }

        return { startX, startY };
    }

    double getDistanceToBound(Point pos, double angle) {
        Point edgeGui;
        vectorRectangleIntersection(pos, angle, edgeGui);

        Point edgeMap = binarySearch(pos.x, pos.y, edgeGui.x, edgeGui.y);

        return sqrt(pow(edgeMap.x- pos.x, 2) + pow(edgeMap.y - pos.y, 2));
    }

public:
    simulator(float x1, float y1, const sf::Image& image1) : image(image) {
        t = 0;
        pos.x = x1;
        pos.y = y1;
        // end su koordinate donjeg desnog vrha prozora, pretpostavimo da prozor pocinje od (0,0)
        end = { 1152, 648 };
        v = 0.0;
        angle = 0.0;
        acc = 0.0;
        topDistance = 0.0;
        leftDistance = 0.0;
        rightDistance = 0.0;
        topLeftDistance = 0.0;
        topRightDistance = 0.0;
    }

    void update(const sf::Image& image) {
        t += 1;
        v = (v + acc * T) * DRAG_K;
        if (v < 0) v = 0;
        if (angle <= 0) { angle += 360; }
        pos.x = pos.x + cos(angle * PI / 180) * v * T;
        pos.y = pos.y - sin(angle * PI / 180) * v * T;
        topDistance = getTopBoundDistance(pos, angle);
        leftDistance = getLeftBoundDistance(pos, angle);
        rightDistance = getRightBoundDistance(pos, angle);
        topLeftDistance = getTopLeftBoundDistance(pos, angle);
        topRightDistance = getTopRightBoundDistance(pos, angle);
    }

    double getAngle() { return angle; }
    double getV() { return v; }
    double getX() { return pos.x; }
    double getY() { return poy; }
    int getT() { return t; }
    int getTopDistance() { return topDistance; }
    int getLeftDistance() { return leftDistance; }
    int getRightDistance() { return rightDistance; }
    int getTopLeftDistance() { return topLeftDistance; }
    int getTopRightDistance() { return topRightDistance; }


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

    int getTopBoundDistance(Point pos, double angle) {
        return getDistanceToBound(pos, angle);
    }

    int getLeftBoundDistance(Point pos, double angle) {
        return getDistanceToBound(pos, ((angle + 90) % 360));
    }

    int getRightBoundDistance(Point pos, double angle) {
        return getDistanceToBound(pos, angle);
    }

    int getTopLeftBoundDistance(Point pos, double angle) {
        return getDistanceToBound(pos, angle);
    }

    int getTopRightBoundDistance(Point pos, double angle) {
        return getDistanceToBound(pos, angle);
    }
};