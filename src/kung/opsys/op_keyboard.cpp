#include "op.h"
#include "op_keyboard.h"

#define SHORTNAME "Keyboard"
#define CLASSNAME OpKeyboard
#define METHOD(name) op_keyboard_##name
#define STRUCTNAME struct op_keyboard_s

std::list<OpKeyboard *> keyboards;

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	op->number_of_inputs = 0;
	op->number_of_outputs = 1;
	strcpy(op->default_link_outputs[0].name, "Text");
	op->resize_minimal(op);
	op->render_editor = METHOD(render_editor);
	op->render        = METHOD(render);
	op->update        = METHOD(update);
	op->destroy       = METHOD(destroy);
	op->type = OpType::OP_TYPE_SENDER;

	keyboards.push_back(op);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	LinkOutput *output = op->default_link_outputs + 0;
	for (auto text : op->events) {
		for (auto il : *output->inputlinks) {
			strcpy(il->owner->default_link_inputs->val_str, text);
			il->owner->update(il->owner);
		}
		free(text);
	}
	op->events.clear();
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
}


void METHOD(destroy)(CLASSNAME *op) {
	op_destroy((Op *)op);
	keyboards.remove(op);
}

CLASSNAME *METHOD(new)() {
	//CLASSNAME *ret = (CLASSNAME *) malloc(sizeof(CLASSNAME));
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
