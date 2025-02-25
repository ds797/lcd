#include <asm-generic/ioctls.h>
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include "state.h"
#include "tui.h"
#include "dir.h"

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

	state::view* state = new state::view(
		ws.ws_col,
		// We printed one line already
		ws.ws_row - 1,
		dir::get_cwd()
	);

	do {
		if (c == 'q') break;

		if (c == 'h') {
			state->navigate_up();
		}
		if (c == 'l') {
			state->navigate_into();
		}
		if (c == 'j') {
			state->highlight_next();
		}
		if (c == 'k') {
			state->highlight_previous();
		}

		tui::clear_screen();
		std::cout << state->active_dir() << "\n";

		state->draw();
	} while (read(STDIN_FILENO, &c, 1));

	tui::main_buffer();
	tui::cooked_mode();
}