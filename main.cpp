#include <asm-generic/ioctls.h>
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <filesystem>
#include <sys/ioctl.h>
#include "tui.h"
#include "dir.h"
#include "color.h"

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
	int index = 0;
	int frame = 0;
	fs::path cwd = dir::get_cwd();

	do {
		if (c == 'q') break;

		std::vector<fs::path> directories;

		int lines_to_draw = ws.ws_row - 1;

		if (c == 'h') {
			fs::path old = cwd;
			cwd = cwd.parent_path();
			directories = dir::list_directories(cwd);
			int i = dir::index_of(directories.begin(), directories.end(), old);
			index = i == -1 ? 0 : i;
		} else directories = dir::list_directories(cwd);
		if (c == 'l') {
			cwd = directories[index];
			directories = dir::list_directories(cwd);
			index = 0;
		}
		if (c == 'j') {
			if (++index == directories.size()) index = 0;
		}
		if (c == 'k') {
			if (--index == -1) index = directories.size() - 1;
		}

		if (index < frame) {
			frame = index;
		} else if (index + 1 > frame + lines_to_draw) {
			frame = index + 1 - lines_to_draw;
		}

		tui::clear_screen();
		std::cout << "CWD: " << cwd << ", current line: " << index + 1 << "\n";

		// We printed one line already
		for (int i = frame; i < frame + lines_to_draw; i++) {
			if (directories.size() <= i) break;

			if (i == index) tui::set_color(color::fg::black, color::bg::white);
			std::cout << directories[i] << (i + 1 < frame + lines_to_draw ? "\n" : "");
			if (i == index) tui::reset_color();

			// Flush the last row
			if (i + 1 == frame + lines_to_draw) std::cout.flush();
		}
	} while (read(STDIN_FILENO, &c, 1));

	tui::main_buffer();
	tui::cooked_mode();
}