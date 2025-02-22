#ifndef DIR_H
#define DIR_H

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class dir {
public:
	static fs::path get_cwd() {
		return fs::current_path();
	}
	static std::vector<fs::path> list_directories(const fs::path& path) {
		std::vector<fs::path> directories;

		try {
			if (!fs::exists(path) || !fs::is_directory(path)) {
				std::cerr << path << " is not a valid directory or does not exist.\n";
			}

			for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
				if (fs::is_directory(entry.status())) {
					directories.push_back(entry);
				}
			}
		} catch (const fs::filesystem_error& e) {
			std::cerr << "Error reading directory: " << e.what() << "\n";
		}

		// Uses move semantics, so it's plenty fast
		return directories;
	}
};

#endif