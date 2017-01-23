
#include "opsystem_opengl.h"
#include "op.h"
#include "op_gl_program.h"
//#include "util.h"

#include <kung/include_duktape.h>


#define SHORTNAME "Program"
#define CLASSNAME OpProgram
#define METHOD(name) op_program_##name
#define STRUCTNAME struct op_program_s

void METHOD(make_program)(CLASSNAME *op) {
	if (op->program)
		glDeleteProgram(op->program);
	GLuint program = glCreateProgram();
	if ( ! op->vertexshader) {
		log("op->vertexshader not set for %s\n", op->name);
		goto error;
	}
	if ( ! op->fragmentshader) {
		log("shader2 not set for %s\n", op->name);
		goto error;
	}
	glAttachShader(program, op->vertexshader);
	glAttachShader(program, op->fragmentshader);
	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar *info = (GLchar *)calloc(length, sizeof(GLchar));
		glGetProgramInfoLog(program, length, NULL, info);
		log("glLinkProgram failed: %s\n", info);
		free(info);
		
		goto error;
	} else {
		op->haserror = 0;
	}
	glDetachShader(program, op->vertexshader);
	glDetachShader(program, op->fragmentshader);
	//glDeleteShader(shader1);
	//glDeleteShader(shader2);
	op->program = program;
	log("update program: %d\n", op->program);
	return;
error:
	if (program)
		glDeleteProgram(program);
	if (op->program)
		glDeleteProgram(op->program);
	op->program = 0;
	op->haserror = 1;
}

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->type = OP_TYPE_DELIVERER;
	op->size = ImVec2(300, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	
	op->fragmentshader = 0;
	op->vertexshader = 0;

	op->number_of_inputs = 2;
	op_init_link((Op *) op, 0, "Frag", OP_TYPE_FRAGMENTSHADER);
	op_init_link((Op *) op, 1, "Vert", OP_TYPE_VERTEXSHADER);
	op->number_of_outputs = 1;
	op_init_link_output((Op *)op, 0, "Prog", OP_TYPE_PROGRAM);
	
	op->resize_minimal(op);

	op->render_editor      = METHOD(render_editor);
	op->render             = METHOD(render);
	op->update             = METHOD(update);
	op->write_extra_data   = METHOD(write_extra_data);
	op->read_extra_data    = METHOD(read_extra_data);
	op->on_link_disconnect = METHOD(on_link_disconnect);
	op->on_link_update     = METHOD(on_link_update);

	
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::Text("%d", op->program);
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {

	if (op->fragmentshader && op->vertexshader && op->haserror==0 && op->program==0) {
		METHOD(make_program)(op);
		op->default_link_outputs[0].changed++;
	}

	//auto *ol = op->default_link_outputs + 0;
	//for (auto il : *ol->inputlinks) {
	//	il->opengl_id = op->program;
	//	//if (op->type == OpType::OP_TYPE_SENDER)
	//	//	il->owner->update(il->owner);
	//}
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
	ImGui::Text("fragmentshader: %d", op->fragmentshader);
	ImGui::Text("vertexshader: %d", op->vertexshader);
	ImGui::Text("program: %d", op->program);
	if ( ImGui::Button("Make Program") )
		METHOD(make_program)(op);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	//fprintf(f, "filename %s",
	//	op->filename
	//);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	//sscanf(line, "filename %[^\n]",
	//	&op->filename
	//);
}

void METHOD(on_link_disconnect)(CLASSNAME *op, int id) {
	//log("disconnect id: %d\n", id);
	switch (op->default_link_inputs[id].type) {
		case OP_TYPE_FRAGMENTSHADER:
			op->fragmentshader = 0;
			break;
		case OP_TYPE_VERTEXSHADER:
			op->vertexshader = 0;
			break;
	}
}

void METHOD(on_link_update)(CLASSNAME *op, int id) {
	log("link changed id: %d\n", id);
	METHOD(make_program)(op);
	switch (op->default_link_inputs[id].type) {
		case OP_TYPE_FRAGMENTSHADER:
			//op->fragmentshader = 0;
			break;
		case OP_TYPE_VERTEXSHADER:
			//op->vertexshader = 0;
			break;
	}
}


CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
