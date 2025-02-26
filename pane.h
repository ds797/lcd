#ifndef PANE_H
#define PANE_H

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class pane {
	std::vector<fs::path> directories;
	int height;
	int index;
	int frame = 0;
	bool has_index;

	void update_frame();

	// Disable copy/move semantics
	pane(pane&);
	pane(pane&&);
	pane& operator=(pane&);
public:
	pane(int height, std::vector<fs::path> directories);
	pane(int height, std::vector<fs::path> directories, int selected);

	int size();
	fs::path get_path();
	fs::path dir_at(int index);
	int get_index();
	int get_frame();
	std::string info();

	void set_dirs(std::vector<fs::path> directories);
	void set_dirs(std::vector<fs::path> directories, int frame, int selected);

	void highlight_previous();
	void highlight_next();

	void draw_line(int index, int width);

	pane& operator=(pane&&);
};

#endif