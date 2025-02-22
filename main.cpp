#include <csignal>
#include <iostream>
#include "tui.h"

int main(int argc, char** argv) {
	if (argc < 2 || 2 < argc) {
		std::cout << "Usage: lcd <path>\n";
		return 1;
	}

	// Register CTRL+C handler
	signal(SIGINT, tui::handle_sigint);

	tui::raw_mode();
	tui::alt_buffer();	

	while (true) {}

	tui::main_buffer();
	tui::cooked_mode();
}