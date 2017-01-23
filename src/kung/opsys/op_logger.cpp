#include "op.h"
#include "op_logger.h"

#define SHORTNAME "Logger"
#define CLASSNAME OpLogger
#define METHOD(name) op_logger_##name
#define STRUCTNAME struct op_logger_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	strcpy( op->default_link_inputs[0].name, "Text");
	strcpy( op->default_link_inputs[0].val_str, "no value set");
	op->number_of_inputs = 1;
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
	log("log: %s", op->default_link_inputs[0].val_str);
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);

}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
