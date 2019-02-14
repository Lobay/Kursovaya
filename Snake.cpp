#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;

int size = 525;
int quads_count = 25;
int quad_size = size / quads_count;

struct snake {
	int x;
	int y;
};

struct eat {
	int x;
	int y;
};

void draw(RenderWindow &window, RectangleShape &rect) {
	for (unsigned i = 0; i < quads_count; ++i)
		for (unsigned j = 0; j < quads_count; ++j) {
			rect.setFillColor(Color::Black);
			rect.setPosition(quad_size*i, quad_size*j);
			window.draw(rect);
		}
}

void draw_eat(eat &et, RectangleShape &rect, RenderWindow &window) {
	rect.setFillColor(Color::Red);
	rect.setPosition(et.x * quad_size, et.y * quad_size);
	window.draw(rect);
}

void draw_snake(std::vector<snake> &snk, RectangleShape &rect, RenderWindow &window) {
	for (unsigned i = 0; i < snk.size(); ++i) {
		rect.setFillColor(Color::Blue);
		rect.setPosition(snk[i].x * quad_size, snk[i].y * quad_size);
		window.draw(rect);
	}
}

void snake_generation(std::vector<snake> &snk) {
	for (unsigned i = 0; i < 5; ++i) {
		snake sk{ 13 - i,13 };
		snk.push_back(sk);
	}
}

void eat_generation(std::vector<snake> &snk,eat &et) {
	bool test = true;
	while (test) {
		int x = rand() % 25;
		int y = rand() % 25;
		for (unsigned i = 0; i < snk.size(); ++i) {
			if (et.x == snk[i].x && et.y == snk[i].y)
				continue;
		}
		test = false;
		et.x = x;
		et.y = y;
	}
}

void game_restart(std::vector<snake> &snk,int &time,int &dir,int &last_dir) {
	snk.clear();
	snake_generation(snk);
	time = 200000;
	dir = 3;
	last_dir = 3;
}

void check_aet(std::vector<snake> &snk,eat &et,int &time) {
	if (snk[0].x == et.x && snk[0].y == et.y) {
		snake sk{ snk[snk.size() - 1].x,snk[snk.size() - 1].y };
		snk.push_back(sk);
		eat_generation(snk,et);
		time -= 10000;
	}
}

void tick(std::vector<snake> &snk,eat &et,int &dir,int &time) {
	static int last_dir = 3;

	if (last_dir == 3 && dir == 1)
		dir = 3;
	else if (last_dir == 1 && dir == 3)
		dir = 1;
	else if (last_dir == 0 && dir == 2)
		dir = 0;
	else if (last_dir == 2 && dir == 0)
		dir = 2;

	for (unsigned i = snk.size() - 1; i > 0; --i) {
		snk[i].x = snk[i - 1].x;
		snk[i].y = snk[i - 1].y;
	}
	
	switch (dir)
	{
	case 0:
		if (snk[1].y != snk[0].y - 1)
			if (snk[0].y - 1 >= 0)
				snk[0].y = snk[0].y - 1;
			else
				snk[0].y = 25;
		break;
	case 1:
		if (snk[1].x != snk[0].x - 1)
			if (snk[0].x - 1 >= 0)
				snk[0].x = snk[0].x - 1;
			else
				snk[0].x = 25;
		break;
	case 2:
		if (snk[1].y != snk[0].y + 1)
			if(snk[0].y + 1 < quads_count)
				snk[0].y = snk[0].y + 1;
			else
				snk[0].y = 0;
		break;
	case 3:
		if (snk[1].x != snk[0].x + 1)
			if (snk[0].x + 1 < quads_count)
				snk[0].x = snk[0].x + 1;
			else
				snk[0].x = 0;
		break;
	}

	last_dir = dir;

	check_aet(snk,et,time);

	for (unsigned i = 1; i < snk.size(); ++i) {
		if (snk[0].x == snk[i].x && snk[0].y == snk[i].y) {
			game_restart(snk,time,dir,last_dir);
		}
	}
	
}

int main() {
	RenderWindow window(VideoMode(size, size), "SNAKE");
	RectangleShape rect(Vector2f(quad_size, quad_size));
	rect.setOutlineColor(Color::Black);
	rect.setOutlineThickness(1);

	std::vector<snake> snk;
	eat et;
	snake_generation(snk);
	eat_generation(snk,et);

	int dir = 3;

	int tick_time = 200000;
	Clock clock;

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
			dir = 0;
		else if (Keyboard::isKeyPressed(Keyboard::Left))
			dir = 1;
		else if (Keyboard::isKeyPressed(Keyboard::Down))
			dir = 2;
		else if (Keyboard::isKeyPressed(Keyboard::Right))
			dir = 3;

		if (time > tick_time) {
			tick(snk,et,dir,tick_time);
			clock.restart();
		}

		window.clear();
		draw(window,rect);
		draw_eat(et, rect, window);
		draw_snake(snk, rect, window);
		window.display();
	}

}
