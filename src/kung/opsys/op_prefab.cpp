#include "op.h"
#include "op_prefab.h"
#include "opsystem.h"

#include <kung/matrix.h>

#define SHORTNAME "Prefab"
#define CLASSNAME OpPrefab
#define METHOD(name) op_prefab_##name
#define STRUCTNAME struct op_prefab_s

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->size = ImVec2(200, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	op->showtitle = 0;
	op->isprefab = 1;
	op->subsys = NULL;

	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	
	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	ImGui::PushID(op);
	ImGui::PushItemWidth(80);
	if (ImGui::Button(op->name))
		cursys = op->subsys;
	ImGui::PopItemWidth();
	ImGui::PopID();

	op_post_render((Op *)op);
}

void METHOD(update)(CLASSNAME *op) {
	int i = 0;

	if (op->subsys == 0) {
		return;
	}

	// first we copy over the input values
	for (auto input : op->subsys->prefabinputs) {
		LinkOutput *linkoutput = input->default_link_outputs + 0;
		for (auto inputlink : *linkoutput->inputlinks) {
			inputlink->val_f = op->default_link_inputs[i].val_f;
		}
		i++;
	}

	// then we update the subsys for the input values
	op->subsys->update();

	// and then we just distribute the calculated values to all outputs
	i = 0;
	for (auto output : op->subsys->prefaboutputs) {
		Link *link = output->default_link_inputs + 0;

		if (link->cached_backlinks->size() != 0) {
			LinkOutput *linkoutput = link->cached_backlinks->front();
		
			LinkOutput *prefablinkoutput = op->default_link_outputs + i;
			for (auto inputlink : *prefablinkoutput->inputlinks) {
				switch (inputlink->type) {
					case OP_TYPE_FLOAT:
						inputlink->val_f = linkoutput->val_f;
						break;
					case OP_TYPE_MATRIX:
						matcpy(inputlink->matrix, linkoutput->matrix);
						
						break;
				}
				
			}
		}
		i++;
	}
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
	//ImGui::Text("Filename: %s", op->filename);
	ImGui::InputText("filename", op->filename, sizeof op->filename);
	if (ImGui::Button("Clear System")) {
		op->subsys->clearScene();
		delete op->subsys;
		op->subsys = NULL;
	}
	if (ImGui::Button("Load System")) {
		op->subsys = new OpSystem(op->filename, op);
		op->subsys->loadFile(op->filename);
	}
	if (ImGui::Button("32ewReload System")) {
	}
	ImGui::Text("inputs: %d outputs: %d", op->number_of_inputs, op->number_of_outputs);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	fprintf(f, "filename %s",
		op->filename
	);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	sscanf(line, "filename %s",
		op->filename
	);

	op->subsys = new OpSystem(op->filename, op);
	op->subsys->loadFile(op->subsys->name);
	op->resize_minimal(op);
}

CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
