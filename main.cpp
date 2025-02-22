#include <asm-generic/ioctls.h>
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <filesystem>
#include <sys/ioctl.h>
#include "tui.h"
#include "dir.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
	if (argc < 2 || 2 < argc) {
		std::cout << "Usage: lcd <path>\n";
		return 1;
	}

	// Register CTRL+C handler
	signal(SIGINT, tui::handle_sigint);

	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
		std::cerr << "Error getting terminal size\n";
		return -1;
	}

	tui::raw_mode();
	tui::alt_buffer();

	char c = 0;
	fs::path cwd = dir::get_cwd();

	do {
		std::vector<fs::path> directories = dir::list_directories(cwd);

		tui::clear_screen();

		std::cout << "CWD: " << cwd << "\n";

		// We printed one line already
		for (int i = 0; i < ws.ws_row - 1; i++) {
			if (directories.size() <= i) break;
			std::cout << directories[i] << (i < ws.ws_row - 2 ? "\n" : "");
		}
	} while (read(STDIN_FILENO, &c, 1));

	tui::main_buffer();
	tui::cooked_mode();
}