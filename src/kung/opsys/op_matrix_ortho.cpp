#include "op.h"
#include "op_matrix_ortho.h"

#include "matrix.h"

#define SHORTNAME "Ortho"
#define CLASSNAME OpOrtho
#define METHOD(name) op_ortho_##name
#define STRUCTNAME struct op_ortho_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	
	op_init_link((Op*)op, 0, "Left"  , OP_TYPE_FLOAT);
	op_init_link((Op*)op, 1, "Right" , OP_TYPE_FLOAT);
	op_init_link((Op*)op, 2, "Bottom", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 3, "Top"   , OP_TYPE_FLOAT);
	op_init_link((Op*)op, 4, "ZNear" , OP_TYPE_FLOAT);
	op_init_link((Op*)op, 5, "ZFar"  , OP_TYPE_FLOAT);
	op_init_link_output((Op*)op, 0, "Matrix", OP_TYPE_MATRIX);

	op->number_of_inputs = 6;
	op->number_of_outputs = 1;
	op->resize_minimal(op);

	op->render_editor = METHOD(render_editor);
	op->render        = METHOD(render);
	op->update        = METHOD(update);
	//op->dump_file     = METHOD(dump_file);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	for (int i=0; i<op->number_of_outputs; i++) {
		auto *ol = op->default_link_outputs + i;
		//log("persp has %d links on output %d", ol->inputlinks->size(), i);
		for (auto il : *ol->inputlinks) {
			// todo: calc once, then memcpy or so
			//mat_perspective(il->matrix, op->fov, op->aspect, op->znear, op->zfar);
			mat_ortho(il->matrix, op->left, op->right, op->bottom, op->top, op->znear, op->zfar);
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
