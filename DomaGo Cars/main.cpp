#include <SFML/Graphics.hpp>
#include <iostream>
#include "simulator.cpp"


//auto se vozi sa w,s,a,d i ako dode do crne pozadine vraca se na pocetak

int main()
{
    //sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "SFML auti", sf::Style::Close | sf::Style::Titlebar);
    ////sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(512.0f, 512.0f));
    //sf::RectangleShape background(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
    //sf::Texture backgroundTexture;
    //backgroundTexture.loadFromFile("background.jpg");
    //background.setTexture(&backgroundTexture);

    //sf::CircleShape player(30.0f);
    //player.setOrigin(18.0f, 45.0f);
    //player.setScale(1.5, 1.2);
    //player.setPosition(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 1.2);
    //int x1 = player.getPosition().x;
    //int y1 = player.getPosition().y;
    ////player.rotate(90);
    //sf::Texture playerTexture;
    //playerTexture.loadFromFile("avatar.jpg");
    //player.setTexture(&playerTexture);

    //auto image = backgroundTexture.copyToImage(); //jer texture nema getPixel, dimenzije se smanjuju za 5/3

    //std::cout << image.getSize().x << std::endl;
    //std::cout << image.getSize().y << std::endl;

    //sf::Vector2f vector = player.getPosition();
    //simulator sim(vector.x, vector.y, image);
    ////std::cout << sim.getTopBoundDistance(x1, y1, image) << std::endl;
    ////std::cout << sim.getBotBoundDistance(x1, y1, image) << std::endl;
    ////std::cout << sim.getLeftBoundDistance(x1, y1, image) << std::endl;
    ////std::cout << sim.getRightBoundDistance(x1, y1, image) << std::endl;
    ////std::cout << sim.getTopLeftBoundDistance(x1, y1, image) << std::endl;
    ////std::cout << sim.getTopRightBoundDistance(x1, y1, image) << std::endl;
    ////std::cout << sim.getBotLeftBoundDistance(x1, y1, image) << std::endl;
    ////std::cout << sim.getBotRightBoundDistance(x1, y1, image) << std::endl;

    //while (window.isOpen())
    //{
    //    sf::Event evnt;
    //    while (window.pollEvent(evnt))
    //    {
    //        switch (evnt.type)
    //        {
    //        case sf::Event::Closed:
    //            window.close();
    //            break;
    //        case sf::Event::Resized:
    //            std::cout << "New window width: " << evnt.size.width << " New window height: " << evnt.size.height << std::endl;
    //            break;
    //        case sf::Event::TextEntered:
    //            break;
    //        }
    //    }

    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    //    {
    //        sim.rotateLeft();
    //    }
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    //    {
    //        sim.rotateRight();
    //    }
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    //    {
    //        sim.gas();
    //    }
    //    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    //    {
    //        sim.brake();
    //    }
    //    else
    //    {
    //        sim.idle();
    //    }

    //    sim.update(image);
    //    player.setPosition(sim.getX(), sim.getY());
    //    player.setRotation(sim.getAngle() * -1.0);
    //    //std::cout << sim.getV() << std::endl;

    //    window.clear();
    //    window.draw(background);
    //    window.draw(player);
    //    window.display();

    //    float koef = 1.666666666666666666666; //jer copyToImage smanjuje broj piksela

    //    float x = player.getPosition().x;
    //    float y = player.getPosition().y;

    //    x /= koef;
    //    y /= koef;

    //    auto color1 = image.getPixel(x, y);
    //    //auto color2 = image.getPixel(x, y + image.getSize().y);
    //    //auto color3 = image.getPixel(x + image.getSize().x, y + image.getSize().y);

    //    if (color1 == sf::Color::Black)
    //    //|| color2 == sf::Color::Black || color3 == sf::Color::Black)
    //    {
    //        //player.setPosition(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 1.2);
    //        sim.setX(sf::VideoMode::getDesktopMode().width / 2);
    //        sim.setY(sf::VideoMode::getDesktopMode().height / 1.2);
    //        sim.setAngle(0);
    //        player.setRotation(0);
    //        sim.setV(0);
    //    }
    //}
    //return 0;

    sf::RectangleShape background(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("background.jpg");
    background.setTexture(&backgroundTexture);

    sf::CircleShape player(30.0f);
    player.setOrigin(18.0f, 45.0f);
    player.setScale(1.5, 1.2);
    player.setPosition(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 1.2);
    sf::Texture playerTexture;
    playerTexture.loadFromFile("avatar.jpg");
    player.setTexture(&playerTexture);

    auto image = backgroundTexture.copyToImage(); //jer texture nema getPixel, dimenzije se smanjuju za 5/3


    sf::Vector2f vector = player.getPosition();
    simulator sim(vector.x, vector.y, image);

    while (true) {



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

        sim.update(image);
        player.setPosition(sim.getX(), sim.getY());
        player.setRotation(sim.getAngle() * -1.0);
        //std::cout << sim.getV() << std::endl;

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
            
            for (int i = 0; i < 5; i++)
            {
                std::cout << "==========================================" << std::endl;
            }
            std::cout << "SUDAR" << std::endl;
            for (int i = 0; i < 5; i++)
            {
                std::cout << "==========================================" << std::endl;
            }
        }

        std::cout << "Trenutna pozicija: " << sim.getX() << " " << sim.getY() << " Brzina: " << sim.getV() << std::endl;
        std::cout << "Top: " << sim.getTopDistance() << " " <<" Bot: " << sim.getBotDistance() << std::endl;

    }
    return 0;



}
