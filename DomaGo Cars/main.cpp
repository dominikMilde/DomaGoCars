#include <iostream>
#include <time.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include "simulator.h"
#include "CGP.h"

#define KOEF 2

using namespace std;

string run(const simulator& sim, CGP* cgp, vector<int> graph) {

    vector<double> inputs;
    inputs.push_back(sim.getV());
    inputs.push_back(sim.getTopDistance());
    inputs.push_back(sim.getTopRightDistance());
    inputs.push_back(sim.getTopLeftDistance());
    inputs.push_back(sim.getRightDistance());
    inputs.push_back(sim.getLeftDistance());

    vector<double> outputs = cgp->calculateOutputs(inputs, graph);

    double speed = outputs.at(0);
    double angle = outputs.at(1);

    std::string akcija = "";

    if (speed < -1.) akcija += "00"; //uspori
    else if (speed > 1.) akcija += "11"; //ubrzaj
    else akcija += "01"; //idle

    if (angle < -1.) akcija += "00"; //left
    else if (angle > 1.) akcija += "11"; //right
    else akcija += "01"; //nista

    return akcija;
}

int imageWidth = 1152;
int imageHeight = 648;
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
}

double evaluate(CGP* cgp, vector<int> graph)
{
    player.setPosition(imageWidth / 2, imageHeight / 1.2);
    player.setRotation(0);

    sf::Vector2f vector = player.getPosition();
    simulator sim(vector.x, vector.y, image);

    while (true) {

        std::string akcija = run(sim, cgp, graph);

        if (akcija == "0000")
        {
            sim.brake();
            sim.rotateLeft();
        }
        else if (akcija == "0001")
        {
            sim.brake();
        }
        else if (akcija == "0011")
        {
            sim.brake();
            sim.rotateRight();
        }
        else if (akcija == "0100")
        {
            sim.idle();
            sim.rotateLeft();
        }
        else if (akcija == "0101")
        {
            sim.idle();
        }
        else if (akcija == "0111")
        {
            sim.idle();
            sim.rotateRight();
        }
        else if (akcija == "1100")
        {
            sim.gas();
            sim.rotateLeft();
        }
        else if (akcija == "1101")
        {
            sim.gas();
        }
        else
        {
            sim.gas();
            sim.rotateRight();
        }

        sim.update();
        player.setPosition(sim.getX(), sim.getY());
        player.setRotation(sim.getAngle() * -1.0);

        float x = player.getPosition().x;
        float y = player.getPosition().y;

        auto color1 = image.getPixel(x, y);

        if (color1 == sf::Color::Black || sim.getT() > 10000)
        {
            //return pow(sim.getAngleDistance(), KOEF) / sim.getT();
            break;
        }
    }
    return pow(sim.getAngleDistance(), KOEF) / sim.getT();
}

/*int main()
{
    int imageWidth = 1152;
    int imageHeight = 648;
    sf::RenderWindow window(sf::VideoMode(imageWidth, imageHeight), "SFML auti", sf::Style::Close | sf::Style::Titlebar);
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

    std::cout << image.getSize().x << std::endl;
    std::cout << image.getSize().y << std::endl;

    sf::Vector2f vector = player.getPosition();
    simulator sim(vector.x, vector.y, image);
    sim.setV(0.4);

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

        window.clear();
        window.draw(background);
        window.draw(player);
        window.display();

        float x = player.getPosition().x;
        float y = player.getPosition().y;

        cout << x << " " << y << endl << endl;

        //cout << sim.getAngle() << " " << sim.getTopDistance() << " " << sim.getLeftDistance() << " " << sim.getRightDistance() << endl;

        auto color1 = image.getPixel(x, y);

        if (color1 == sf::Color::Black)
        {
            std::cout << "T: " << sim.getT() << std::endl;
            std::cout << "Distance(in degrees): " << sim.getAngleDistance() << std::endl;
            sim.setT(0);
            sim.setX(imageWidth / 2);
            sim.setY(imageHeight / 1.2);
            sim.setAngle(0);
            player.setRotation(0);
            sim.setV(0);
            sim.setAngleDistance(0.0f);
        }
    }
    return 0;
}*/
