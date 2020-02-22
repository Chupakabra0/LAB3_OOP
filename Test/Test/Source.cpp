#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "ShapeDealer.h"
#include <iostream>
#include "Circle.h"

#include <string>
#include <sstream>

using namespace sf;

enum figureID {
	CIRCLE
};

struct Const {
	Const() = delete;
	~Const() = delete;
	constexpr static float SPEED = 1000.f;
	constexpr static float TIME = 500.f;
	constexpr static float MOVE = 3.f;
	constexpr static float ANGLE = 0.1f;
	constexpr static float SCALE_PLUS = 1.f;
	constexpr static float SCALE_MINUS = -1.f;

	static void Key(std::vector<IShape*>& shapes, Event& event, float& time, unsigned int& focus, unsigned int& id) {
		if (time >= Const::TIME) {
			if (!shapes.empty() && event.type == Event::KeyPressed) {
				if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
					ShapeDealer::Move(dynamic_cast<IMove*>(shapes[focus]), XY(-MOVE, 0.f));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
					ShapeDealer::Move(dynamic_cast<IMove*>(shapes[focus]), XY(MOVE, 0.f));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
					ShapeDealer::Move(dynamic_cast<IMove*>(shapes[focus]), XY(0.f, -MOVE));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
					ShapeDealer::Move(dynamic_cast<IMove*>(shapes[focus]), XY(0.f, MOVE));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::E)) {
					ShapeDealer::Rotate(dynamic_cast<IRotate*>(shapes[focus]), Angle(-ANGLE));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::Q)) {
					ShapeDealer::Rotate(dynamic_cast<IRotate*>(shapes[focus]), Angle(ANGLE));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::W)) {
					ShapeDealer::Zoom(dynamic_cast<IScale*>(shapes[focus]), Scale(SCALE_PLUS));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::S)) {
					ShapeDealer::Zoom(dynamic_cast<IScale*>(shapes[focus]), Scale(SCALE_MINUS));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::Z)) {
					ShapeDealer::LegacyCondition(dynamic_cast<IShape*>(shapes[focus]));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::X)) {
					ShapeDealer::FirstCondition(dynamic_cast<IShape*>(shapes[focus]));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::PageDown)) {
					ShapeDealer::SwitchFocus(dynamic_cast<Figure*>((shapes[focus])));
					if (focus != 0) focus--;
					else focus = shapes.size() - 1;
					ShapeDealer::SwitchFocus(dynamic_cast<Figure*>((shapes[focus])));
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::PageUp)) {
					ShapeDealer::SwitchFocus(dynamic_cast<Figure*>((shapes[focus])));
					if (focus != shapes.size() - 1) focus++;
					else focus = 0;
					ShapeDealer::SwitchFocus(dynamic_cast<Figure*>((shapes[focus])));
				}
			}
			else if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Key::Add) {
					std::vector<float> coordinates;
					CreateFigure(shapes, coordinates, focus, id);
				}
			}
			time = 0.f;
		}
	}

	static void Text(std::vector<IShape*>& shapes, std::string& string, const Event& event, unsigned int& focus, unsigned int& id) {
		if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode == '\b') {
				if (!string.empty()) string.erase(string.cend() - 1);
			}
			else if (event.text.unicode == 13) Check(shapes, string, focus, id);
			else if (event.text.unicode != 43) string += {static_cast<char>(event.text.unicode)};
		}
	}

private:
	static void Check(std::vector<IShape*>& shapes, std::string& string, unsigned int& focus, unsigned int& id) {
		if (!string.find("Create")) {
			if (!string.find(typeid(Circle).name()))
			string = NumberCheck(string);
			std::replace(string.begin(), string.end(), '_', ' ');
			std::istringstream ss(string);
			std::vector<float> coordinates{ std::istream_iterator<float>(ss), {} };
			//TODO CREATE
			CreateFigure(shapes, coordinates, focus, id);
			
		}
		else {
			
		}
		string.erase();
	}

	static std::string NumberCheck(std::string& string) {
		std::string result;
		const auto size = string.size();
		for (auto i = 0; i < size; i++) {
			if (string[i] == '_' || (string[i] >= '0' && string [i] <= '9')){
				result += string[i];
			}
		}
		return result;
	}

	static void CreateFigure(std::vector<IShape*>& shapes, std::vector<float>& coordinates, unsigned int& focus, unsigned int& id) {
		if (!shapes.empty()) ShapeDealer::SwitchFocus(dynamic_cast<Figure*>((shapes[focus])));
		Figure* temp = nullptr;
		switch (id) {
			case CIRCLE: {
				XY xy;
				Color color;
				float radius = 10.f;
				if (coordinates.empty()) {
					xy.setXY(0.f, 0.f);
					color = Color::Red;
				}
				else if (coordinates.size() == 1) {
					xy.setXY(coordinates[0], coordinates[0]);
					color = Color::Red;
				}
				else if (coordinates.size() == 2) {
					xy.setXY(coordinates[0], coordinates[1]);
					color = Color::Red;
				}
				else if (coordinates.size() == 3 || coordinates.size() == 4 || coordinates.size() == 5) {
					xy.setXY(coordinates[0], coordinates[1]);
					radius = coordinates[2];
					color = Color::Red;
				}
				else if (coordinates.size() >= 6) {
					xy.setXY(coordinates[0], coordinates[1]);
					radius = coordinates[2];
					color = Color(coordinates[3], coordinates[4], coordinates[5]);
				}
				temp = new Circle(XY(xy.getX(), xy.getY()), color, radius);
				break;
			}
			default: {
				break;
			}
		}
		shapes.push_back(dynamic_cast<IDraw*>(temp));
		focus = shapes.size() - 1;
		ShapeDealer::SwitchFocus(dynamic_cast<Figure*>((shapes[focus])));
	}
};

int main(void) {

	RenderWindow window{ VideoMode(600, 600), L"�������������� ������" };

	std::vector<IShape*> shapes;
	Clock clock;
	std::string cmd;
	unsigned int focus = 0;
	unsigned int id = CIRCLE;

	while (window.isOpen()) {

		auto time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		time /= Const::SPEED;

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();

			Const::Key(shapes, event, time, focus, id);
			Const::Text(shapes, cmd, event, focus, id);
		}

		window.clear();
		for (auto i = 0; i < shapes.size(); i++) {
			std::cout << i + 1 << std::endl;
			ShapeDealer::Draw(dynamic_cast<IDraw*>(shapes[i]), window);
		}

		//
		Text text;
		Font font;
		font.loadFromFile("BAUHS93.ttf");
		text.setString(cmd);
		text.setFont(font);
		window.draw(text);
		//


		window.display();
	}

	return EXIT_SUCCESS;
}