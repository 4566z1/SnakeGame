#include "Game.hpp"
#include "GameModel.hpp"

#include <iostream>
#include <thread>
#include <memory>
#include <format>
using std::format;
using std::cout;
using std::endl;
using std::make_unique;
using std::thread;

void Game::end()
{
	cout << "Game Over" << endl;
}

void Game::work_thread()
{
	while(!GameGlobal::Instance().get_var("end_flag"))
	{
		SetConsoleTitleA(format("Score: {}", GameGlobal::Instance().get_var("score")).c_str());
		m_scene.update();
		m_scene.show();
		Sleep(50);
	}
}

void Game::start()
{
	GameGlobal::Instance().set_var("end_flag", false);
	GameGlobal::Instance().set_var("score", 0);

	m_scene.add_object(make_unique<Snake>(&m_scene, snake_symbol));
	m_scene.add_object(make_unique<Value>(&m_scene, value_symbol, "value"));
	m_scene.add_object(make_unique<Value>(&m_scene, value_symbol, "value1"));
	m_scene.add_object(make_unique<Value>(&m_scene, value_symbol, "value2"));

	thread thread_work(&Game::work_thread, this);

	thread_work.join();

	end();
}