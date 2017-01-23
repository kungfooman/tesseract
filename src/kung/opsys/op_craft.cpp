#include "op.h"
#include "op_craft.h"

#define SHORTNAME "Craft"
#define CLASSNAME OpCraft
#define METHOD(name) op_craft_##name
#define STRUCTNAME struct op_craft_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	strcpy( op->default_link_inputs[0].name, "Chunks X");
	strcpy( op->default_link_inputs[1].name, "Chunks Y");

	//op_init_link

	op->number_of_inputs = 2;
	op->number_of_outputs = 0;
	op->resize_minimal(op);

	op->render_editor = METHOD(render_editor);
	op->render        = METHOD(render);
	op->update        = METHOD(update);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	if (op->cache_ingoing_connections == 0)
		return;
	log("new chunks i: %s\n", op->default_link_inputs[0].val_i);
	log("new chunks j: %s\n", op->default_link_inputs[1].val_i);
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);

}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
