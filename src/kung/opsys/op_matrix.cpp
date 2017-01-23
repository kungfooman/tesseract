#include "op.h"
#include "op_matrix.h"
#include <kung/matrix.h> // matcpy

#define SHORTNAME "Matrix"
#define CLASSNAME OpMatrix
#define METHOD(name) op_matrix_##name
#define STRUCTNAME struct op_matrix_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	
	op_init_link((Op*)op, 0, "Matrix", OP_TYPE_MATRIX);
	op_init_link_output((Op*)op, 0, "Matrix", OP_TYPE_MATRIX);
	op->number_of_inputs = 1;
	op->number_of_outputs = 1;
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

	matcpy(op->default_link_outputs[0].matrix, op->default_link_inputs[0].matrix);

	for (int i=0; i<op->number_of_outputs; i++) {
		auto *ol = op->default_link_outputs + i;
		for (auto il : *ol->inputlinks) {
			memcpy((void *)il->matrix, (void *)op->matrix, 16 * 4);
			il->owner->update(il->owner);
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