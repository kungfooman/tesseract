#include "op.h"
#include "op_checkerboard.h"

#include <kung/include_imgui.h>

#define SHORTNAME "Checkerboard"
#define CLASSNAME OpCheckerboard
#define METHOD(name) op_checkerboard_##name
#define STRUCTNAME struct op_checkerboard_s

#define WIDTH  ((op->width))
#define HEIGHT ((op->height))
#define AND_I  ((op->and_i))
#define AND_J  ((op->and_j))

void METHOD(makeCheckImage)(CLASSNAME *op)
{
	int i, j, c;

	if ( ! op->checkImage)
	return;

	GLubyte *img = op->checkImage;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			c = ((((i & AND_I)==0)^((j & AND_J))==0))*255;
			*img++ = (GLubyte) c;
			*img++ = (GLubyte) c;
			*img++ = (GLubyte) c;
			*img++ = (GLubyte) 255;
		}
	}
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, op->checkImage);

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

	op->number_of_inputs = 4;

	WIDTH  = 64;
	HEIGHT = 64;
	AND_I  =  8;
	AND_J  =  8;
	
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

	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::Text("Checkboard texid=%d", op->textureid);

	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	if ( ! op_did_inputs_change((Op *)op) )
		return;

	if (op->checkImage)
		free(op->checkImage);
	op->checkImage = (GLubyte *)malloc(WIDTH * HEIGHT * 4);
	METHOD(makeCheckImage)(op);
	glDeleteTextures(1, (const GLuint *)&op->textureid);
	METHOD(generate_texture)(op);

	op_set_inputs_unchanged((Op *)op);
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
	ImGui::Text("address: %x", op->checkImage);
	ImGui::Text("textureid: %x", op->textureid);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	fprintf(f, "width %d height %d and_i %d and_j %d",
		op->width,
		op->height,
		op->and_i,
		op->and_j
	);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	sscanf(line, "width %d height %d and_i %d and_j %d",
		&op->width,
		&op->height,
		&op->and_i,
		&op->and_j
	);
}

CLASSNAME *METHOD(new)() {
	//CLASSNAME *ret = (CLASSNAME *) malloc(sizeof(CLASSNAME));
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
