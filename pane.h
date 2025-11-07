#ifndef PANE_H
#define PANE_H

#include <filesystem>
#include <vector>
#include <optional>
#include "color.h"

namespace fs = std::filesystem;

class pane {
	int height;
	std::optional<fs::path> path;
	std::vector<fs::path> directories;
	std::optional<int> index;

	int frame = 0;
	void update_frame();

	// Disable copy/move semantics
	pane(pane&);
	pane(pane&&);
	pane& operator=(pane&);
public:
	pane(int height);
	pane(int height, fs::path path);
	pane(int height, fs::path path, fs::path selected);

	int size();
	std::optional<fs::path> get_path();
	fs::path dir_at(int index);
	int get_frame();
	std::string info();

	std::optional<int> get_index();
	void clear_index();
	void set_index(int index);

	void clear_path();
	void set_path(fs::path path);
	void set_path(fs::path path, int frame, fs::path selected);

	void highlight_previous();
	void highlight_next();

	void jump_to_previous(char);
	void jump_to_next(char);

	void draw_line(int index, int width, color::fg selected_foreground, color::bg selected_background);

	pane& operator=(pane&&);
};

#endif
