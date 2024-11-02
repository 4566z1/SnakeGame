#include "GameModel.hpp"
#include "Game.hpp"
#include "Scene.hpp"

#include <WinUser.h>

#include <format>
using std::format;

default_random_engine random_engine;

void Value::on_collide(const char& symbol)
{
	if(symbol == '*')
	{
		const int& score = GameGlobal::Instance().get_var("score");
		GameGlobal::Instance().set_var("score", score + 1);

		Snake& snake = dynamic_cast<Snake&>(m_scene->get_object("snake").value().get());
		snake.add_body();
		move_random();
	}
}

void Value::move_random()
{
	uniform_int_distribution<unsigned> x_random_generate(1,m_scene->get_scene()[0].size() - 2);
	uniform_int_distribution<unsigned> y_random_generate(1,m_scene->get_scene().size() - 2);

	m_x = x_random_generate(random_engine);
	m_y = y_random_generate(random_engine);
}

void Snake::on_collide(const char& symbol)
{
	if(symbol != '@')
	{
		GameGlobal::Instance().set_var("end_flag", true);
	}
}


void Snake::on_update()
{
	if(GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_direction[0] = 0;
		m_direction[1] = -1;
	}
	else if(GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_direction[0] = 0;
		m_direction[1] = 1;
	}
	else if(GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_direction[0] = -1;
		m_direction[1] = 0;
	}
	else if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_direction[0] = 1;
		m_direction[1] = 0;
	}

	move(m_x + m_direction[0] * m_speed, m_y + m_direction[1] * m_speed);
}

void Snake::move(const float& x, const float& y)
{
	// Move head
	float pre_x = m_x, pre_y = m_y;
	m_x = x; m_y = y;

	// Move the snake body and render instantly
	for (const string& part : m_child)
	{
		Object& object = m_scene->get_object(part).value().get();
		const int _buf_x = object.get_x();
		const int _buf_y = object.get_y();
		object.move(pre_x, pre_y);
		pre_x = _buf_x;
		pre_y = _buf_y;
	}
}

void Snake::add_body()
{
	string part_id = format("snake{}", m_child.size() + 1);
	string last_id = m_child.empty() ? m_id : m_child.back();
	Object& last_part = m_scene->get_object(last_id).value().get();

	m_child.push_back(part_id);
	m_scene->add_object(make_unique<SnakePart>(m_scene, m_symbol, part_id));
	m_scene->get_object(part_id).value().get().move(last_part.get_x() - m_direction[0], last_part.get_y() - m_direction[1]);
}
