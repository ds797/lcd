#include <string>
#include "pane.h"
#include "tui.h"
#include "color.h"
#include "dir.h"

void pane::update_frame() {
	if (!this->index.has_value()) return;

	int index = this->index.value();

	int last_index = height - 1;
	int frame_start = frame;
	int frame_end = frame + last_index;

	if (index < frame_start) {
		frame = index;
	} if (index > frame_end) {
		frame = index - last_index;
	}
}

pane::pane(int height) {
	this->path = {};
	this->height = height;
	this->directories = std::vector<fs::path>();
	this->index = {};
	this->frame = 0;
}

pane::pane(int height, fs::path path) {
	this->path = path;
	this->height = height;
	this->directories = dir::list_directories(path);
	this->index = {};
	this->frame = 0;
}

pane::pane(int height, fs::path path, fs::path selected) {
	this->path = path;
	this->height = height;
	this->directories = dir::list_directories(path);
	this->index = dir::index_of(directories.begin(), directories.end(), selected);
	this->frame = 0;

	update_frame();
}

int pane::size() {
	return directories.size();
}

std::optional<fs::path> pane::get_path() {
	return this->path;
}

fs::path pane::dir_at(int index) {
	return directories[index];
}

int pane::get_frame() {
	return frame;
}

std::optional<int> pane::get_index() {
	return index;
}

void pane::clear_index() {
	this->index = {};
}

void pane::set_index(int index) {
	this->index = index;
}

void pane::clear_path() {
	this->path = {};
	this->directories = std::vector<fs::path>();
	this->index = {};
	this->frame = 0;
}

void pane::set_path(fs::path path) {
	this->path = path;
	this->directories = dir::list_directories(path);
	this->index = {};
	this->frame = 0;
}

void pane::set_path(fs::path path, int frame, fs::path selected) {
	this->path = path;
	this->directories = dir::list_directories(selected);
	this->index = dir::index_of(directories.begin(), directories.end(), selected);
	this->frame = frame;

	update_frame();
}

void pane::highlight_previous() {
	if (!index.has_value()) return;

	if (--index.value() == -1) index = directories.size() - 1;
	update_frame();
}

void pane::highlight_next() {
	if (!index.has_value()) return;

	if (++index.value() == directories.size()) index = 0;
	update_frame();
}

void pane::jump_to_previous(char c) {
	if (!index.has_value()) return;
	std::optional<int> possible_index = {};

	for (int i = index.value() - 1; i >= 0; i--) {
		if (directories[i].filename().string().starts_with(c)) {
			possible_index = i;
			break;
		}
	}

	// Loop back around
	if (!possible_index.has_value()) {
		for (int i = directories.size() - 1; i > index.value(); i--) {
			if (directories[i].filename().string().starts_with(c)) {
				possible_index = i;
				break;
			}		
		}
	}

	if (possible_index.has_value()) {
		index = possible_index;
		update_frame();
	}
}

void pane::jump_to_next(char c) {
	if (!index.has_value()) return;
	std::optional<int> possible_index = {};

	for (int i = index.value() + 1; i < directories.size(); i++) {
		if (directories[i].filename().string().starts_with(c)) {
			possible_index = i;
			break;
		}		
	}

	// Loop back around
	if (!possible_index.has_value()) {
		for (int i = 0; i < index.value(); i++) {
			if (directories[i].filename().string().starts_with(c)) {
				possible_index = i;
				break;
			}		
		}
	}

	if (possible_index.has_value()) {
		index = possible_index;
		update_frame();
	}
}

void pane::draw_line(int line_number, int width, color::fg selected_foreground, color::bg selected_background) {
	int index = frame + line_number;

	if (directories.size() <= frame + line_number) {
		std::string s = "";
		for (int i = 0; i < width; i++) s += " ";
		std::cout << s;
		return;
	}

	std::string name = directories[frame + line_number].filename();

	if (index == this->index) tui::set_color(selected_foreground, selected_background);
	std::cout << name.substr(0, width);
	if (index == this->index) tui::reset_color();

	std::string s = "";
	// name.size() is a uint
	for (int i = 0; i < width - static_cast<int>(name.size()); i++) s += " ";
	std::cout << s;
}

pane& pane::operator=(pane&& src) {
	if (&src == this) return *this;

	this->directories = std::move(src.directories);
	this->path = src.path;
	this->index = src.index;
	this->frame = src.frame;
	src.path = {};
	src.index = {};
	src.frame = 0;

	return *this;
}