#pragma once

#include "Scene.hpp"

#include <unordered_map>
using std::unordered_map;

constexpr char value_symbol = '@';
constexpr char snake_symbol = '*';

class GameGlobal
{
public:
	GameGlobal() = default;

	static GameGlobal& Instance()
	{
		static GameGlobal game_global;
		return game_global;
	}

	[[nodiscard]] const int& get_var(string_view var_name) { return m_hashmap[string(var_name)]; }
	void set_var(string_view var_name, const int& var_content) { m_hashmap[string(var_name)] = var_content; }
private:
	unordered_map<string, int> m_hashmap;
};

class Game
{
public:
	virtual ~Game() = default;
	Game() = default;

	void start();
	void end();
	void work_thread();
private:
	Scene m_scene;
};