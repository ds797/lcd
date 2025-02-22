#include <csignal>
#include <iostream>
#include "tui.h"

int main(int argc, char** argv) {
	if (argc < 2 || 2 < argc) {
		std::cout << "Usage: lcd <path>\n";
		return 1;
	}

	signal(SIGINT, tui::handle_sigint);

	tui::alt_buffer();	
	tui::raw_mode();

	while (true) {}

	tui::cooked_mode();
	tui::main_buffer();
}