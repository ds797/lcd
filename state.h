#ifndef VIEW_H
#define VIEW_H

#include "dir.h"
#include "pane.h"
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace state {
	class view {
		// Pane overview:
		// Final pane is for child directories of cwd.
		// Penultimate pane is for directories on the level of cwd. This is the
		// active pane that navigates.
		// Prior panes show parent directories
		int pane_count = 3;
		int width;
		int height;

		std::unique_ptr<std::unique_ptr<pane>[]> panes;
		fs::path path;

		// Disable copy/move semantics
		view(view&);
		view(view&&);
		view& operator=(view&);
		view& operator=(view&&);
	public:
		view(int width, int height, fs::path cwd);
		~view();

		fs::path active_dir();

		void navigate_up();
		void navigate_into();
		void highlight_previous();
		void highlight_next();

		void draw();
	};
}

#endif