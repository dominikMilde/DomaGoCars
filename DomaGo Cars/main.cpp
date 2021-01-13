#include <iostream>
#include <time.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include "simulator.h"
#include "CGP.h"

#define KOEF 2

using namespace std;

int run(const simulator& sim, CGP& cgp) {

    cgp.inputs[0] = sim.getV();
    cgp.inputs[1] = sim.getTopDistance();
    cgp.inputs[2] = sim.getTopRightDistance();
    cgp.inputs[3] = sim.getTopLeftDistance();
    cgp.inputs[4] = sim.getRightDistance();
    cgp.inputs[5] = sim.getLeftDistance();

    return cgp.akcija();
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

double evaluate(CGP& cgp)
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
        int akcija = run(sim, cgp);
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


/*int imageWidth = 1152;
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

double evaluate(CGP& cgp)
{
    player.setPosition(imageWidth / 2, imageHeight / 1.2);
    player.setRotation(0);

    sf::Vector2f vector = player.getPosition();
    simulator sim(vector.x, vector.y, image);

    while (true) {

        int akcija = run(sim, cgp);
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

        float x = player.getPosition().x;
        float y = player.getPosition().y;

        auto color1 = image.getPixel(x, y);

        if (color1 == sf::Color::Black || sim.getT() > 10000)
        {
            cout << "Angle: " << sim.getAngleDistance() << endl;
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
