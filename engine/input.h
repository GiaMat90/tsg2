#pragma once
#include "input_object.h"

/* std includes */
#include <vector>
#include <cassert>

enum class input_type {
	keyboard,
	mouse,
	gamepad,
	joystick
};
enum class input_key : int {
	key_esc,
	key_ret,
	key_lctrl,
	key_rctrl,
	key_lshift,
	key_rshift,
	key_ralt,
	key_lalt,
	key_tab,
	key_space,
	key_left,
	key_right,
	key_down,
	key_up,
	key_q,
	key_w,
	key_e,
	key_r,
	key_t,
	key_y,
	key_u,
	key_i,
	key_o,
	key_p,
	key_a,
	key_s,
	key_d,
	key_f,
	key_g,
	key_h,
	key_j,
	key_k,
	key_l,
	key_z,
	key_x,
	key_c,
	key_v,
	key_b,
	key_n,
	key_m,
	mouse_left,
	mouse_right,
	mouse_middle,
	max_key = mouse_middle
};
enum class input_mouse {
	left,
	right,
	middle,
	scroll
};


class input_engine {
public:
	input_engine() = default;
	virtual ~input_engine() = default;
public:
	// keyboard
	virtual bool is_key_pressed(const input_key key) = 0;
	// mouse
	virtual bool is_mouse_clicked(const input_mouse side) = 0;
	virtual bool is_mouse_pressed(const input_mouse side) = 0;
	virtual bool is_mouse_released(const input_mouse side) = 0;
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
	bool is_key_pressed(const input_key key) override { return static_cast<InputImpl*>(this)->is_key_pressed(key); };
	// mouse
	bool is_mouse_clicked(const input_mouse side) override { return static_cast<InputImpl*>(this)->is_mouse_clicked(side); };
	bool is_mouse_pressed(const input_mouse side) override { return static_cast<InputImpl*>(this)->is_mouse_pressed(side); };
	bool is_mouse_released(const input_mouse side) override { return static_cast<InputImpl*>(this)->is_mouse_released(side); };
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
