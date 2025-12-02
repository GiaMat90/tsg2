#include <tsg/io.h>
#include "arrow_and_bubbles.h"

int main() {
	tsg::print("Arrow and Bubbles starts");
	try {
		arrow_and_bubbles g("Arrow and Bubbles.exe", 1024, 768, 60);
		g.initialize();
		g.run_game();
		g.shutdown();
	}
	catch (std::exception& e) {
		tsg::print(e.what());
	}
	catch (...) {
		tsg::print("Unkown exception occured");
	}
	tsg::print("Arrow and Bubles ends");
}