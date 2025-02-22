#ifndef TERM_H
#define TERM_H

#include <cstdlib>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

class tui {
private:
	static termios orig_termios;
public:
	static void alt_buffer() {
		// Hide cursor, save position, enable alternate buffer, move cursor
		std::cout << "\033[?25l\033[s\033[?1049h\033[1;1H";
		std::cout.flush();
	}
	static void main_buffer() {
		// Disable alternate buffer, restore position, and show cursor
		std::cout << "\033[?1049l\033[u\033[?25h"; 
		std::cout.flush();
	}

	// Switch to main screen buffer and cooked mode
	static void handle_sigint(int signal) {
		// Restore terminal
		cooked_mode();
		main_buffer();

		std::cout << "Caught Ctrl+C! Cleaning up...\n";

		exit(0);
	}

	static void raw_mode() {
		tcgetattr(STDIN_FILENO, &orig_termios);

		struct termios raw = orig_termios;

		// Disable echo of input characters (don't show keys pressed)
		// Disable canonical mode (handle input immediately)
		raw.c_lflag &= ~(ECHO | ICANON);

		// Use TCSAFLUSH to avoid processing leftover input
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	}
	static void cooked_mode() {
		// Restore original settings
		struct termios cooked = orig_termios;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &cooked);
	}
};

#endif