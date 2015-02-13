#include "TransformComponent.hpp"
#include <math.h>
#include "../Engine.hpp"

void TransformComponent::move(double x,double y,double s)
{
	Vector2 test(x,y);
	Vector2 distance;
	distance.x = test.x - position.x;
	distance.y = test.y - position.y;
		

	if(distance.normalize())
	{

		double dx = distance.x;
		double dy = distance.y;


		/*
		int angle = position.angle(&test);
		angle = (360 + (angle % 360)) % 360;

		double cos = std::cos(angle);
		double sin = std::sin(angle);
		//*/
		/*
		dx = cos * s;
		dy = sin * s;
		//*/
		/*
		test.normalize();
		dx = test.x * s;
		dy = test.y * s;
		//*/

		position.x += dx*s;
		position.y += dy*s;
	}
	
}
double TransformComponent::distance(TransformComponent* target)
{
	return	position.distance(&target->position);
}
double TransformComponent::angle(TransformComponent* target)
{
	return position.angle(&target->position);
}