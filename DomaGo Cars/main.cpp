#include <SFML/Graphics.hpp>
#include <iostream>
#include "simulator.cpp"


using namespace std;

//auto se vozi sa w,s,a,d i ako dode do crne pozadine vraca se na pocetak

// 1=right 2=left 3=right&break 4=left&break 5=gas 6=idle 7=left&gas 8=right&gas
int run(const simulator& sim) {

    if (sim.getTopDistance() < 200) {
        if (sim.getTopLeftDistance() - sim.getTopRightDistance() > 0) {
            if (sim.getV() > 0.7) {
                return 4;
            }
            else if (sim.getV() < 0.5)
            {
                return 7;
            }
            else {
                return 2;
            }
        }
        else {
            if (sim.getV() > 0.7) {
                return 3;
            }
            else if (sim.getV() < 0.5)
            {
                return 8;
            }
            else {
                return 1;
            }
        }
    }
    if (sim.getLeftDistance() < 200) {
        if (sim.getV() > 0.7) {
            return 3;
        }
        else if (sim.getV() < 0.5)
        {
            return 8;
        }
        else {
            return 1;
        }
    }

    if (sim.getRightDistance() < 200) {
        if (sim.getV() > 0.7) {
            return 4;
        }
        else if (sim.getV() < 0.5)
        {
            return 7;
        }
        else {
            return 2;
        }
    }

    if (sim.getV() < 0.7) {
        return 5;
    }

    return 6;

}

double evaluate()
{
    int imageWidth = 1152;
    int imageHeight = 648;
    sf::RectangleShape background(sf::Vector2f(imageWidth, imageHeight));
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("background.jpg");
    background.setTexture(&backgroundTexture);

    sf::CircleShape player(20.0f);
    player.setOrigin(12.0f, 15.0f);
    player.setScale(1.5, 1.2);
    player.setPosition(imageWidth / 2, imageHeight / 1.2);
    player.setRotation(0);

    auto image = backgroundTexture.copyToImage(); //jer texture nema getPixel, dimenzije se smanjuju za 5/3

    sf::Vector2f vector = player.getPosition();
    simulator sim(vector.x, vector.y, image);
    sim.setV(0.4);

    while (true)
    {

        int akcija = run(sim);
        //cout << akcija << endl;
        if (akcija == 1)
        {
            sim.rotateRight();
        }
        else if (akcija == 2)
        {
            sim.rotateLeft();
        }
        else if (akcija == 3)
        {
            sim.rotateRight();
            sim.brake();
        }
        else if (akcija == 4)
        {
            sim.rotateLeft();
            sim.brake();
        }
        else if (akcija == 5)
        {
            sim.gas();
        }
        else if (akcija == 7)
        {
            sim.rotateLeft();
            sim.gas();
        }
        else if (akcija == 8)
        {
            sim.rotateRight();
            sim.gas();
        }
        else
        {
            sim.idle();
        }

        sim.update();
        player.setPosition(sim.getX(), sim.getY());
        player.setRotation(sim.getAngle() * -1.0);


        float x = player.getPosition().x;
        float y = player.getPosition().y;

        //cout << sim.getAngle() << " " << sim.getTopDistance() << " " << sim.getLeftDistance() << " " << sim.getRightDistance() << endl;

        auto color1 = image.getPixel(x, y);

        if (color1 == sf::Color::Black)
        {
            return sim.getAngleDistance() * sim.getAngleDistance() / sim.getT();
        }
    }
}
