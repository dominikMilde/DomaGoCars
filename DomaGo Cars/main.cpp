#include <SFML/Graphics.hpp>
#include <iostream>
#include "simulator.h"
#include "main.h"

using namespace std;

//auto se vozi sa w,s,a,d i ako dode do crne pozadine vraca se na pocetak

// 1=right 2=left 3=right&break 4=left&break 5=gas 6=left&gas 7=right&gas
int run(const simulator& sim, neuralnetwork& nn) {
    nn.nIn[0] = sim.getTopDistance();
    nn.nIn[1] = sim.getLeftDistance();
    nn.nIn[2] = sim.getRightDistance();
    nn.nIn[3] = sim.getTopLeftDistance();
    nn.nIn[4] = sim.getTopRightDistance();

    nn.propagate();

    int idx = max_element(nn.nOut.begin(), nn.nOut.end()) - nn.nOut.begin();
    return idx + 1;
}

double evaluate(neuralnetwork& nn)
{
    int imageWidth = 1152;
    int imageHeight = 648;
    //sf::RenderWindow window(sf::VideoMode(imageWidth, imageHeight), "SFML auti", sf::Style::Close | sf::Style::Titlebar);
    sf::RectangleShape background(sf::Vector2f(imageWidth, imageHeight));
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("background.jpg");
    background.setTexture(&backgroundTexture);

    sf::CircleShape player(20.0f);
    player.setOrigin(12.0f, 15.0f);
    player.setScale(1.5, 1.2);
    player.setPosition(imageWidth / 2, imageHeight / 1.2);
    player.setRotation(0);
    sf::Texture playerTexture;
    playerTexture.loadFromFile("avatar.jpg");
    player.setTexture(&playerTexture);

    auto image = backgroundTexture.copyToImage(); //jer texture nema getPixel, dimenzije se smanjuju za 5/3

    sf::Vector2f vector = player.getPosition();
    simulator sim(vector.x, vector.y, image);
    //sim.setV(0.4);

    while (true)
    {
        int akcija = run(sim, nn);
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
        else if (akcija == 6)
        {
            sim.rotateLeft();
            sim.gas();
        }
        else if (akcija == 7)
        {
            sim.rotateRight();
            sim.gas();
        }

        sim.update();
        player.setPosition(sim.getX(), sim.getY());
        player.setRotation(sim.getAngle() * -1.0);

        /*window.clear();
        window.draw(background);
        window.draw(player);
        window.display();*/

        float x = player.getPosition().x;
        float y = player.getPosition().y;

        if (sim.getT() % 2000 == 0)
            cout << sim.getAngle() << " " << sim.getTopDistance() << " " << sim.getLeftDistance() << " " << sim.getRightDistance()
                 << " " << sim.getV() << " " << akcija << " " << sim.getT() << endl;

        auto color1 = image.getPixel(x, y);

        if (color1 == sf::Color::Black || sim.getAngleDistance() > 10000 || sim.getT() > 100000)
        {
            cout << sim.getAngleDistance() * sim.getAngleDistance() / sim.getT() << endl;
            return sim.getAngleDistance() * sim.getAngleDistance() / sim.getT();
        }
    }
}
