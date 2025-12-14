#pragma once
#include "input_object.h"

/* std includes */
#include <vector>
#include <cassert>

enum class INPUT_TYPE {
	KEYBOARD,
	MOUSE,
	GAMEPAD,
	JOYSTICK
};
enum class INPUT_KEY : int {
	KEY_ESC,
	KEY_RET,
	KEY_LCTRL,
	KEY_RCTRL,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_RALT,
	KEY_LALT,
	KEY_TAB,
	KEY_SPACE,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_UP,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MAX_KEY = MOUSE_MIDDLE
};
enum class INPUT_MOUSE {
	LEFT,
	RIGHT,
	MIDDLE,
	SCROLL
};


class input_engine {
public:
	input_engine() = default;
	virtual ~input_engine() = default;
public:
	// keyboard
	virtual bool is_key_pressed(const INPUT_KEY key) = 0;
	// mouse
	virtual bool is_mouse_clicked(const INPUT_MOUSE side) = 0;
	virtual bool is_mouse_pressed(const INPUT_MOUSE side) = 0;
	virtual bool is_mouse_released(const INPUT_MOUSE side) = 0;
	virtual void get_mouse_position(float& x, float& y) = 0;
};

template <typename WindowImpl, typename CameraImpl, typename InputImpl>
class input : public input_engine {
public:
	input(WindowImpl * const w, CameraImpl * const c) : input_engine(), m_window(w), m_camera(c) {
		assert(w);
		assert(c);
	};
	virtual ~input() = default;
public:
	inline void process_input() {
		m_camera->update_camera(this);
		for (auto p : m_playables) {			
			p->process_input(this);
		}
	}
public:
	// kind of inputs events
	// keyboard
	bool is_key_pressed(const INPUT_KEY key) override { return static_cast<InputImpl*>(this)->is_key_pressed(key); };
	// mouse
	bool is_mouse_clicked(const INPUT_MOUSE side) override { return static_cast<InputImpl*>(this)->is_mouse_clicked(side); };
	bool is_mouse_pressed(const INPUT_MOUSE side) override { return static_cast<InputImpl*>(this)->is_mouse_pressed(side); };
	bool is_mouse_released(const INPUT_MOUSE side) override { return static_cast<InputImpl*>(this)->is_mouse_released(side); };
	void get_mouse_position(float& x, float& y) override { return static_cast<InputImpl *>(this)->get_mouse_position(x, y); };
	// gamepad
	/* TODO */
	// joystick
	/* TODO */
public:
	// TODO: evalueate to made it private and friendable of game
	inline void add_playable(playable_object* d) {
		m_playables.push_back(d);
	}
protected:
	WindowImpl* m_window{ nullptr };
	CameraImpl* m_camera{ nullptr };
	std::vector<playable_object*> m_playables;

};
