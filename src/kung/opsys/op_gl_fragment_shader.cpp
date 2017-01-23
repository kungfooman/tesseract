#include "opsystem_opengl.h"
#include "op.h"
#include "op_gl_fragment_shader.h"

#include <kung/include_duktape.h>


#define SHORTNAME "FragmentShader"
#define CLASSNAME OpFragmentShader
#define METHOD(name) op_fragment_shader_##name
#define STRUCTNAME struct op_fragment_shader_s

void METHOD(make_shader)(CLASSNAME *op) {
	if (op->fragmentshader)
		glDeleteShader(op->fragmentshader);
	op->fragmentshader = opengl_load_shader(GL_FRAGMENT_SHADER, op->filename, &op->haserror);
	op->changed(op);
	log("frag loaded: op->haserror=%d\n", op->haserror);
}

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->type = OP_TYPE_DELIVERER;
	op->size = ImVec2(300, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	strcpy( op->default_link_inputs[0].name, ">");
	strcpy( op->default_link_inputs[0].val_str, "no value set");
	op->number_of_inputs = 0;
	op->number_of_outputs = 1;
	op_init_link_output((Op *)op, 0, "Frag", OP_TYPE_FRAGMENTSHADER);

	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);

	//op->fragmentshader = load_shader(GL_FRAGMENT_SHADER, op->filename);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::PushItemWidth(op->size.x - 50);
	ImGui::PushID(op);
	ImGui::InputText("File", op->filename, sizeof op->filename);
	ImGui::PopID();
	ImGui::PopItemWidth();
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {

	if (openglstarted && op->fragmentshader==0 && op->haserror==0) {
		METHOD(make_shader)(op);
		op->default_link_outputs[0].changed++;
	}

	//auto *ol = op->default_link_outputs + 0;
	//for (auto il : *ol->inputlinks) {
	//	il->opengl_id = op->fragmentshader;
	//	//if (op->type == OpType::OP_TYPE_SENDER)
	//	//	il->owner->update(il->owner);
	//}
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
	ImGui::Text("fragmentshader: %d", op->fragmentshader);
	if ( ImGui::Button("Reload shader") )
		METHOD(make_shader)(op);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	fprintf(f, "filename %s",
		op->filename
	);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	sscanf(line, "filename %[^\n]",
		&op->filename
	);
}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
