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
	panes[pane_count - 1] = std::make_unique<pane>(height, temp);
	for (int i = pane_count - 2; i >= 0; i--) {
		fs::path selected = temp;
		temp = temp.parent_path();
		panes[i] = std::make_unique<pane>(height, temp, selected);
	}
}

pane& view::active_pane() {
	return *panes[pane_count - 2];
}

std::optional<fs::path> view::active_dir() {
	pane& p = view::active_pane();
	if (!p.get_index().has_value()) return {};

	return p.dir_at(p.get_index().value());
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
		panes[0] = std::make_unique<pane>(height);
	} else {
		panes[0] = std::make_unique<pane>(height, temp.parent_path(), temp);
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
		std::optional<int> index = panes[i + 1]->get_index();
		*panes[i] = std::move(*panes[i + 1]);
		panes[i]->set_index(index.value_or(0));
	}

	panes[pane_count - 1] = std::make_unique<pane>(height, path);
}

void view::highlight_previous() {
	pane& p = active_pane();
	if (!p.get_index().has_value()) return;

	// Penultimate pane is the one we navigate in
	p.highlight_previous();

	// Update path
	path = p.dir_at(p.get_index().value());

	// Update child pane
	panes[pane_count - 1] = std::make_unique<pane>(height, path);
}

void view::highlight_next() {
	pane& p = active_pane();
	if (!p.get_index().has_value()) return;

	// Penultimate pane is the one we navigate in
	p.highlight_next();

	// Update path
	path = p.dir_at(p.get_index().value());

	// Update child pane
	panes[pane_count - 1]->set_path(path);
}

void view::jump_to_previous(char c) {
	active_pane().jump_to_previous(c);
}

void view::jump_to_next(char c) {
	active_pane().jump_to_next(c);
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

	std::cout.flush();
}