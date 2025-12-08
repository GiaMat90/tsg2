#pragma once

#include <game.h>
#include "arrow.h"
#include "bubble.h"
#include <vector>

#define NUMBER_OF_BUBBLE 20
#define FORCE_INCLUDE_ARROW 1

class arrow_and_bubbles : public game<2, arrow_and_bubbles>
{
public:
	arrow_and_bubbles(const std::string&, const unsigned, const unsigned, const unsigned);
	~arrow_and_bubbles();
public: // override public-methods
	void initialize();
	void run_game();
	void shutdown();
	void initialize_objects();
	void initialize_camera();
	void create_physics();
protected: // override private-methods
	void process_input();
	void update_game();
	void generate_output();
private: // object
	arrow m_arrow;
	std::vector<bubble> m_bubbles;
};

