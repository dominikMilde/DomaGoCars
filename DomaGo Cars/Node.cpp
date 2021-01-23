#include "Node.h"

void Node::eventHandler(const sf::Event& event)
{
}

void Node::init()
{
}

void Node::initChild(Application* application, Node* parent)
{
    this->application = application;
    this->parent = parent;
    init();
}

Node* Node::getParent()
{
    return parent;
}