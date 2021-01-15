#pragma once

#define PI 3.14159265
#define SQRT2 1.414213562
#define EPSILON 1E-10

// KOEF - koeficijent za ubrzanje ili usporenje simulacije
#define KOEF 1

// KONSTANTE ZA UBRZANJE/USPORENJE AUTICA
// gas - konstantno povecava brzinu
#define GAS_ACC 0.00002
// kocnica - konstantno smanjuje brzinu
#define BRAKE_ACC -0.00001
// trenje - ovisi o brzini
#define RR 0.0003
// otpor zraka - ovisi o brzini na kvadrat
#define DRAG 0.00001

// KONSTANTE ZA SKRETANJE AUTICA
// brzina okretanja volana
#define STEERING_RATE 0.007
// brzina vracanja volana u normalu
#define STEERING_RETURN_RATE 0.005
// maksimalan kut zakretaja volana
#define MAX_STEERING_ANGLE 10
// rotacija autica pri mirovanju - ne ovisi o brzini
#define ROTATION_IDX_0 0.001
// rotacija autica pri voznji - ovisi o brzini
#define ROTATION_IDX_1 0.1

#define WIDTH 1152
#define HEIGHT 648

#define BLOK 20

struct Vector2
{
    float x;
    float y;
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

const Vector2 center = Vector2(WIDTH / 2.0f, HEIGHT / 2.0f);

class simulator {
private:
    bool mat[WIDTH][HEIGHT];

    Vector2 pos;
    // end su koordinate donjeg desnog vrha prozora, pretpostavimo da prozor pocinje od (0,0)
    Vector2 end;
    double v;
    double angle;
    double steering;
    double acc;
    double traction;
    int t;
    float topDistance;
    float leftDistance;
    float rightDistance;
    float topLeftDistance;
    int topRightDistance;
    float angleDistance;

    float getDistanceToBound(Vector2 pos, float angle) const;

    static float calcAngle(Vector2 a, Vector2 b, Vector2 c) {
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
        if (steering < MAX_STEERING_ANGLE)
            steering += STEERING_RATE;
    }
    void rotateRight() {
        if (steering > -MAX_STEERING_ANGLE)
            steering -= STEERING_RATE;
    }
    void gas() {
        traction = GAS_ACC;
    }
    void brake() {
        traction = BRAKE_ACC;
    }
    void idle() {
        traction = 0.0;
    }

    float getTopBoundDistance(Vector2 pos, float angle) const {
        return getDistanceToBound(pos, angle);
    }

    float getLeftBoundDistance(Vector2 pos, float angle) const {
        return getDistanceToBound(pos, fmod(angle + 90, 360));
    }

    float getRightBoundDistance(Vector2 pos, float angle) const {
        return getDistanceToBound(pos, fmod(angle - 90, 360));
    }

    float getTopLeftBoundDistance(Vector2 pos, float angle) const {
        return getDistanceToBound(pos, fmod(angle + 45, 360));
    }

    float getTopRightBoundDistance(Vector2 pos, float angle) const {
        return getDistanceToBound(pos, fmod(angle - 45, 360));
    }
};
