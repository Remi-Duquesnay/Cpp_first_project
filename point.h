#pragma once
class Point
{
private :
	int _x;
	int _y;
	int _id;

public :
	int X()const;        
	void X(int X);
	int Y()const;
	void Y(int Y);
	int Id()const;
	void Id(int Id);
	Point();
	Point(int X);
	Point(int X, int Y);
	Point(int X, int Y, int Id);
};

