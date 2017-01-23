#include "format_iwi.h"
#include "opsystem_hierarchy.h"
#include "include_devil.h"

#include <map>

std::map<std::string, FormatIWI *> loaded_images;



FormatIWI *FormatIWI::getByFilename(char *filename_) {
	auto it = loaded_images.find(filename_);
	if (it != loaded_images.end())
	{
		auto ret = it->second;
		
		log("Reuse textureid %d for %s", ret->textureid, filename_);
		return it->second;
	} else {
		auto ret = new FormatIWI;
		
		strcpy(ret->filename, filename_);

		ret->file_handle = PHYSFS_openRead(ret->filename);
		if (ret->file_handle == NULL) {

			ret->file_handle = PHYSFS_openRead("/images/case.iwi");
			//ret->file_handle = PHYSFS_openRead("/images/default.iwi");
			log("Could not load %s\n", ret->filename);
			//return NULL;
		}
		ret->file_buffer = (char *)malloc(PHYSFS_fileLength(ret->file_handle));
		ret->file_length = PHYSFS_fileLength(ret->file_handle);
		ret->file_length_read = PHYSFS_read (ret->file_handle, ret->file_buffer, 1, ret->file_length);



		ilGenImages(1, &ret->devilID);
		ilBindImage(ret->devilID);
		//ilLoadImage("assets/head.iwi"); // Loads into the current bound image devilError = ilGetError();
		ilLoadL(IL_IWI, ret->file_buffer, ret->file_length);

		iluFlipImage();

		//if (devilError != IL_NO_ERROR) { printf ("Devil Error (ilLoadImage: %s\n", iluGetErrorString (devilError)); exit (2); }

		//IL_OPENGL
		ilutRenderer(ILUT_OPENGL); // Switch the renderer
		//ilugeterr

		//GLuint openglID, openglError;

		ret->textureid = ilutGLBindTexImage(); // This generates the texture for you devilError = ilGetError();

		// cache it
		loaded_images[filename_] = ret;

		return ret;
	}
}


void FormatIWI::imgui() {
	ImGui::Text("Format IWI imgui()");
	ImGui::Text("Format IWI imgui()");
	ImGui::Text("Format IWI imgui()");
	
}