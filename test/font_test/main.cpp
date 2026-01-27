// engine includes
#include <game.h>
// tsg includes
#include <tsg/logger.h>
#include <tsg/io.h>	// print
#include <tsg/os.h>
// std includes
#include <cassert>

class text_render_test : public game<2, text_render_test> {
public:
	text_render_test(const std::string& window_text, const unsigned w, const unsigned h, const unsigned fps) :
		game<2, text_render_test>(window_text, w, h, fps)
	{
		tsg::logger::get_instance().write("text_render_test ctor");
	};
	~text_render_test() {
		delete m_header_font;
		delete m_date_font;
		delete m_time_font;
		tsg::logger::get_instance().write("text_render_test dtor");
	};
	void initialize() {
		tsg::logger::get_instance().write("text_render_test initialize");
		// Initialize camera
		initialize_camera();
		// Create fonts
		m_header_font = create_font();
		m_date_font = create_font();
		m_time_font = create_font();
		m_font = create_font();
		// Set font positions
		constexpr float num_row{ 2.0f };
		font_container header_container{ 
			get_window_width() / std::size_t(4), 
			get_window_width() - (get_window_width() / std::size_t(4)),
			get_window_height() / std::size_t(2) / std::size_t(num_row),
			get_window_height() / std::size_t(2)
		};
		m_header_font->set_container(
			{
				get_window_width() / std::size_t(4),
				get_window_width() - (get_window_width() / std::size_t(4)),
				get_window_height() / std::size_t(2) / std::size_t(num_row),
				get_window_height() / std::size_t(2)
			}
		);
		m_date_font->set_container(
			{
				get_window_width() / std::size_t(4),
				get_window_width() - (get_window_width() / std::size_t(4)),
				std::size_t(2) * get_window_height() / std::size_t(2) / std::size_t(num_row),
				get_window_height() / std::size_t(2)
			}
		);
		m_time_font->set_container(
			{
				get_window_width() / std::size_t(4),
				get_window_width() - (get_window_width() / std::size_t(4)),
				std::size_t(3) * get_window_height() / std::size_t(2) / std::size_t(num_row),
				get_window_height() / std::size_t(2)
			}
		);
		// set aligments
		m_header_font->set_alignmet(font_horizontal_alignment::center, font_vertical_alignment::center);
		m_date_font->set_alignmet(font_horizontal_alignment::center, font_vertical_alignment::center);
		m_time_font->set_alignmet(font_horizontal_alignment::center, font_vertical_alignment::center);
		// Load fonts
		constexpr std::size_t font_size{ 72u };
		m_header_font->load_font(font_size, font_type::carlito, font_style::regular);
		m_date_font->load_font(font_size, font_type::carlito, font_style::regular);
		m_time_font->load_font(font_size, font_type::carlito, font_style::regular);
		// Set font texts
		m_header_font->set_text(m_header);
		m_date_font->set_text(m_date);
		m_time_font->set_text(m_time);
		// add fonts to renderer
		add_font(m_header_font);
		add_font(m_date_font);
		add_font(m_time_font);

		m_state = game_state::running;
	};
	void run_game() {
		tsg::logger::get_instance().write("text_render_test run_game");
		while (game_state::running == m_state) {
			process_input();
			update_game();
			generate_output();
		}
	};
	void shutdown() {
		tsg::logger::get_instance().write("text_render_test shutdown");
		quit();
	};
	void initialize_camera() {
		set_camera_type(camera_type::fixed);
		set_camera_initial_zoom(1.0f);
		camera_init();
	}
	void process_input() {
		if (game_events::quit == get_event()) {
			m_state = game_state::shut_down;
		}
		m_input.process_input();
	};
	void update_game() {
		update();
	};
	void generate_output() {
		render_scene();
	}
private:
	void update() {
		// Nothing to update
		m_date = tsg::date::get_date(tsg::date::TYPE::ONLY_DATE, false).replace_c(' ', '/');
		m_time = tsg::date::get_date(tsg::date::TYPE::ONLY_TIME, false);
		m_date_font->set_text(m_date);
		m_time_font->set_text(m_time);
	}
private:
	const std::string m_header{"Text Render Test"};
	std::string m_date{};
	std::string m_time{};
	font* m_header_font{ nullptr };
	font* m_date_font{ nullptr };
	font* m_time_font{ nullptr };
	font* m_font{ nullptr };
};

int main() {
	text_render_test test("Text Render Test", 800, 600, 60);

	test.initialize();
	test.run_game();
	test.shutdown();

	return 0;
}