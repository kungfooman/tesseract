#include "op.h"
#include "op_javascript.h"

#include <kung/include_duktape.h>

#define SHORTNAME "JavaScript"
#define CLASSNAME OpJavaScript
#define METHOD(name) op_javascript_##name
#define STRUCTNAME struct op_javascript_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(300, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	strcpy( op->default_link_inputs[0].name, ">");
	strcpy( op->default_link_inputs[0].val_str, "no value set");
	op->number_of_inputs = 1;
	op->number_of_outputs = 0;

	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::PushItemWidth(op->size.x - 50);
	ImGui::PushID(op);
	ImGui::InputText("Code", op->code, sizeof op->code);
	ImGui::PopID();
	ImGui::PopItemWidth();
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	if (op->cache_ingoing_connections == 0)
		return;
	//log("log: %s", op->default_link_inputs[0].val_str);
	js_call("eval", "s", op->code);
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);

}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	fprintf(f, "code %s",
		op->code
	);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	sscanf(line, "code %[^\n]",
		&op->code
	);
}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
