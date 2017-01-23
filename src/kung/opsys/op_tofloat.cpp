#include "op.h"
#include "op_tofloat.h"

#include <kung/include_duktape.h>

#define SHORTNAME "ToFloat"
#define CLASSNAME OpToFloat
#define METHOD(name) op_tofloat_##name
#define STRUCTNAME struct op_tofloat_s

void init_js_code(OpToFloat *op) {
	duk_push_string(ctx, op->code);
	duk_push_string(ctx, op->name);
	if (duk_pcompile(ctx, 0) != 0) {
	log("compile failed: %s\n", duk_safe_to_string(ctx, -1));
	} else {
			
		duk_push_global_object(ctx);
			
		duk_dup(ctx, -2); // 
		//duk_put_prop_string(ctx, -2, op->name);
		duk_put_prop_index(ctx, -2, (int)op);

		duk_pop(ctx); // pop global

		//duk_call(ctx, 0);      /* [ func ] -> [ result ] */
		//log("program result: %s\n", duk_safe_to_string(ctx, -1));
	}
	duk_pop(ctx);
}

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(300, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	//strcpy( op->default_link_inputs[0].name, ">");
	//strcpy( op->default_link_inputs[0].val_str, "no value set");

	op_init_link_output((Op *)op, 0, "#", OP_TYPE_FLOAT);

	op->number_of_inputs = 0;
	op->number_of_outputs = 1;

	strcpy(op->code, "");
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
	ImGui::PushItemWidth(100);
	ImGui::Text("%f", op->val);
	ImGui::PopItemWidth();

	ImGui::SetCursorPos(op->pos + ImVec2(105,5));
	ImGui::PushItemWidth(op->size.x - 110);
	ImGui::PushID(op);
	int changed = ImGui::InputText("", op->code, sizeof op->code);

	if (changed) {
		log("changed\n");

		init_js_code(op);


	}
	ImGui::PopID();
	ImGui::PopItemWidth();
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	//log("log: %s", op->default_link_inputs[0].val_str);
	//js_call("eval", "s", op->code);

	//duk_eval_string(ctx, op->code);

	//duk_push_string(ctx, op->name);
	//int ret = duk_eval_raw(ctx, op->code, 0, DUK_COMPILE_EVAL | DUK_COMPILE_SAFE | DUK_COMPILE_NOSOURCE | DUK_COMPILE_STRLEN);

	duk_push_global_object(ctx);
	//duk_get_prop_string(ctx, -1, op->name);
	duk_get_prop_index(ctx, -1, (int)op);
	int ret = duk_pcall(ctx, 0);
	if (ret == 0)
		op->val = duk_get_number(ctx, -1);
	else
		op->val = 1337.0f;
	duk_pop(ctx); // pop ret
	duk_pop(ctx); // pop global
	op->default_link_outputs[0].changed++;
	//if (ret == 0)
	//	op->color = 0x00FF00FF;
	//else
	//	op->color = 0x0000FFFF;

	//log("ret: %d", ret);


	//auto *ol = op->default_link_outputs + 0;
	//for (auto il : *ol->inputlinks) {
	//	il->val_f = op->val;
	//	if (op->type == OpType::OP_TYPE_SENDER)
	//		il->owner->update(il->owner);
	//}


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

	init_js_code(op);
}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
