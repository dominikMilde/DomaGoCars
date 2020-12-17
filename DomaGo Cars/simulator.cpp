#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>

#define PI 3.14159265
#define GAS_ACC 0.001
#define BRAKE_ACC -0.05
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
    int t;
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
        t = 0;
        x = x1;
        y = y1;
        v = 0.0;
        angle = 0.0;
        acc = 0.0;
        topDistance = 0.0;
        leftDistance = 0.0;
        rightDistance = 0.0;
        topLeftDistance = 0.0;
        topRightDistance = 0.0;

        if (int(angle) % 360 <= 22 || int(angle) % 360 >= 338)
        {
            topDistance = getRightBoundDistance(x1, y1, image);
            leftDistance = getTopBoundDistance(x1, y1, image);
            rightDistance = getBotBoundDistance(x1, y1, image);
            topLeftDistance = getTopRightBoundDistance(x1, y1, image);
            topRightDistance = getBotRightBoundDistance(x1, y1, image);
        } 
        else if (int(angle) % 360 >= 23 && int(angle) % 360 <= 67)
        {
            topDistance = getTopRightBoundDistance(x1, y1, image);
            leftDistance = getTopLeftBoundDistance(x1, y1, image);
            rightDistance = getBotRightBoundDistance(x1, y1, image);
            topLeftDistance = getTopBoundDistance(x1, y1, image);
            topRightDistance = getRightBoundDistance(x1, y1, image);
        }
        else if (int(angle) % 360 >= 68 && int(angle) % 360 <= 112)
        {
            topDistance = getTopBoundDistance(x1, y1, image);
            leftDistance = getLeftBoundDistance(x1, y1, image);
            rightDistance = getRightBoundDistance(x1, y1, image);
            topLeftDistance = getTopLeftBoundDistance(x1, y1, image);
            topRightDistance = getTopRightBoundDistance(x1, y1, image);
        }
        else if (int(angle) % 360 >= 113 && int(angle) % 360 <= 157)
        {
            topDistance = getTopLeftBoundDistance(x1, y1, image);
            leftDistance = getBotLeftBoundDistance(x1, y1, image);
            rightDistance = getTopRightBoundDistance(x1, y1, image);
            topLeftDistance = getLeftBoundDistance(x1, y1, image);
            topRightDistance = getTopBoundDistance(x1, y1, image);
        }
        else if (int(angle) % 360 >= 158 && int(angle) % 360 <= 202)
        {
            topDistance = getLeftBoundDistance(x1, y1, image);
            leftDistance = getBotBoundDistance(x1, y1, image);
            rightDistance = getTopBoundDistance(x1, y1, image);
            topLeftDistance = getBotLeftBoundDistance(x1, y1, image);
            topRightDistance = getTopLeftBoundDistance(x1, y1, image);
        }
        else if (int(angle) % 360 >= 203 && int(angle) % 360 <= 247)
        {
            topDistance = getBotLeftBoundDistance(x1, y1, image);
            leftDistance = getBotRightBoundDistance(x1, y1, image);
            rightDistance = getTopLeftBoundDistance(x1, y1, image);
            topLeftDistance = getBotBoundDistance(x1, y1, image);
            topRightDistance = getLeftBoundDistance(x1, y1, image);
        }
        else if (int(angle) % 360 >= 248 && int(angle) % 360 <= 292)
        {
            topDistance = getBotBoundDistance(x1, y1, image);
            leftDistance = getRightBoundDistance(x1, y1, image);
            rightDistance = getLeftBoundDistance(x1, y1, image);
            topLeftDistance = getBotRightBoundDistance(x1, y1, image);
            topRightDistance = getBotLeftBoundDistance(x1, y1, image);
        }
        else if (int(angle) % 360 >= 293 && int(angle) % 360 <= 337)
        {
            topDistance = getBotRightBoundDistance(x1, y1, image);
            leftDistance = getTopRightBoundDistance(x1, y1, image);
            rightDistance = getBotLeftBoundDistance(x1, y1, image);
            topLeftDistance = getRightBoundDistance(x1, y1, image);
            topRightDistance = getBotBoundDistance(x1, y1, image);
        }

    }

    void update(const sf::Image& image) {
        t += 1;
        v = (v + acc * T) * DRAG_K;
        if (v < 0) v = 0;
        if (angle <= 0) { angle += 360; }
        x = x + cos(angle * PI / 180) * v * T;
        y = y - sin(angle * PI / 180) * v * T;
        if (int(angle) % 360 <= 22 || int(angle) % 360 >= 338)
        {
            topDistance = getRightBoundDistance(x, y, image);
            leftDistance = getTopBoundDistance(x, y, image);
            rightDistance = getBotBoundDistance(x, y, image);
            topLeftDistance = getTopRightBoundDistance(x, y, image);
            topRightDistance = getBotRightBoundDistance(x, y, image);
        }
        else if (int(angle) % 360 >= 23 && int(angle) % 360 <= 67)
        {
            topDistance = getTopRightBoundDistance(x, y, image);
            leftDistance = getTopLeftBoundDistance(x, y, image);
            rightDistance = getBotRightBoundDistance(x, y, image);
            topLeftDistance = getTopBoundDistance(x, y, image);
            topRightDistance = getRightBoundDistance(x, y, image);
        }
        else if (int(angle) % 360 >= 68 && int(angle) % 360 <= 112)
        {
            topDistance = getTopBoundDistance(x, y, image);
            leftDistance = getLeftBoundDistance(x, y, image);
            rightDistance = getRightBoundDistance(x, y, image);
            topLeftDistance = getTopLeftBoundDistance(x, y, image);
            topRightDistance = getTopRightBoundDistance(x, y, image);
        }
        else if (int(angle) % 360 >= 113 && int(angle) % 360 <= 157)
        {
            topDistance = getTopLeftBoundDistance(x, y, image);
            leftDistance = getBotLeftBoundDistance(x, y, image);
            rightDistance = getTopRightBoundDistance(x, y, image);
            topLeftDistance = getLeftBoundDistance(x, y, image);
            topRightDistance = getTopBoundDistance(x, y, image);
        }
        else if (int(angle) % 360 >= 158 && int(angle) % 360 <= 202)
        {
            topDistance = getLeftBoundDistance(x, y, image);
            leftDistance = getBotBoundDistance(x, y, image);
            rightDistance = getTopBoundDistance(x, y, image);
            topLeftDistance = getBotLeftBoundDistance(x, y, image);
            topRightDistance = getTopLeftBoundDistance(x, y, image);
        }
        else if (int(angle) % 360 >= 203 && int(angle) % 360 <= 247)
        {
            topDistance = getBotLeftBoundDistance(x, y, image);
            leftDistance = getBotRightBoundDistance(x, y, image);
            rightDistance = getTopLeftBoundDistance(x, y, image);
            topLeftDistance = getBotBoundDistance(x, y, image);
            topRightDistance = getLeftBoundDistance(x, y, image);
        }
        else if (int(angle) % 360 >= 248 && int(angle) % 360 <= 292)
        {
            topDistance = getBotBoundDistance(x, y, image);
            leftDistance = getRightBoundDistance(x, y, image);
            rightDistance = getLeftBoundDistance(x, y, image);
            topLeftDistance = getBotRightBoundDistance(x, y, image);
            topRightDistance = getBotLeftBoundDistance(x, y, image);
        }
        else if (int(angle) % 360 >= 293 && int(angle) % 360 <= 337)
        {
            topDistance = getBotRightBoundDistance(x, y, image);
            leftDistance = getTopRightBoundDistance(x, y, image);
            rightDistance = getBotLeftBoundDistance(x, y, image);
            topLeftDistance = getRightBoundDistance(x, y, image);
            topRightDistance = getBotBoundDistance(x, y, image);
        }

    }

    double getAngle() { return angle; }
    double getV() { return v; }
    double getX() { return x; }
    double getY() { return y; }
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