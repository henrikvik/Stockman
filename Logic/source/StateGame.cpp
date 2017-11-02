#include <StateGame.h>

#include <Graphics\include\Renderer.h> // Remove this when merged

using namespace Logic;

StateGame::StateGame()
{

}

StateGame::~StateGame()
{
    printf("Deleted Game");
}

void StateGame::reset()
{

}

void StateGame::update(float deltaTime)
{
    printf("Updating Game menu.");
}

void StateGame::render(Graphics::Renderer & renderer)
{

}