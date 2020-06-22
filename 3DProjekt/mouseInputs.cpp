#include "mouseInputs.h"

MouseInputs::MouseInputs()
	:event(whichEvent::Move), x(0), y(0)
{
}

MouseInputs::MouseInputs(const whichEvent event, const int x, const int y)
	:event(event), x(x), y(y)
{
}

MouseInputs::~MouseInputs()
{
}


MouseInputs::whichEvent MouseInputs::getEvent() const
{
	return this->event;
}

mousePosition MouseInputs::getPosition() const
{
	return { this->x, this->y };
}

int MouseInputs::getPositionX() const
{
	return this->x;
}

int MouseInputs::getPositionY() const
{
	return this->y;
}
