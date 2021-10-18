#include "point.h"

int Point::X()const
{
	return this->_x;
}

void Point::X(int x)
{
	this->_x = x;
}

int Point::Y()const
{
	return this->_x;
}

void Point::Y(int y)
{
	this->_x = y;
}

int Point::Id()const
{
	return this->_id;
}

void Point::Id(int Id)
{
	this->_id = Id;
}

Point::Point()
{
}

Point::Point(int X)
{
	this->_x = X;
}

Point::Point(int X, int Y)
{
	this->_x = X;
	this->_y = Y;
}

Point::Point(int X, int Y, int Id)
{
	this->_x = X;
	this->_y = Y;
	this->_id = Id;
}
