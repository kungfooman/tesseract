#include "op.h"
#include "op_matrix_multiply.h"

#include "matrix.h"

#define SHORTNAME "MatrixMultiply"
#define CLASSNAME OpMatrixMultiply
#define METHOD(name) op_matrix_multiply_##name
#define STRUCTNAME struct op_matrix_multiply_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	
	for (int i=0; i<16; i++) {

		op_init_link((Op*)op, i, "Matrix", OP_TYPE_MATRIX);
	}
	op_init_link_output((Op*)op, 0, "Matrix", OP_TYPE_MATRIX);
	op->number_of_inputs = 2;
	op->number_of_outputs = 1;
	op->resize_minimal(op);

	op->render_editor      = METHOD(render_editor);
	op->render             = METHOD(render);
	op->update             = METHOD(update);
	op->write_extra_data   = METHOD(write_extra_data);
	op->read_extra_data    = METHOD(read_extra_data);
	op->on_link_disconnect = METHOD(on_link_disconnect);

	
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	
	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	if ( ! op->forcereload)
		return;
	//if (!strcmp(op->name, "MatMulHurr"))
	//	log("upddate matrix multiply\n");

	// multiply first 2 matrices together (always exist)

	//std::vector<int> valid_inputs(16);
	//for (int i=0; i<op->number_of_inputs; i++) {
	//	if (op->default_link_inputs[i].cached_backlinks->size() == 0)
	//		continue;
	//
	//	valid_inputs.push_back(i);
	//}
	//
	//
	//// we cant calculate anything without at least 2 matrices
	//if (valid_inputs.size() < 2)
	//	return;


	mat_multiply(op->default_link_outputs[0].matrix, op->matrix_a, op->matrix_b);

	for (int i=2; i<op->number_of_inputs; i++) {

		if (op->default_link_inputs[i].cached_backlinks->size() == 0)
			continue;
		// dont calculate inputs with no connection, so we can just leave some empty spaces to try&error stuff inbetween them for fast testing

		mat_multiply(op->default_link_outputs[0].matrix,op->default_link_outputs[0].matrix, op->default_link_inputs[i].matrix);
	}
	
	op->default_link_outputs[0].changed++;
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
	ImGui::DragInt("inputs", &op->number_of_inputs, 1.0f, 2, 16);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	fprintf(f, "inputs %d",
		op->number_of_inputs
	);
}


void METHOD(on_link_disconnect)(CLASSNAME *op, int id) {
	// reset matrix when we remove the link, so we cont need this std::vector shit to detect the first two "real" matrices... we just calculate all
	// multiplying by identity doesnt matters
	mat_identity(op->default_link_inputs[id].matrix);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	sscanf(line, "inputs %d",
		&op->number_of_inputs
	);
}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}

