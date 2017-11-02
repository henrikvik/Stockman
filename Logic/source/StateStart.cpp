#include <StateStart.h>

#include <Graphics\include\Renderer.h> // Remove this when merged

using namespace Logic;

StateStart::StateStart()
{

}

StateStart::~StateStart()
{
    printf("Deleted Start");
}

void StateStart::reset()
{

}

void StateStart::update(float deltaTime)
{
    printf("Updating Start menu.");
}

void StateStart::render(Graphics::Renderer& renderer)
{
    
}