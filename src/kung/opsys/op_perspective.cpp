#include "op.h"
#include "op_perspective.h"

#include "matrix.h"

#define SHORTNAME "Perspective"
#define CLASSNAME OpPerspective
#define METHOD(name) op_perspective_##name
#define STRUCTNAME struct op_perspective_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	
	op_init_link((Op*)op, 0, "Fov", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 1, "Aspect", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 2, "ZNear", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 3, "ZFar", OP_TYPE_FLOAT);
	op_init_link_output((Op*)op, 0, "Matrix", OP_TYPE_MATRIX);

	op->number_of_inputs = 4;
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
	mat_perspective(op->tmp_matrix, op->fov, op->aspect, op->znear, op->zfar);

	for (int i=0; i<op->number_of_outputs; i++) {
		auto *ol = op->default_link_outputs + i;
		//log("persp has %d links on output %d", ol->inputlinks->size(), i);
		for (auto il : *ol->inputlinks) {
			matcpy(il->matrix, op->tmp_matrix);
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