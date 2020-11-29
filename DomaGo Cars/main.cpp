#include <SFML/Graphics.hpp>
#include <iostream>
#include "simulator.cpp"


//auto se vozi sa w,s,a,d i ako dode do crne pozadine vraca se na pocetak

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "SFML auti", sf::Style::Close | sf::Style::Titlebar);
    //sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(512.0f, 512.0f));
    sf::RectangleShape background(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("background.jpg");
    background.setTexture(&backgroundTexture);

    sf::CircleShape player(30.0f);
    player.setScale(1.5, 1.2);
    player.setPosition(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 1.2);
    //player.rotate(90);
    player.setOrigin(30.0f, 51.0f);
    sf::Texture playerTexture;
    playerTexture.loadFromFile("avatar.jpg");
    player.setTexture(&playerTexture);

    auto image = backgroundTexture.copyToImage(); //jer texture nema getPixel, dimenzije se smanjuju za 5/3

    sf::Vector2f vector = player.getPosition();
    simulator sim(vector.x, vector.y);

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            sim.rotateLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            sim.rotateRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            sim.gas();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            sim.brake();
        }
        else
        {
            sim.idle();
        }

        sim.update();
        player.setPosition(sim.getX(), sim.getY());
        player.setRotation(sim.getAngle() * -1.0);
        //std::cout << sim.getV() << std::endl;

        window.clear();
        window.draw(background);
        window.draw(player);
        window.display();

        float koef = 1.666666666666666666666; //jer copyToImage smanjuje broj piksela

        float x = player.getPosition().x;
        float y = player.getPosition().y;

        x /= koef;
        y /= koef;

        auto color1 = image.getPixel(x, y);
        //auto color2 = image.getPixel(x, y + image.getSize().y);
        //auto color3 = image.getPixel(x + image.getSize().x, y + image.getSize().y);

        if (color1 == sf::Color::Black)
     //|| color2 == sf::Color::Black || color3 == sf::Color::Black)
        {
            //player.setPosition(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 1.2);
            sim.setX(sf::VideoMode::getDesktopMode().width / 2);
            sim.setY(sf::VideoMode::getDesktopMode().height / 1.2);
            sim.setAngle(0);
            player.setRotation(0);
            sim.setV(0);
        }
    }
    return 0;
}
