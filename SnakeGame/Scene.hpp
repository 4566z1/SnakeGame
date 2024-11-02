#pragma once

#include "GameModel.hpp"

#include <Windows.h>

#include <optional>
#include <memory>
#include <string>
#include <unordered_map>
using std::optional;
using std::unique_ptr;
using std::unordered_map;
using std::string;
using std::string_view;

class Scene
{
public:
	virtual ~Scene() = default;
	Scene();

	[[nodiscard]] const vector<vector<char>>& get_scene() const { return m_scene; }

	optional<std::reference_wrapper<Object>> get_object(string_view id);
	void add_object(unique_ptr<Object> object);
	void remove_object(string_view id);

	void update();
	void clear();
	void detect(Object& object) const;
	void render(const char& symbol, const int& x, const int& y);
	void show() const;
private:
	HANDLE m_std_handle, m_output_buf;

	vector<vector<char>> m_scene;
	unordered_map<string, unique_ptr<Object>> m_scene_objects;
};