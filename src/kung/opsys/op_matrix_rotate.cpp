#include "op.h"
#include "op_matrix_rotate.h"

#include "matrix.h"

#define SHORTNAME "MatrixRotate"
#define CLASSNAME OpMatrixRotate
#define METHOD(name) op_matrix_rotate_##name
#define STRUCTNAME struct op_matrix_rotate_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	
	op_init_link((Op*)op, 0, "X", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 1, "Y", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 2, "Z", OP_TYPE_FLOAT);
	op_init_link((Op*)op, 3, "Angle", OP_TYPE_FLOAT);
	op_init_link_output((Op*)op, 0, "Matrix", OP_TYPE_MATRIX);
	op->number_of_inputs = 4;
	op->number_of_outputs = 1;
	op->resize_minimal(op);

	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	if ( ! op->forcereload)
		return;


	//if (!strcmp(op->name, "MatRotHurr"))
	//	log("upddate matrix rotate\n");

	mat_rotate(op->default_link_outputs[0].matrix, op->x, op->y, op->z, op->angle);
	op->default_link_outputs[0].changed++;
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	fprintf(f, "x %f y %f z %f angle %f",
		op->x,
		op->y,
		op->z,
		op->angle
	);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	sscanf(line, "x %f y %f z %f angle %f",
		&op->x,
		&op->y,
		&op->z,
		&op->angle
	);
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
