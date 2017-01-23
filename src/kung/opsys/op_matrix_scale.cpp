#include "op.h"
#include "op_matrix_scale.h"

#include "matrix.h"

#define SHORTNAME "MatrixScale"
#define CLASSNAME OpMatrixScale
#define METHOD(name) op_matrix_scale_##name
#define STRUCTNAME struct op_matrix_scale_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	
	op_init_link((Op*)op, 0, "dx", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 1, "dy", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 2, "dz", OP_TYPE_FLOAT);
	op_init_link_output((Op*)op, 0, "Matrix", OP_TYPE_MATRIX);
	op->number_of_inputs = 3;
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
	if ( ! op->forcereload)
		return;
	mat_scale(op->default_link_outputs[0].matrix, op->dx, op->dy, op->dz);
	op->default_link_outputs[0].changed++;
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}