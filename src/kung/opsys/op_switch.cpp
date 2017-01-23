#include "op.h"
#include "op_switch.h"

#define SHORTNAME "Switch"
#define CLASSNAME OpSwitch
#define METHOD(name) op_switch_##name
#define STRUCTNAME struct op_switch_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	strcpy( op->default_link_inputs[0].name, "Text");
	op->number_of_inputs = 1;
	op->number_of_outputs = 7;
	op->resize_minimal(op);

	op->render_editor = METHOD(render_editor);
	op->render        = METHOD(render);
	op->update        = METHOD(update);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);

	char *switchlist[16] = {
		"Escape", "w", "a", "s", "d", "f", "g", "h", "", "", "", "", "", "", ""
	};
	
	for (int i=0; i<7; i++)
		sprintf(op->default_link_outputs[i].name, "On %s", switchlist[i]);

	op->resize_minimal(op);

	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	//log("log: %s", op->default_link_inputs[0].val_str);
	
	if (op->cache_ingoing_connections == 0)
		return;

	char *switchlist[16] = {
		"Escape", "w", "a", "s", "d", "f", "g", "h", "", "", "", "", "", "", ""
	};
	

	for (int i=0; i<7; i++) {
		if ( ! strcmp(op->default_link_inputs[0].val_str, switchlist[i]) )
		{
			for (auto il : *op->default_link_outputs[i].inputlinks) {
				il->owner->update(il->owner);
			}
		}
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
