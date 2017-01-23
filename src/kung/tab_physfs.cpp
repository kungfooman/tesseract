#include "G:/libs/physfs-2.0.3/physfs.h"
#include <kung/include_imgui.h>

#include <opsys/opsystem_imgui_console.h>
#include <vector>

std::string *dragdrop_file;

static std::vector<std::string> files;
static std::vector<std::string> folders;
static std::vector<std::string> in;

std::string getFullPath() {
	std::string fullpath;
	for (auto tmp : in) {
		fullpath += "/" + tmp;
	}
	return fullpath;
}

void update_files_and_folders() {
	std::string fullpath = getFullPath();
	files.clear();
	folders.clear();
	char **rc;
	rc = PHYSFS_enumerateFiles(fullpath.c_str());
	if (rc == NULL)
		log("Failure. reason: %s.\n", PHYSFS_getLastError());
	else {
		int file_count;
		char **i;
		for (i = rc, file_count = 0; *i != NULL; i++, file_count++) {

			std::string fullpath2 = /*fullpath + "/" +*/ *i;
			if (PHYSFS_isDirectory(fullpath2.c_str()))
				folders.push_back(fullpath2);
			else
				files.push_back(fullpath2);

		}

		PHYSFS_freeList(rc);
	}
}

void draw_hierarchy() {
	int i=0;
	int resizeto = 0;

	if (ImGui::Button("basepath")) {
		in.clear();
		update_files_and_folders();
	}
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
		update_files_and_folders();
	
	}
}

void tab_physfs() {
	
	std::string enterfolder = "";
	static int first = 1;
	if (first) {
		update_files_and_folders();
		first = 0;
	}

	ImGui::Text("Base Dir: %s", PHYSFS_getBaseDir());

	if (ImGui::Button("list all loaded files")) {
		char **rc = PHYSFS_getSearchPath();

		if (rc == NULL)
			log("Failure. reason: %s.\n", PHYSFS_getLastError());
		else {
			int dir_count;
			char **i;
			for (i = rc, dir_count = 0; *i != NULL; i++, dir_count++)
				log("%s\n", *i);

			log("\n total (%d) directories.\n", dir_count);
			PHYSFS_freeList(rc);
		}

	}
	static char filter[256] = {"images"};
	ImGui::InputText("filter", filter, sizeof filter);
	

	draw_hierarchy();
	
	ImGui::Text("Found %d folders:", folders.size());
	for (auto folder : folders) {
		if (ImGui::Button(folder.c_str())) {
			// manipulate "folders" not while iterating over it
			enterfolder = folder;
		}
	}
	ImGui::Text("Found %d files:", files.size());
	for (auto file : files) {
		if (ImGui::Button(file.c_str())) {
			log("load file: %s\n", file.c_str());
		}

		if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0, 1.0f) && dragdrop_file==NULL) {
			// the dragdrop receiver has to free it
			dragdrop_file = new std::string(getFullPath() + "/" + file);
			//log("Drag file: %s\n", dragdrop_file->c_str());
			//dragdrop_file = NULL;
		}
	}


	if (enterfolder != "") {
		in.push_back(enterfolder);
		update_files_and_folders();
	}


	if (ImGui::IsMouseHoveringWindow() && ImGui::IsMouseReleased(0) && dragdrop_file) {
		//log("pphysfs has dragdrop_file: %d\n", dragdrop_file);
		dragdrop_file = NULL;
	}

}