#include <kung/include_imgui.h>
#include "opsystem_imgui_console.h" // log()
#include <vector>
#include "opsystem_explorer.h"

#include <kung/include_duktape.h>

#include <windows.h>
#include <stdio.h>

std::vector<std::string> files_of_dir(std::string folder) {
	HANDLE fh;
	WIN32_FIND_DATA wfd;
	int i;
	std::vector<std::string> ret;

	fh = FindFirstFile((folder+"\\*.*").c_str(), &wfd);
	if (fh != INVALID_HANDLE_VALUE) {
		do {
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				ret.push_back( wfd.cFileName );
			}
		} while (FindNextFile (fh, &wfd));
		FindClose (fh);
	}
	return ret;
}
std::vector<std::string> folders_of_dir(std::string folder) {
	HANDLE fh;
	WIN32_FIND_DATA wfd;
	int i;
	std::vector<std::string> ret;

	fh = FindFirstFile((folder+"\\*.*").c_str(), &wfd);
	if (fh != INVALID_HANDLE_VALUE) {
		do {

			if (wfd.cFileName[0] == '.' && (wfd.cFileName[1] == 0 || wfd.cFileName[1] == '.'))
				continue;
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				ret.push_back( wfd.cFileName );
			}
		} while (FindNextFile (fh, &wfd));
		FindClose (fh);
	}
	return ret;
}

static std::vector<std::string> files;
static std::vector<std::string> folders;
static std::string root = "assets";
static std::vector<std::string> in;


std::string getFullPath(std::string filename) {
	std::string fullpath = root;
	for (auto tmp : in) {
		fullpath += "/" + tmp;
	}
	fullpath += "/" + filename;
	return fullpath;
}

void update_files() {

	std::string fullpath = root;
	for (auto tmp : in) {
		fullpath += "/" + tmp;
	}

	files = files_of_dir(fullpath);
	folders = folders_of_dir(fullpath);
}

void render_explorer() {
	static int first = 1;

	if (first) {
		update_files();
		first = 0;
	}
	if (ImGui::Button("update")) {
		update_files();
	}
	
	std::string enterfolder = "";



	if (ImGui::Button("assets")) {
		in.clear();
		update_files();
	}
	int i=0;
	int resizeto = 0;
	for (auto tmp : in) {
		ImGui::SameLine(0,5);
		if (ImGui::Button(tmp.c_str())) {
			resizeto = i + 1;
		}
		i++;
	}
	// out of loop to prevent crash
	if (resizeto) {
		in.resize(resizeto);
		update_files();
	
	}

	for (auto folder : folders) {
		if (ImGui::Button(folder.c_str())) {
			// manipulate "folders" not while iterating over it
			enterfolder = folder;

		}
	}
	for (auto file : files) {
		if (ImGui::Button(file.c_str()))
		{
		
			
			//log("open file: %s\n", getFullPath(file).c_str());

			js_call("repl_set_filename", "s", getFullPath(file).c_str());
		}
	}


	if (enterfolder != "") {
		in.push_back(enterfolder);
		update_files();
	}

}