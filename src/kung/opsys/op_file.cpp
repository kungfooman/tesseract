#include "op.h"
#include "op_file.h"

#include <kung/include_imgui.h>
#include "include_soil.h"
#include "include_devil.h"
#include "include_physfs.h"

#define SHORTNAME "File"
#define CLASSNAME OpFile
#define METHOD(name) op_file_##name
#define STRUCTNAME struct op_file_s

#define WIDTH  ((op->width))
#define HEIGHT ((op->height))
#define AND_I  ((op->and_i))
#define AND_J  ((op->and_j))


void METHOD(makeCheckImage)(CLASSNAME *op)
{
	//int i, j, c;
	//
	//if ( ! op->checkImage)
	//return;
	//
	//GLubyte *img = op->checkImage;
	//
	//for (i = 0; i < HEIGHT; i++) {
	//	for (j = 0; j < WIDTH; j++) {
	//		c = ((((i & AND_I)==0)^((j & AND_J))==0))*255;
	//		*img++ = (GLubyte) c;
	//		*img++ = (GLubyte) c;
	//		*img++ = (GLubyte) c;
	//		*img++ = (GLubyte) 255;
	//	}
	//}
}

void METHOD(generate_texture)(CLASSNAME *op) {
	
   if ( ! op->checkImage)
	   return;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, (GLuint *)&op->textureid);
	//glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture
	glBindTexture(GL_TEXTURE_2D, op->textureid);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, op->checkImage);

}

void METHOD(init)(CLASSNAME *op) {

	op_init((Op *)op);
	op->shortname = SHORTNAME;
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 250);
	strcpy( op->name, SHORTNAME );
	op->checkImage = NULL;

	op_init_link((Op *)op, 0, "Width" , OP_TYPE_INT );
	op_init_link((Op *)op, 1, "Height", OP_TYPE_INT );
	op_init_link((Op *)op, 2, "And I" , OP_TYPE_INT );
	op_init_link((Op *)op, 3, "And J" , OP_TYPE_INT );

	op->number_of_inputs = 0;


	
	op->type = OpType::OP_TYPE_SENDER;
	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	
	ImGui::SetCursorPos(op->pos);
	ImGui::Image((ImTextureID)op->textureid, op->size/* + ImVec2(-20,-20)*/);
	
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	if ( ! op_did_inputs_change((Op *)op) )
		return;

	//if (op->checkImage)
	//	free(op->checkImage);
	//op->checkImage = (GLubyte *)malloc(WIDTH * HEIGHT * 4);
	//METHOD(makeCheckImage)(op);
	//glDeleteTextures(1, (const GLuint *)&op->textureid);
	//METHOD(generate_texture)(op);

	op_set_inputs_unchanged((Op *)op);
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
	ImGui::Text("address: %x", op->checkImage);
	ImGui::Text("textureid: %x", op->textureid);
	
	if (ImGui::Button("Make DDS")) {
		/* load another image, but into the same texture ID, overwriting the last one */
		op->textureid = SOIL_load_OGL_texture
		(
			"assets/head.iwi_out.dds",
			SOIL_LOAD_AUTO,
			op->textureid,
			SOIL_FLAG_DDS_LOAD_DIRECT
		);	
	}


	//if (ImGui::Button("Make .iwi")) {
	//
	//	ILuint devilError;
	//	//ilInit();
	//	devilError = ilGetError();
	//	//if (devilError != IL_NO_ERROR) { printf ("Devil Error (ilInit: %s\n", iluGetErrorString (devilError)); exit (2); }
	//
	//
	//	
	//	ilGenImages(1, &op->devilID);
	//	ilBindImage(op->devilID);
	//	//ilLoadImage("assets/head.iwi"); // Loads into the current bound image devilError = ilGetError();
	//	ilLoadL(IL_IWI, op->file_buffer, op->file_length);
	//
	//	iluFlipImage();
	//
	//	//if (devilError != IL_NO_ERROR) { printf ("Devil Error (ilLoadImage: %s\n", iluGetErrorString (devilError)); exit (2); }
	//
	//	//IL_OPENGL
	//	ilutRenderer(ILUT_OPENGL); // Switch the renderer
	//	//ilugeterr
	//
	//	//GLuint openglID, openglError;
	//
	//	op->textureid = ilutGLBindTexImage(); // This generates the texture for you devilError = ilGetError();
	//
	//	//if (devilError != IL_NO_ERROR) { printf ("Error: %s\n", iluGetErrorString (devilError)); exit (2); }
	//
	//	//if (openglError != GL_NO_ERROR) { printf ("Opengl Error (ilutGLBindTexImage): %s\n", gluGetErrorString (openglError)); exit (2); }
	//
	//	// Make sure to close the image when you are done with it (though DevIL // automatically deletes them when the program exits):
	//
	//	//glDeleteTextures(1, &openglID); ilDeleteImages (1, &devilID);
	//
	//
	//}

	
	ImGui::Text("opengl textureid: %d", op->textureid);
	//ImGui::Text("devilID: %d", op->devilID);
	ImGui::Text("filename: %s", op->filename);
	ImGui::Text("file_handle: %d", op->file_handle);
	ImGui::Text("file_buffer: %d", op->file_buffer);
	ImGui::Text("file_length: %d", op->file_length);
	ImGui::Text("file_length_read: %d", op->file_length_read);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	fprintf(f, "filename %s",
		op->filename
	);
}

// http://stackoverflow.com/questions/5309471/getting-file-extension-in-c
const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

#include "op_prefab.h"

// meh, restore defines... should convert the system back to C++ at some point

#include "opsystem.h"
#define SHORTNAME "File"
#define CLASSNAME OpFile
#define METHOD(name) op_file_##name
#define STRUCTNAME struct op_file_s

#include "format_cod2_xmodel.h"

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	sscanf(line, "filename %s",
		op->filename
	);
	log("Create file: %s\n", line);

	op->file_handle = PHYSFS_openRead(op->filename);
	op->file_buffer = (char *)malloc(PHYSFS_fileLength(op->file_handle));
	op->file_length = PHYSFS_fileLength(op->file_handle);
	op->file_length_read = PHYSFS_read (op->file_handle, op->file_buffer, 1, op->file_length);
	log("Read file %s length=%d length_read=%d\n", op->filename, op->file_length, op->file_length_read);


	const char *extension = get_filename_ext(op->filename);
	if (strcmp(extension, "iwi") == 0) {
		op->format_iwi = FormatIWI::getByFilename(op->filename);
		op->textureid = op->format_iwi->textureid;
	}

	
	if (strcmp(extension, "opsys") == 0) {

		OpPrefab *prefab = op_prefab_new();
		char buf[128];
		sprintf(buf, "filename %s", op->filename);
		op_prefab_read_extra_data(prefab, buf);
		cursys->add((Op *)prefab);
		cursys->regenerateCallGraphs();
		prefab->pos = op->pos;
		op->system->deleteOp((Op *)op);
	}



}

CLASSNAME *METHOD(new)() {
	//CLASSNAME *ret = (CLASSNAME *) malloc(sizeof(CLASSNAME));
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);


	return ret;
}
