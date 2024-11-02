#include "Scene.hpp"

#include <Windows.h>

#include <functional>
#include <iostream>
#include <ranges>
using std::views::values;

Scene::Scene()
{
	CONSOLE_SCREEN_BUFFER_INFO console_info;
	CONSOLE_CURSOR_INFO cci;
	m_std_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(m_std_handle, &console_info);
	m_scene.resize(console_info.dwSize.Y);
	for (vector<char>& var : m_scene) var.resize(console_info.dwSize.X);

	// Double buffering
	m_output_buf = CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    cci.bVisible = 0;
    SetConsoleCursorInfo(m_output_buf, &cci);
}

void Scene::detect(Object& object) const
{
	const char& symbol = m_scene[static_cast<size_t>(object.get_y())][static_cast<size_t>(object.get_x())];
	if(symbol != ' ')
	{
		object.on_collide(symbol);
	}
}

void Scene::show() const 
{
	// Needs update;
	DWORD buffer;
	for (int i = get_scene().size() - 1;i >=0;--i)
	{
		WriteConsoleOutputCharacterA(m_output_buf, m_scene[i].data(),
			m_scene[i].size(), { 0, static_cast<short>(i) }, &buffer);
	}
	SetConsoleActiveScreenBuffer(m_output_buf);
}


void Scene::clear()
{
	for(auto i = m_scene.begin();i < m_scene.end();++i)
	{
		if(i == m_scene.begin() || i == m_scene.end() - 1) for (char& c : (*i)) c = '-';
		else
		{
			for (auto j = (*i).begin();j < (*i).end();++j)
			{
				if (j == (*i).begin() || j == (*i).end() - 1)  (*j) = '|';
				else (*j) = ' ';
			}
		}
	}
}

void Scene::update()
{
	clear();
	for(const auto& object : values(m_scene_objects))
	{
		// Only update the father object
		if (!object->get_is_father()) continue;

		object->on_update();

		// Render child first
		if(!object->get_child().empty())
		{
			for(string& child_id : object->get_child())
			{
				Object& child = get_object(child_id).value().get();
				child.on_update();
				detect(child);
				render(child.get_symbol(), static_cast<int>(child.get_x()), static_cast<int>(child.get_y()));
			}
		}

		// Collision detect
		detect(*object);

		// Render the father object
		render(object->get_symbol(), static_cast<int>(object->get_x()), static_cast<int>(object->get_y()));
	}
}

optional<std::reference_wrapper<Object>> Scene::get_object(string_view id)
{
	return *(m_scene_objects[string(id)]);
}

void Scene::remove_object(string_view id)
{
	m_scene_objects.erase(string(id));
}

void Scene::add_object(unique_ptr<Object> object)
{
	m_scene_objects[object->get_id()] = std::move(object);
}

void Scene::render(const char& symbol, const int& x, const int& y)
{
	m_scene[y][x] = symbol;
}