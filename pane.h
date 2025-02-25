#ifndef PANE_H
#define PANE_H

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class pane {
	fs::path path;
	std::vector<fs::path> directories;
	int height;
	int index;
	int frame = 0;

	void update_frame();

public:
	pane(int height, fs::path path);
	pane(int height, fs::path path, fs::path selected, int dbg);

	fs::path get_path();
	int size();
	fs::path dir_at(int index);
	int get_index();
	int get_frame();

	void set_path(fs::path path);
	void set_path(fs::path path, fs::path selected);

	void highlight_previous();
	void highlight_next();

	void draw_line(int index, int width);
};

#endif