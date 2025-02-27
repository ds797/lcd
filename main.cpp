#include <asm-generic/ioctls.h>
#include <csignal>
#include <filesystem>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include "state.h"
#include "tui.h"
#include "dir.h"

int main(int argc, char** argv) {
	if (2 < argc) {
		std::cout << "Usage: lcd [path]\n";
		return 1;
	} else if (argc == 2 && !fs::exists(argv[1])) {
		std::cout << "Invalid path\n";
		return -1;
	}

	// Register CTRL+C handler
	signal(SIGINT, tui::handle_sigint);

	struct winsize ws;
	int tty = open("/dev/tty", O_RDWR);
	if (ioctl(tty, TIOCGWINSZ, &ws) == -1) {
		std::cerr << "Error getting terminal size\n";
		return -1;
	}

	// Save original STDOUT
	int stdout = dup(STDOUT_FILENO);
	// Redirect STDOUT to the tty
	dup2(tty, STDOUT_FILENO);
	close(tty);

	tui::raw_mode();
	tui::alt_buffer();

	fs::path initial_path = argc == 2 ? std::filesystem::absolute(argv[1]) : dir::get_cwd();

	state::view* state = new state::view(
		ws.ws_col,
		// We printed one line already
		ws.ws_row - 1,
		initial_path
	);

	std::optional<char> search = {};

	char c = 0;
	do {
		if (c == 'q') break;

		if (c == 'h') state->navigate_up();
		if (c == 'l') state->navigate_into();
		if (c == 'j') state->highlight_next();
		if (c == 'k') state->highlight_previous();

		if (c == 'N') {
			if (search.has_value()) state->jump_to_previous(search.value());
		}
		if (c == 'n') {
			if (search.has_value()) state->jump_to_next(search.value());
		}

		std::optional<fs::path> active_dir = state->active_dir();

		if (c == '\r' || c == '\n') {
			tui::main_buffer();
			tui::cooked_mode();

			// Revert STDOUT
			dup2(stdout, STDOUT_FILENO);
			close(stdout);

			if (active_dir.has_value()) {
				std::cout << "Switching working directory to " << active_dir.value() << "...\n";
			}
			return 0;
		}

		tui::clear_screen();
		if (c == 'f') {
			std::cout << "Enter your search character\n";
			state->draw();

			char t = 0;
			read(STDIN_FILENO, &t, 1);
			search = t;
			state->jump_to_next(search.value());
			active_dir = state->active_dir();
		}

		if (active_dir.has_value()) {
			std::cout << active_dir.value() << "\n";
		} else {
			std::cout << "No active directory" << "\n";
		}

		state->draw();
	} while (read(STDIN_FILENO, &c, 1));

	tui::main_buffer();
	tui::cooked_mode();
}