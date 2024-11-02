#pragma once

#include <random>
#include <string>
#include <vector>
#include <memory>
using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::string;
using std::string_view;
using std::default_random_engine;
using std::uniform_int_distribution;

class Scene;

class Object
{
public:
	virtual ~Object() = default;
	Object(Scene* scene, const char& symbol, string_view id)
	: m_scene(scene), m_id(id), m_symbol(symbol), m_x(0), m_y(0)
	{}

	virtual void on_create(){}
	virtual void on_update(){}
	virtual void on_collide(const char& symbol){}
	virtual void on_destroy(){}

	virtual void move(const float& x, const float& y) { m_x = x; m_y = y; }
	[[nodiscard]] virtual const string& get_id() const { return m_id; }
	[[nodiscard]] virtual const char& get_symbol() const { return m_symbol; }
	[[nodiscard]] virtual const float& get_x() const { return m_x; }
	[[nodiscard]] virtual const float& get_y() const { return m_y; }
	[[nodiscard]] virtual vector<string>& get_child() { return m_child; }
	[[nodiscard]] virtual bool get_is_father() { return true; }

protected:
	vector<string> m_child;

	Scene* m_scene;
	string m_id;
	char m_symbol;
	float m_x;
	float m_y;
};

class Child : public Object
{
public:
	~Child() override = default;
	Child(Scene* scene, const char& symbol, string_view id)
	: Object(scene, symbol, id)
	{}

	[[nodiscard]] bool get_is_father() override { return false; }
};


class SnakePart : public Child
{
public:
	~SnakePart() override = default;
	SnakePart(Scene* scene, const char& symbol, string_view id)
		: Child(scene, symbol, id)
	{}
};

class Snake : public Object
{
public:
	~Snake() override = default;
	Snake(Scene* scene, const char& symbol, string_view id = "snake")
	: Object(scene, symbol, id)
	{
		m_x = 10;
		m_y = 10;
	}

	[[nodiscard]] const bool& get_is_end() const { return m_is_end; }

	void add_body();

	void on_collide(const char& symbol) override;
	void on_update() override;
	void move(const float& x, const float& y) override;
private:
	bool m_is_end = false;
	float m_direction[2] = { 1.0f, 0.0f };
	float m_speed = 1.0f;
};

class Value : public Object
{
public:
	~Value() override = default;
	Value(Scene* scene, const char& symbol, string_view id)
	: Object(scene, symbol, id)
	{
		move_random();
	}

	void on_collide(const char& symbol) override;

	void move_random();
private:
};