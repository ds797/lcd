#include <memory>
#include "state.h"
#include "pane.h"

using namespace state;

view::view(int width, int height, fs::path cwd) {
	// Pointer to a list of pointers to panes
	panes = std::unique_ptr<std::unique_ptr<pane>[]>(new std::unique_ptr<pane>[3]);

	this->width = width;
	this->height = height;

	path = cwd;

	// Initialize panes
	fs::path temp = path;
	std::vector<fs::path> dirs = dir::list_directories(temp);
	panes[pane_count - 1] = std::make_unique<pane>(height, dirs);
	for (int i = pane_count - 2; i >= 0; i--) {
		fs::path p = temp;
		temp = temp.parent_path();
		dirs = dir::list_directories(temp);
		int selected = dir::index_of(dirs.begin(), dirs.end(), p);
		panes[i] = std::make_unique<pane>(height, dirs, selected);
	}
}

fs::path view::active_dir() {
	pane& active_pane = *panes[pane_count - 2];
	return active_pane.dir_at(active_pane.get_index());
}

void view::navigate_up() {
	int root_index = -1;
	for (int i = pane_count - 1; i >= 0; i--) {
		if (panes[i]->get_path() == "/") {
			root_index = i;
			break;
		}
	}
	// Can't navigate up a directory, we're already at the root
	if (root_index == pane_count - 2) return;

	// Save current path in temp
	fs::path temp = path;
	path = path.parent_path();

	// Update panes
	for (int i = pane_count - 1; i >= 1; i--) {
		temp = path.parent_path();
		*panes[i] = std::move(*panes[i - 1]);
	}

	if (root_index != -1) {
		std::vector<fs::path> empty;
		panes[0] = std::make_unique<pane>(height, empty);
	} else {
		std::vector<fs::path> dirs = dir::list_directories(temp.parent_path());
		int selected = dir::index_of(dirs.begin(), dirs.end(), temp);

		panes[0] = std::make_unique<pane>(height, dirs, selected);
	}
}

void view::navigate_into() {
	// Can't navigate into directory, nothing there
	if (panes[pane_count - 1]->size() == 0) return;

	path = panes[pane_count - 1]->dir_at(0);

	// Update panes
	fs::path temp = path;
	for (int i = 0; i < pane_count - 1; i++) {
		temp = path.parent_path();
		*panes[i] = std::move(*panes[i + 1]);
	}

	std::vector<fs::path> dirs = dir::list_directories(path);
	panes[pane_count - 1] = std::make_unique<pane>(height, dirs);
}

void view::highlight_previous() {
	// Penultimate pane is the one we navigate in
	panes[pane_count - 2]->highlight_previous();

	// Update path
	path = panes[pane_count - 2]->dir_at(panes[pane_count - 2]->get_index());

	// Update child pane
	std::vector<fs::path> dirs = dir::list_directories(path);
	panes[pane_count - 1] = std::make_unique<pane>(height, dirs);
}

void view::highlight_next() {
	// Penultimate pane is the one we navigate in
	panes[pane_count - 2]->highlight_next();

	pane& active_pane = *panes[pane_count - 2];

	// Update path
	path = active_pane.dir_at(active_pane.get_index());

	// Update child pane
	std::vector<fs::path> dirs = dir::list_directories(path);
	panes[pane_count - 1]->set_dirs(dirs);
}

void view::draw() {
	int width_without_separators = width - (pane_count - 1);
	int pane_width = width_without_separators / pane_count;
	int main_width = width_without_separators - (pane_count - 1) * pane_width;

	for (int i = 0; i < height; i++) {
		for (int p = 0; p < pane_count; p++) {
			int width = (p == (pane_count - 2)) ? main_width : pane_width;
			panes[p]->draw_line(i, width);
			if (p < pane_count - 1) std::cout << "│";
		}
		std::cout << ((i < height - 1) ? "\n" : "");
	}








	// 	std::string parent = (parent_dirs[parent_frame + i]).string();
	// 	std::string current = (current_dirs[current_frame + i]).string();
	// 	bool parent_selected = (parent_frame + i) == parent_index;
	// 	bool current_selected = (current_frame + i) == current_index;

	// 	std::string s = "";
	// 	int parent_padding = column_width - parent.size();
	// 	int parent_end_index = parent_padding < 0 ? column_width : parent.size();
	// 	s += parent.substr(0, parent_end_index);
	// 	for (int i = 0; i < parent_padding; i++) s += " ";

	// 	int current_padding = column_width - parent.size();
	// 	int current_end_index = parent_padding < 0 ? column_width : parent.size();
	// 	s += parent.substr(0, current_end_index);
	// 	for (int i = 0; i < current_padding; i++) s += " ";

	// 	for (int i = 0; i < column_width; i++) s += " ";

	// 	std::cout << s << (i < (height - 1) ? "\n" : "");
	// }
	std::cout.flush();
}