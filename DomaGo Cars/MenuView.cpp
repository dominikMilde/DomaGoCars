#include "MenuView.h"

void MenuView::init()
{
    application->getWindow()->setFramerateLimit(60);

    const float     windowMiddleWidth = application->getWindowWidth() / 2;
    const float     windowMiddleHeight = application->getWindowHeight() / 2;
    const float     backgroundWidth = 800;
    const float     backgroundHeight = 400;
    const float     backgroundBorderWidth = 15;
    constexpr float backgroundOffset = 33;

    // init background
    outerBackground.setSize(sf::Vector2f(backgroundWidth, backgroundHeight));
    outerBackground.setPosition(windowMiddleWidth - backgroundWidth / 2,
        windowMiddleHeight - backgroundHeight / 2 - backgroundOffset);
    outerBackground.setFillColor(sf::Color::White);
    innerBackground.setSize(
        sf::Vector2f(backgroundWidth - 2 * backgroundBorderWidth, backgroundHeight - 2 * backgroundBorderWidth));
    innerBackground.setPosition(windowMiddleWidth - (backgroundWidth - 2 * backgroundBorderWidth) / 2,
        windowMiddleHeight - (backgroundHeight - 2 * backgroundBorderWidth) / 2 -
        backgroundOffset);
    innerBackground.setFillColor(sf::Color::Black);

    // init title text
    title.setString("DomaGo Cars");
    //title.setFont(fontTitle);
    title.setCharacterSize(130);

    constexpr float titleOffset = 120;
    float           textMiddleWidth = title.getLocalBounds().width / 2;
    float           textHeight = title.getLocalBounds().height;

    title.setPosition(windowMiddleWidth - textMiddleWidth, windowMiddleHeight - textHeight - titleOffset);

    auto button = sfg::Button::Create("Click me");
    button->GetSignal(sfg::Widget::OnLeftClick).Connect([&button] {
        // When the Button is clicked it's label should change.
        printf("radi!");
        });
}

void MenuView::eventHandler(const sf::Event& event)
{

}

void MenuView::draw(sf::RenderWindow& window)
{
    window.draw(outerBackground);
    window.draw(innerBackground);
    window.draw(title);
}