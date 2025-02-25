#include "pane.h"
#include "dir.h"
#include "tui.h"
#include "color.h"

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

pane::pane(int height, fs::path path) {
	this->height = height;
	this->path = path;

	directories = dir::list_directories(this->path);
	index = -1;
}

pane::pane(int height, fs::path path, fs::path selected, int dbg) {
	this->height = height;
	this->path = path;

	directories = dir::list_directories(this->path);
	index = dir::index_of(directories.begin(), directories.end(), selected);

	update_frame();
}

fs::path pane::get_path() {
	return path;
}

int pane::size() {
	return directories.size();
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

void pane::set_path(fs::path path) {
	directories = dir::list_directories(path);
	index = -1;
	frame = 0;
}

void pane::set_path(fs::path path, fs::path selected) {
	directories = dir::list_directories(path);
	index = dir::index_of(directories.begin(), directories.end(), selected);
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
	std::string s = "";


	if (directories.size() <= frame + line_number) {
		for (int i = 0; i < width; i++) s += " ";
		std::cout << s;
		return;
	}

	std::string name = directories[frame + line_number].filename();
	if (name.size() > width) {
		s += name.substr(0, width);
	} else s += name;

	if (index == selected) tui::set_color(color::fg::black, color::bg::white);
	std::cout << s;
	if (index == selected) tui::reset_color();
	s = "";

	for (int i = 0; i < width - name.size(); i++) s += " ";
	if (s.size() > 0) std::cout << s;
}