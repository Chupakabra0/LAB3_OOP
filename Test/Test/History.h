#pragma once
#include <utility>
#include "XY.h"
#include "Interfaces.h"

class History {
public:
	History() = delete;
	explicit History(XY dot, Scale scale, Angle angle, sf::Color color) : dot(dot), scale(std::move(scale)), angle(angle), color(color) {}
	//---------------------------------------------------------------------
	XY GetDot() const;
	Scale GetScale() const;
	Angle GetAngle() const;
	sf::Color GetColor() const;
protected:
	XY dot;
	Scale scale;
	Angle angle;
	sf::Color color;
};
