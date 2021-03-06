#include "op.h"
#include "op_prefab_input.h"

#define SHORTNAME "PrefabInput"
#define CLASSNAME OpPrefabInput
#define METHOD(name) op_prefab_input_##name
#define STRUCTNAME struct op_prefab_input_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(300, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	op->inputid = 0;
	
	op->number_of_inputs = 0;
	op->number_of_outputs = 1;
	op_init_link_output((Op *)op, 0, "In", OP_TYPE_PREFAB_INPUT);
	op->resize_minimal(op);

	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);

	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::Text("%d", op->inputid);

	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);

}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	//fprintf(f, "code %s",
	//	op->code
	//);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	//sscanf(line, "code %[^\n]",
	//	&op->code
	//);
}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
