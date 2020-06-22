#pragma once

struct mousePosition
{
	int x;
	int y;
};

class MouseInputs
{
public:
	//Ifall vi vill ha fler event senare, 
	//så som mus-klick och scroll-wheel för zoomning
	enum whichEvent
	{
		Move

	};

private:
	whichEvent event;
	int x;
	int y;

public:
	MouseInputs();
	MouseInputs(const whichEvent event, const int x, const int y);
	virtual ~MouseInputs();

	whichEvent getEvent() const;
	mousePosition getPosition() const;
	int getPositionX() const;
	int getPositionY() const;
	


};