#include "op.h"
#include "op_expression.h"

#include <kung/include_duktape.h>

#define SHORTNAME "Expression"
#define CLASSNAME OpExpression
#define METHOD(name) op_expression_##name
#define STRUCTNAME struct op_expression_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(300, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	strcpy( op->default_link_inputs[0].name, ">");
	strcpy( op->default_link_inputs[0].val_str, "no value set");
	op->number_of_inputs = 1;
	op->number_of_outputs = 0;

	op->execid = 0;
	strcpy(op->code, "");

	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
}

// as seen in tofloat op
static void init_js_code(OpExpression *op) {
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


void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);

	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::Text("%d", op->execid);

	ImGui::SetCursorPos(op->pos + ImVec2(40,5));
	ImGui::PushItemWidth(op->size.x - 50);
	ImGui::PushID(op);
	int changed = ImGui::InputText("Code", op->code, sizeof op->code);
	if (changed) {
		init_js_code(op);
	}
	ImGui::PopID();
	ImGui::PopItemWidth();
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	//log("log: %s", op->default_link_inputs[0].val_str);
	//js_call("eval", "s", op->code);


	duk_push_global_object(ctx);
	//duk_get_prop_string(ctx, -1, op->name);
	duk_get_prop_index(ctx, -1, (int)op);
	int ret = duk_pcall(ctx, 0);
	//if (ret == 0)
	//	op->val = duk_get_number(ctx, -1);
	//else
	//	op->val = 1337.0f;
	duk_pop(ctx); // pop ret
	duk_pop(ctx); // pop global
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
