#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>

#define PI 3.14159265
#define GAS_ACC 0.001
#define BRAKE_ACC -0.01
#define DRAG_K 0.999
#define T 1.0
#define SQRT2 1.414213562

class simulator {
private:
    double x;
    double y;
    double v;
    double angle;
    double acc;
    int topDistance;
    int botDistance;
    int leftDistance;
    int rightDistance;
    int topLeftDistance;
    int topRightDistance;
    int botLeftDistance;
    int botRightDistance;

public:
    simulator(double x1, double y1, const sf::Image& image) {
        x = x1;
        y = y1;
        v = 0.0;
        angle = 0.0;
        acc = 0.0;
        topDistance = getTopBoundDistance(x1, y1 , image);
        botDistance = getBotBoundDistance(x1, y1, image);
        leftDistance = getLeftBoundDistance(x1, y1, image);
        rightDistance = getRightBoundDistance(x1, y1, image);
        topLeftDistance = getTopLeftBoundDistance(x1, y1, image);
        topRightDistance = getTopRightBoundDistance(x1, y1, image);
        botLeftDistance = getBotLeftBoundDistance(x1, y1, image);
        botRightDistance = getBotRightBoundDistance(x1, y1, image);
    }

    void update(const sf::Image& image) {
        v = (v + acc * T) * DRAG_K;
        if (v < 0) v = 0;
        if (angle <= 0) { angle += 360; }
        x = x + cos(angle * PI / 180) * v * T;
        y = y - sin(angle * PI / 180) * v * T;
        topDistance = getTopBoundDistance(x, y, image);
        botDistance = getBotBoundDistance(x, y, image);
        leftDistance = getLeftBoundDistance(x, y, image);
        rightDistance = getRightBoundDistance(x, y, image);
        topLeftDistance = getTopLeftBoundDistance(x, y, image);
        topRightDistance = getTopRightBoundDistance(x, y, image);
        botLeftDistance = getBotLeftBoundDistance(x, y, image);
        botRightDistance = getBotRightBoundDistance(x, y, image);

    }

    double getAngle() { return angle; }
    double getV() { return v; }
    double getX() { return x; }
    double getY() { return y; }
    int getTopDistance() { return topDistance; }
    int getBotDistance() { return botDistance; }
    int getLeftDistance() { return leftDistance; }
    int getRightDistance() { return rightDistance; }
    int getTopLeftDistance() { return topLeftDistance; }
    int getTopRightDistance() { return topRightDistance; }
    int getBotLeftDistance() { return botLeftDistance; }
    int getBotRightDistance() { return botRightDistance; }


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

    int getTopBoundDistance(int x, int y, const sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        int i = 1;
        while (color != sf::Color::Black)
        {
            color = image.getPixel(x, y - i);
            i += 1;
        }

        return (i - 1) * 1.666666666666666666666;
    }

    int getBotBoundDistance(int x, int y, const sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        int i = 1;
        while (color != sf::Color::Black)
        {
            color = image.getPixel(x, y + i);
            i += 1;
        }

        return (i - 1);
    }


    int getLeftBoundDistance(int x, int y, const sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        int i = 1;
        while (color != sf::Color::Black)
        {
            color = image.getPixel(x - i, y);
            i += 1;
        }

        return (i - 1);
    }

    int getRightBoundDistance(int x, int y, const sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        int i = 1;
        while (color != sf::Color::Black)
        {
            color = image.getPixel(x + i, y);
            i += 1;
        }

        return (i - 1);
    }

    int getTopLeftBoundDistance(int x, int y, const sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        int i = 1;
        while (color != sf::Color::Black)
        {
            color = image.getPixel(x - i, y - i);
            i += 1;
        } 

        i -= 1;

        return SQRT2 * i;
    }

    int getTopRightBoundDistance(int x, int y, const sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        int i = 1;
        while (color != sf::Color::Black)
        {
            color = image.getPixel(x + i, y - i);
            i += 1;
        }

        i -= 1;

        return SQRT2 * i;
    }

    int getBotLeftBoundDistance(int x, int y, const sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        int i = 1;
        while (color != sf::Color::Black)
        {
            color = image.getPixel(x - i, y + i);
            i += 1;
        }

        i -= 1;

        return SQRT2 * i;
    }

    int getBotRightBoundDistance(int x, int y, const sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        int i = 1;
        while (color != sf::Color::Black)
        {
            color = image.getPixel(x + i, y + i);
            i += 1;
        }

        i -= 1;

        return SQRT2 * i;
    }
};