#include "op.h"
#include "op_value.h"

#define SHORTNAME "Value"
#define CLASSNAME OpValue
#define METHOD(name) op_value_##name
#define STRUCTNAME struct op_value_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(150, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );

	op_init_link((Op*)op, 0, "#", OP_TYPE_FLOAT);
	op_init_link_output((Op*)op, 0, "#", OP_TYPE_FLOAT);
	op->number_of_inputs = 1;
	op->number_of_outputs = 1;
	//op->resize_minimal(op);
	op->showtitle = 0;

	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	
	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::PushID(op);
	ImGui::PushItemWidth(80);
	op->default_link_inputs[0].changed += (int)ImGui::DragFloat(op->name, &op->val, 0.2f, 0.0f, 0.0f, "%.3f");
	ImGui::PopItemWidth();
	ImGui::PopID();

	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	//log("matrix: %d", op->default_link_inputs[0].val_i);
	//log("got value: %f", op->val);
	auto *ol = op->default_link_outputs + 0;

	ol->val_f = op->default_link_inputs[0].val_f;
	op->default_link_outputs[0].changed++;

	//for (auto il : *ol->inputlinks) {
	//	il->val_f = op->val;
	//	if (op->type == OpType::OP_TYPE_SENDER)
	//		il->owner->update(il->owner);
	//	il->changed += op->default_link_inputs[0].changed;
	//
	//	
	//}
	// only update the changed values one frame
	//op->default_link_inputs[0].changed = 0;
}


void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	fprintf(f, "value %f",
		op->val
	);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	sscanf(line, "value %f",
		&op->val
	);
}


CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
