#include "pane.h"
#include "tui.h"
#include "color.h"
#include <string>

void pane::update_frame() {
	int last_index = height - 1;
	int frame_start = frame;
	int frame_end = frame + last_index;

	if (index < frame_start) {
		frame = index;
	} if (index > frame_end) {
		frame = index - last_index;
	}
}

pane::pane(int height, std::vector<fs::path> directories) {
	this->height = height;
	this->directories = directories;
	this->index = 0;
	this->frame = 0;
	this->has_index = false;
}

pane::pane(int height, std::vector<fs::path> directories, int selected) {
	this->height = height;
	this->directories = directories;
	this->index = selected;
	this->frame = 0;
	this->has_index = true;

	update_frame();
}

std::string pane::info() {
	std::string s = "";

	if (directories.size() > 0) {
		s += "Curdir: ";
		s += directories[index];
	} else {
		s += "Contains no directories";
	}

	s += ", index: ";
	s += std::to_string(index);
	s += "!";

	return s;
}

int pane::size() {
	return directories.size();
}

fs::path pane::get_path() {
	return directories[this->index].parent_path();
}

fs::path pane::dir_at(int index) {
	return directories[index];
}

int pane::get_index() {
	return index;
}

int pane::get_frame() {
	return frame;
}

void pane::set_dirs(std::vector<fs::path> directories) {
	this->directories = directories;
	this->index = 0;
	this->frame = 0;
}

void pane::set_dirs(std::vector<fs::path> directories, int frame, int selected) {
	this->directories = directories;
	this->frame = frame;
	this->index = selected;

	update_frame();
}

void pane::highlight_previous() {
	if (--index == -1) index = directories.size() - 1;
	update_frame();
}

void pane::highlight_next() {
	if (++index == directories.size()) index = 0;
	update_frame();
}

void pane::draw_line(int line_number, int width) {
	int selected = this->index;
	int index = frame + line_number;

	if (directories.size() <= frame + line_number) {
		std::string s = "";
		for (int i = 0; i < width; i++) s += " ";
		std::cout << s;
		return;
	}

	std::string name = directories[frame + line_number].filename();

	if (index == selected) tui::set_color(color::fg::black, color::bg::white);
	std::cout << name.substr(0, width);
	if (index == selected) tui::reset_color();

	std::string s = "";
	// name.size() is a uint
	for (int i = 0; i < width - static_cast<int>(name.size()); i++) s += " ";
	std::cout << s;
}

pane& pane::operator=(pane&& src) {
	if (&src == this) return *this;

	this->directories = std::move(src.directories);
	if (this->has_index) {
		this->index = src.index;
		this->frame = src.frame;
	} else {
		this->index = 0;
		this->frame = 0;
	}
	src.index = 0;
	src.frame = 0;

	return *this;
}