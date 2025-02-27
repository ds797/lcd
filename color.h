#ifndef COLOR_H
#define COLOR_H


#include <string>

namespace color {
	class fg {
		std::string escape_sequence;
		fg(std::string escape_sequence);
	public:
		static const color::fg black;
		static const color::fg black_bold;
		static const color::fg white;

		operator std::string();
		std::string string() const;
	};

	class bg {
		std::string escape_sequence;
		bg(std::string escape_sequence);
	public:
		static const color::bg transparent;
		static const color::bg black;
		static const color::bg white;
		static const color::bg white_intense;

		operator std::string();
		std::string string() const;
	};
}

#endif