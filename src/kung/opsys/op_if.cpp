#include "op.h"
#include "op_if.h"

#define SHORTNAME "If"
#define CLASSNAME OpIf
#define METHOD(name) op_if_##name
#define STRUCTNAME struct op_if_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(200, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	
	op_init_link((Op*)op, 0, "a", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 1, "b", OP_TYPE_FLOAT);
	op_init_link_output((Op*)op, 0, "Y", OP_TYPE_FLOAT);
	op_init_link_output((Op*)op, 1, "N", OP_TYPE_FLOAT);
	op->number_of_inputs = 2;
	op->number_of_outputs = 2;
	//op->resize_minimal(op);

	op->render_editor = METHOD(render_editor);
	op->render        = METHOD(render);
	op->update        = METHOD(update);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	
	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::PushID(op);
	ImGui::PushItemWidth(150);
	ImGui::InputFloat(op->name, &op->tmp_val);
	ImGui::PopItemWidth();
	ImGui::PopID();

	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	op->tmp_val = op->val_a - op->val_b;
	auto *ol = op->default_link_outputs + 0;
	for (auto il : *ol->inputlinks) {
		il->val_f = op->tmp_val;
		if (op->type == OpType::OP_TYPE_SENDER)
			il->owner->update(il->owner);
	}
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
