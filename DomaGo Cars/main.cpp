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

int imageWidth = 1152;
int imageHeight = 648;
sf::RenderWindow window(sf::VideoMode(imageWidth, imageHeight), "SFML auti", sf::Style::Close | sf::Style::Titlebar);
sf::RectangleShape background(sf::Vector2f(imageWidth, imageHeight));
sf::Texture backgroundTexture;
sf::CircleShape player(20.0f);
sf::Image image;
sf::Texture playerTexture;

void init() {
    backgroundTexture.loadFromFile("background.jpg");
    background.setTexture(&backgroundTexture);
    player.setOrigin(12.0f, 15.0f);
    player.setScale(1.5, 1.2);
    playerTexture.loadFromFile("avatar.jpg");
    player.setTexture(&playerTexture);
    image = backgroundTexture.copyToImage();
    player.setPosition(imageWidth / 2, imageHeight / 1.2);
    window.clear();
    window.draw(background);
    window.draw(player);
    window.display();
}

double evaluate(neuralnetwork& nn)
{
    player.setPosition(imageWidth / 2, imageHeight / 1.2);
    player.setRotation(0);

    sf::Vector2f vector = player.getPosition();
    simulator sim(vector.x, vector.y, image);
    //sim.setV(0.4);

    while (window.isOpen())
    {
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                std::cout << "New window width: " << evnt.size.width << " New window height: " << evnt.size.height << std::endl;
                break;
            case sf::Event::TextEntered:
                break;
            }
        }
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

        if (sim.getT() % 20 == 0) {
            window.clear();
            window.draw(background);
            window.draw(player);
            window.display();
        }

        float x = player.getPosition().x;
        float y = player.getPosition().y;

        if (sim.getT() % 2000 == 0)
            cout << sim.getAngle() << " " << sim.getTopDistance() << " " << sim.getLeftDistance() << " " << sim.getRightDistance()
                 << " " << sim.getV() << " " << akcija << " " << sim.getT() << endl;

        auto color1 = image.getPixel(x, y);

        if (color1 == sf::Color::Black || sim.getAngleDistance() > 10000 || sim.getT() > 200000)
        {
            cout << pow(sim.getAngleDistance(), 2.5) / sim.getT() << endl;
            return pow(sim.getAngleDistance(), 2.5) / sim.getT();
        }
    }
}
