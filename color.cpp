#include <string>
#include "color.h"

const color::fg color::fg::black = fg("30");
const color::fg color::fg::black_bold = fg("1;30");
const color::fg color::fg::white = fg("37");

const color::bg color::bg::transparent = bg("39;49");
const color::bg color::bg::black = bg("40");
const color::bg color::bg::white = bg("47");
const color::bg color::bg::white_intense = bg("0;107");

color::fg::fg(std::string escape_sequence) {
	this->escape_sequence = escape_sequence;
}

color::bg::bg(std::string escape_sequence) {
	this->escape_sequence = escape_sequence;
}

color::fg::operator std::string() {
	return escape_sequence;
}
std::string color::fg::string() const {
	return escape_sequence;
}

color::bg::operator std::string() {
	return escape_sequence;	
}
std::string color::bg::string() const {
	return escape_sequence;	
}