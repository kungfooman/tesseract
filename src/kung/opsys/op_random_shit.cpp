//#include "craftstructs.h"

#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
//#include "imgui_impl_glfw.h"
#include <stdio.h>
//#include "libs\glfw\include\GLFW/glfw3.h"
#include <Windows.h>
#include <list>

#include "op.h"
#include "op_random_shit.h"


#define SHORTNAME "RandomShit"
#define CLASSNAME OpRandomShit
#define METHOD(name) op_random_shit_##name
#define STRUCTNAME struct op_random_shit_s

void METHOD(init)(CLASSNAME *op) {

	op_init((Op *)op);
	strcpy( op->name, SHORTNAME );
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 250);
	
	op->number_of_inputs = 1;
	op_init_link((Op *)op, 0, "Mesh", OP_TYPE_MESH );
	op->number_of_outputs = 1;
	op_init_link_output((Op *)op, 0, "Mesh", OP_TYPE_MESH );

	op->type             = OpType::OP_TYPE_REQUESTER;
	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
}

class FixedArea {
public:
	ImVec2 initoffset;
	ImVec2 pos;
	ImVec2 size;
	int cols;
	FixedArea(ImVec2 pos_, ImVec2 size_, int cols_) {
		pos = pos_;
		size = size_;
		cols = cols_;
		initoffset = ImGui::GetCursorPos();
	}

	// returns width and height of the element
	ImVec2 SetPos(int row, int col) {
		float x = (size.x / cols) * col;
		float y = row * 20;
		ImGui::SetCursorPos(initoffset + pos + ImVec2(x, y));
		return ImVec2(size.x / cols, 20);
	}
};

//extern Model *g;

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
#if 0
	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::Text("g->chunk_count: %d", g->chunk_count);
	
	FixedArea fa(op->pos, op->size, 3);

	static int from = 0;
	static int amount = 20;
	
	ImGui::DragInt("from", &from);
	ImGui::DragInt("amount", &amount);

	int fromzero = 0;
	for (int i=from; i<from+amount; i++, fromzero++) {


		for (int j=0; j<3; j++) {

			ImGui::SetCursorPos(op->pos + ImVec2(5, (float)fromzero*20.0f));
			//ImGui::Text("%d, %d", i, j);
			int offset = i * 3 + j;
			
			ImGui::PushID(i);
			//ImGui::DragFloat("", address, 0.01f);

			Chunk *c = g->chunks + i;
			ImGui::Text("chunk i=%d faces=%d p=%d q=%d buffer=%d datasize=%d datacopy=%d", i, c->faces, c->p, c->q, c->buffer, c->datasize, c->datacopy);

			ImGui::PopID();
		}
	}
#endif



	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	auto *ol = op->default_link_outputs + 0;
	for (auto il : *ol->inputlinks) {
		il->mesh = op->default_link_inputs[0].mesh;
		//if (op->type == OpType::OP_TYPE_SENDER)
		//	il->owner->update(il->owner);
	}

}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
	ImGui::Text("Mesh: %x", op->default_link_inputs[0].mesh);
	//ImGui::Text("textureid: %x", op->textureid);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	//fprintf(f, "x %f y %f z %f n %f",
	//	op->x,
	//	op->y,
	//	op->z,
	//	op->n
	//);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	//sscanf(line, "x %f y %f z %f n %f",
	//	&op->x,
	//	&op->y,
	//	&op->z,
	//	&op->n
	//);
}

CLASSNAME *METHOD(new)() {
	//CLASSNAME *ret = (CLASSNAME *) malloc(sizeof(CLASSNAME));
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
