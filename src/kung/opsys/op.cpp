#include <include_imgui.h>
#include <include_gl.h>

//#include "imgui_impl_glfw.h"
#include <stdio.h>
//#include "libs\glfw\include\GLFW/glfw3.h"
#include <Windows.h>
#include <list>
#include <matrix.h>

//void DrawLine(float a_x, float a_y, float b_x, float b_y) {
void DrawLine(float a_x, float a_y, float b_x, float b_y) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 winpos = ImGui::GetWindowPos();
	ImVec2 a = ImVec2(a_x, a_y);
	a += winpos;
	ImVec2 b = ImVec2(b_x, b_y);
	b += winpos;
	float thickness = 1.0f;
	window->DrawList->AddLine(a, b, 0xffffffff, thickness);
}
void DrawRect(float a_x, float a_y, float b_x, float b_y) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 winpos = ImGui::GetWindowPos();
	ImVec2 a = ImVec2(a_x, a_y);
	a += winpos;
	ImVec2 b = ImVec2(b_x, b_y);
	b += winpos;
	float thickness = 1.0f;
	window->DrawList->AddRect(a, b, 0xffffffff, thickness);
}

#include "op_all.h"
#include "op_prefab.h"
#include "op_prefab_input.h"
#include "op_prefab_output.h"
Op *dragged_op = NULL;


// new link system
LinkOutput *link_from;
Link *link_to;

std::list<linkline_t *> linklines;

//void LinkOutput::setMatrix(glm::mat4x4 matrix) {
void LinkOutput::setMatrix(float *matrix) {
	for (auto link_end : *inputlinks) {
		// link_end->matrix = matrix;
		memcpy((void *) link_end->matrix, (void *) matrix, 16);
		link_end->changed = 1;
	}
}

void op_init(Op *op) {
	op->pos = ImVec2(100.0f, 100.0f);
	op->size.x = 40.0f;
	op->size.y = 40.0f;
	strcpy( op->name, "Default Op" );
	op->selected = 0;
	op->is_permanent = 0;
	op->type = OpType::OP_TYPE_RECEIVER;
	char *names[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p"};
	op->color = 0xffffFFFF;
	op->system = NULL; // system the op belongs to, not the OpPrefab->subsys
	op->isprefab = 0;
	// init the default input links
	for (int i=0; i<16; i++) {
		Link *in = op->default_link_inputs + i;
		// set default names
		//strcpy(in->name, names[i]);
		// set owner to this op
		in->owner = op;
		// kinda force update, for ops who check: if ( ! did_inputs_change() ) return;
		in->changed = 1;
		in->local_id = i; // the load/save system depends on this
		in->cached_backlinks = new std::list<LinkOutput *>();
	}
	// init the default output links
	for (int i=0; i<op->number_of_outputs; i++) {
		LinkOutput *lo = op->default_link_outputs + i;
		strcpy(lo->name, names[i]);
		lo->owner = op;
		lo->inputlinks = new std::list<Link*>();
		lo->local_id = i; // need it for prefab remapping
	}

	//log("Constructor called for %s", op->name);
	
	// "virtual" methods
	op->render_editor      = op_render_editor;
	op->render             = op_render;
	op->update             = op_update;
	op->changed            = op_changed;
	op->dump_file          = op_dump_file;
	op->dump_links         = op_dump_links;
	op->resize_minimal     = op_resize_minimal;
	op->destroy            = op_destroy;
	op->request            = op_request;
	op->write_extra_data   = op_write_extra_data;
	op->read_extra_data    = op_read_extra_data;
	op->on_link_connect    = op_on_link_connect;
	op->on_link_disconnect = op_on_link_disconnect;
	op->on_link_update     = op_on_link_update;
	op->draw_in_scene      = NULL; // they are all collected in OpSystem::renderables
}

void op_init_link(Op *op, int id, char *name, int type /* maybe ... as default value */) {
	Link *current = op->default_link_inputs + id;
	strcpy(current->name, name);
	current->type = type;

	switch (type) {
		case OP_TYPE_MATRIX: mat_identity(current->matrix); break;
	}
	//if (strcmp(firstword,   "int") == 0) *ptr = (void *)&current->val_i;
	//if (strcmp(firstword, "float") == 0) *ptr = (void *)&current->val_f;
}
void op_init_link_output(Op *op, int id, char *name, int type) {
	LinkOutput *current = op->default_link_outputs + id;
	strcpy(current->name, name);
	current->type = type;
}

int op_did_inputs_change(Op *op) {
	int changed = 0;
	//changed += op->changed;
	for (int i=0; i<op->number_of_inputs; i++)
		changed += op->default_link_inputs[i].changed;
	return changed;
}
void op_set_inputs_unchanged(Op *op) {
	for (int i=0; i<op->number_of_inputs; i++)
		op->default_link_inputs[i].changed = 0;
}

// op_propagate_data is called in op_request()

void op_propagate_data(Op *op) {
	for (int i=0; i<op->number_of_outputs; i++) {
		LinkOutput *lo = op->default_link_outputs + i;

		if ( ! lo->changed)
			continue;

		for (auto li : *lo->inputlinks) {
			li->val_f = lo->val_f;
			li->val_i = lo->val_i;
			
			li->opengl_id = lo->opengl_id;
			matcpy(li->matrix, lo->matrix);

			// if we update a link, the op can check op->forcereload to reevaluate his outputs
			li->owner->forcereload++;
			li->changed++;
		}
	}
}

void op_request(Op *op) {
	// update childs in correct order
	// when we clearscene(), always update these references, otherwise crash
	for (auto tmp : op->update_ops_this_order) {
		//if (tmp->forcereload)
		{
			tmp->update(tmp);
			// we only copy over the values if there was actually a change
			//if (tmp->forcereload)
				op_propagate_data(tmp);
			tmp->forcereload = 0;
		}
	}
	// now our op has it's inputs set, so update ourselves now
	op->update(op);
	op->forcereload = 0;

}

// every op can implement on_link_update
// tho we dont call it directly
// the OpSystem will collect the op_changed calls
// and when every sub-op was updated, we are going to dispatch the on_link_updates
// otherwise e.g. "Prog" has "Vert" and "Frag" as inputs
// if Vert calls op_changed(), Prog would try to rebuild the shader
// but Frag wasn't even evaluated yet
void op_changed   (Op *op         ) {
	op->system->changed_ops.push_back( op );
}

void op_pre_render(Op *op) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 winpos = ImGui::GetWindowPos();
	ImRect op_item = ImRect(winpos + op->pos, winpos + op->pos + op->size);
	ImGuiContext& g = *GImGui;
	unsigned int color = op->selected ? 0xff00ff00 : op->color;
	// we draw the border 1 pixel outside, so the contents can use all space
	window->DrawList->AddRect(op_item.Min + ImVec2(-1,-1), op_item.Max + ImVec2(1,1), color);
}



// post_render does the mouse movement interaction, otherwise the childrens items are "disabled", because this one would "eat" them
void op_post_render(Op *op) {
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 winpos = ImGui::GetWindowPos();

	ImRect op_item = ImRect(winpos + op->pos, winpos + op->pos + op->size);


	int isprefab = IS_PREFAB(op);
	
	OpPrefab *prefab = NULL;
	if (IS_PREFAB(op))
		prefab = (OpPrefab *)op;
	if ( ! prefab) {
		for (int i=0; i<op->number_of_inputs; i++) {
			Link *in = op->default_link_inputs + i;
			in->pos = op->pos + ImVec2(-20, i * 20);
			ImGui::SetCursorPos(in->pos);
			ImGui::Button(in->name);
			if (ImGui::IsItemHoveredRect()) {
				link_to = in;
			}
		}

		for (int i=0; i<op->number_of_outputs; i++) {
			LinkOutput *lo = op->default_link_outputs + i;

			// print the output buttons
			lo->pos = op->pos + ImVec2(op->size.x,20 * i);
			ImGui::SetCursorPos(lo->pos);
			ImGui::Button(lo->name);
			if (ImGui::IsItemClicked(0)) {
				link_from = lo;
			}
		}
	} else if (prefab->subsys) {

		

		int i=0;
		for (auto op : prefab->subsys->prefabinputs) {
			OpPrefabInput *prefabinput = (OpPrefabInput *)op;

			// This is the output of prefabinput
			LinkOutput *asd = prefabinput->default_link_outputs + 0;
			
			Link *prefablink = prefab->default_link_inputs + i;
			ImVec2 linkpos = prefab->pos + ImVec2(-20, i * 20);
			prefablink->pos = linkpos;
			ImGui::SetCursorPos(linkpos);

			if (asd->inputlinks->size() == 0) {
				ImGui::Button("no link");
			} else {
				Link *reallink = asd->inputlinks->front();
				ImGui::Button(reallink->name);
			}
			if (ImGui::IsItemHoveredRect()) {
				// we do not link to the real links
				// Instead prefab->update() will copy the needed input value into the subsystem values
				// so the systems do not mix and it feels way less awkward
				//link_to = reallink;
				link_to = prefablink;
			}
			i++;
		}

	
		i = 0;
		for (auto op : prefab->subsys->prefaboutputs) {
			OpPrefabOutput *prefaboutput = (OpPrefabOutput *)op;
			Link *polink = prefaboutput->default_link_inputs + 0;
			
			LinkOutput *prefablinkout = prefab->default_link_outputs + i;
			ImVec2 linkoutputpos = prefab->pos + ImVec2(prefab->size.x, 20 * i);
			prefablinkout->pos = linkoutputpos;
			ImGui::SetCursorPos(linkoutputpos);
			if (polink->cached_backlinks->size()) {
				LinkOutput *reallinkoutput = polink->cached_backlinks->front();
				ImGui::Button(reallinkoutput->name);
			} else {
				ImGui::Button("no link");
			}
			if (ImGui::IsItemClicked(0)) {
				//link_from = reallinkoutput;
				link_from = prefablinkout;
			}
			i++;
		}

	} else {
		// subsys is NULL, do nothing... prefab has no inputs/outputs
	}

	// Draw the output lines
	for (int i=0; i<op->number_of_outputs; i++) {
		LinkOutput *lo = op->default_link_outputs + i;
		for (auto link_end : *lo->inputlinks) {
			linkline_t *ll = (linkline_t *) malloc(sizeof linkline_t);
			ll->from = lo;
			ll->to = link_end;
			linklines.push_back(ll);
		}
	}

	if (op->showtitle) {
		ImGui::SetCursorPos(op->pos + ImVec2(0,-20));
		ImGui::Text("%s", op->name);
	}

	//if ( ! ImGui::IsAnyItemHovered())
	{

		//g.ActiveId = (ImGuiID)this;
		ImGuiID id = (int)op;
		ImGui::PushID(id);
		ImGui::ItemAdd(op_item, &id);



		if (ImGui::IsMouseClicked(0))
			op->imgui_was_dragged = 0;
		if (ImGui::IsMouseDragging(0))
			op->imgui_was_dragged = 1;
		

		//ImGui::Text("g.IO.MouseDownDuration[0] = %.2f", g.IO.MouseDownDuration[0]);

		//if (ImGui::IsMouseReleased(0))
		//	log("released this=%d", this);
		//
		//if (g.IO.MouseDownDuration[0] >= 0.0f)
		//	log("g.IO.MouseDownDuration[0] >= 0.0f %.2f", g.IO.MouseDownDuration[0]);
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0) && g.IO.MouseDownDurationPrev[0] >= 0.0f && !op->imgui_was_dragged) {
			

			if (ImGui::GetIO().KeyCtrl) {
				// ctrl+mouse = add to selection
				op->selected = !op->selected;
			} else {
				// single mouse click = only select clicked one
				cursys->unselectAll();
				op->selected = 1;
			}
			//log("selected = !selected;");
		}

		if (ImGui::IsMouseReleased(0))
			op->imgui_was_dragged = 0;

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
			dragged_op = op;
		}

		if (dragged_op == op) {
			ImGui::SetHoveredID(id);
			if (ImGui::IsMouseDragging(0)) {


				op->pos += ImGui::GetMouseDragDelta();
				//op->selected = 1;

				// if the op was selected, move the whole "selection group", otherwise just the unselected one
				if (op->selected) {

					for (auto i : cursys->all) {
						if (i->selected && i!=op) // dont move twice
							i->pos += ImGui::GetMouseDragDelta();
					}
				}
				ImGui::ResetMouseDragDelta();
			}
		}

		if (ImGui::IsMouseReleased(0))
		{
			dragged_op = NULL;
		}

		ImGui::PopID();
	}
}

char *op_type_to_string(OpType type) {
	char *ret = "fix op_type_to_string";
	switch (type) {
		case OpType::OP_TYPE_SENDER   : ret = "Sender"   ; break;
		case OpType::OP_TYPE_RECEIVER : ret = "Receiver" ; break;
		case OpType::OP_TYPE_REQUESTER: ret = "Requester"; break;
		case OpType::OP_TYPE_DELIVERER: ret = "Deliverer"; break;
		case OpType::OP_TYPE_META     : ret = "Meta"     ; break;
		case OpType::OP_TYPE_NONE     : ret = "None"     ; break;
	}
	return ret;
}

void op_generate_callgraph(Op *op, std::list<Op *> *callgraph) {
	for (int i=0; i<op->number_of_inputs; i++) {
		Link *input = op->default_link_inputs + i;

		for (auto backlink : *input->cached_backlinks) {
			op_generate_callgraph(backlink->owner, callgraph);
			callgraph->push_back(backlink->owner);
			//log("Owner of backlink: %s", backlink->owner->name);
		}
	}
}

void generate_js(Op *op) {
	for (auto g : op->update_ops_this_order)
		log("call: %s", g->name);
}


    bool BeginButtonDropDown(const char* label, ImVec2 buttonSize)
    {
        ImGui::SameLine(0.f, 0.f);

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        //ImGui::GetStateStorage &g = *GImGui;
        const ImGuiStyle& style = GImGui->Style;

        float x = ImGui::GetCursorPosX();
        float y = ImGui::GetCursorPosY();

        ImVec2 size(20, buttonSize.y);
        bool pressed = ImGui::Button("##", size);

        // Arrow
        ImVec2 center(window->Pos.x + x + 10, window->Pos.y + y + buttonSize.y / 2);
        float r = 8.f;
        center.y -= r * 0.25f;
        ImVec2 a = center + ImVec2(0, 1) * r;
        ImVec2 b = center + ImVec2(-0.866f, -0.5f) * r;
        ImVec2 c = center + ImVec2(0.866f, -0.5f) * r;

        window->DrawList->AddTriangleFilled(a, b, c, ImGui::GetColorU32(ImGuiCol_Text));

        // Popup

        ImVec2 popupPos;

        popupPos.x = window->Pos.x + x - buttonSize.x;
        popupPos.y = window->Pos.y + y + buttonSize.y;

        ImGui::SetNextWindowPos(popupPos);

        if (pressed)
        {
            ImGui::OpenPopup(label);
        }

        if (ImGui::BeginPopup(label))
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_Button]);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, style.Colors[ImGuiCol_Button]);
            ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, style.Colors[ImGuiCol_Button]);
            return true;
        }

        return false;
    }

    void EndButtonDropDown()
    {
        ImGui::PopStyleColor(3);
        ImGui::EndPopup();
    }


typedef struct openglmodes {
	char *str;
	int val;
} openglmodes_t;

openglmodes_t glmodes[] = {
	{"GL_POINTS"                           , GL_POINTS						},
	{"GL_LINES"                            , GL_LINES						},
	{"GL_LINE_LOOP"                        , GL_LINE_LOOP					},
	{"GL_LINE_STRIP"                       , GL_LINE_STRIP					},
	{"GL_TRIANGLES"                        , GL_TRIANGLES					},
	{"GL_TRIANGLE_STRIP"                   , GL_TRIANGLE_STRIP				},
	{"GL_TRIANGLE_FAN"                     , GL_TRIANGLE_FAN				},
	{"GL_LINES_ADJACENCY"                  , GL_LINES_ADJACENCY				},
	{"GL_LINE_STRIP_ADJACENCY"             , GL_LINE_STRIP_ADJACENCY		},
	{"GL_TRIANGLES_ADJACENCY"              , GL_TRIANGLES_ADJACENCY			},
	{"GL_TRIANGLE_STRIP_ADJACENCY"         , GL_TRIANGLE_STRIP_ADJACENCY	},
	{"GL_PATCHES"                          , GL_PATCHES						},
	{NULL, NULL}
};

char *glmodetostr(int mode) {
	openglmodes_t *cur = glmodes;
	while (cur->str) {
		if (cur->val == mode) {
			return cur->str;
		}
		cur++;
	}
	return "no glmode found";
}

void op_render_editor(Op *op) {
	ImGui::InputText("Name", op->name, sizeof op->name);
	ImGui::Text("Class %s", op->shortname);
	ImGui::Text("Type %s", op_type_to_string(op->type));
	ImGui::Text("Connections: In=%d Out=%d", op->cache_ingoing_connections, op->cache_outgoing_connections);
	if (ImGui::Button("Update")) {
		op->update(op);
	}
	if (ImGui::Button("List Requesters")) {
		for (auto op_ : op->system->all) {
			int outgoing_links = 0;
			for (int i=0; i<op_->number_of_outputs; i++) {
				LinkOutput *output = op_->default_link_outputs + i;
				outgoing_links += output->inputlinks->size();
			}
			if (outgoing_links == 0)
				log("%s: outgoing links: %d", op_->name, outgoing_links);
		}
	}
	if (ImGui::Button("Show Callgraph")) {
		std::list<Op *> *callgraph = new std::list<Op *>();
		op_generate_callgraph(op, callgraph);
		int j = 0;
		for (auto i : *callgraph) {
			log("callgraph[%d]: %s\n", j, i->name);
			j++;
		}
	}
	if (ImGui::Button("Show Backlinks")) {
			for (int i=0; i<op->number_of_inputs; i++) {
				Link *input = op->default_link_inputs + i;
				int j=0;
				for (auto backlink : *input->cached_backlinks) {
					log("input[%d].backlink[%d]: %s\n", i, j, backlink->owner->name);
					j++;
				}
			}
	}
	ImGui::DragFloat("size x", &op->size.x, 1.0f, 0.0f, 0.0f, "%.3f");
	ImGui::DragFloat("size y", &op->size.y, 1.0f, 0.0f, 0.0f, "%.3f");
	ImGui::DragFloat("pos x", &op->pos.x, 1.0f, 0.0f, 0.0f, "%.3f");
	ImGui::DragFloat("pos y", &op->pos.y, 1.0f, 0.0f, 0.0f, "%.3f");

	for (int i=0; i<op->number_of_inputs; i++) {
		Link *il = op->default_link_inputs + i;
		// only increase il->changed, do not set to 1
		// otherwise previous changed flags will be overwritten (this "bug" made me rewrite this system in C, because I thought it's a initializer bug in C++ lol)
		switch ( il->type ) {
			case OP_TYPE_FLOAT:
				ImGui::PushID(il);
				il->changed += ImGui::DragFloat(il->name, &il->val_f);
				ImGui::PopID();
				break;
			case OP_TYPE_INT:
				ImGui::PushID(il);
				il->changed += ImGui::DragInt(il->name, &il->val_i);
				ImGui::PopID();
				break;
			case OP_TYPE_MATRIX:
				ImGui::PushID(il);				
				ImGui::PushItemWidth(50);
				il->changed += ImGui::DragFloat("aa", il->matrix +  0 + 0, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("ab", il->matrix +  4 + 0, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("ac", il->matrix +  8 + 0, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("ad", il->matrix + 12 + 0, 0.01);
				il->changed += ImGui::DragFloat("ba", il->matrix +  0 + 1, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("bb", il->matrix +  4 + 1, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("bc", il->matrix +  8 + 1, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("bd", il->matrix + 12 + 1, 0.01);
				il->changed += ImGui::DragFloat("ca", il->matrix +  0 + 2, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("cb", il->matrix +  4 + 2, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("cc", il->matrix +  8 + 2, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("cd", il->matrix + 12 + 2, 0.01);
				il->changed += ImGui::DragFloat("da", il->matrix +  0 + 3, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("db", il->matrix +  4 + 3, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("dc", il->matrix +  8 + 3, 0.01); ImGui::SameLine(0, 5);
				il->changed += ImGui::DragFloat("dd", il->matrix + 12 + 3, 0.01);
				ImGui::PopItemWidth();
				ImGui::PopID();
				break;
			case OP_TYPE_VERTEXSHADER:
			case OP_TYPE_FRAGMENTSHADER:
			case OP_TYPE_PROGRAM:
			case OP_TYPE_GLBUFFER:
			case OP_TYPE_GLATTRIB:
			case OP_TYPE_GLUNIFORM:
				ImGui::PushID(il);
				il->changed += ImGui::DragInt(il->name, (int *)&il->opengl_id);
				ImGui::PopID();
				break;
			case OP_TYPE_GLDRAWMODE: {
				ImGui::PushID(il);
				il->changed += ImGui::DragFloat(il->name,&il->val_f);

				ImGui::Text("Mode: %s", glmodetostr((int)il->val_f));

				// fuckin shit doesnt work
				//ImVec2 size(200, 20);
				////ImGui::Button(glmodetostr(il->val_i), size);
				//ImGui::Button("muh", size);
				//if (BeginButtonDropDown("##sprtdd", size)) {
				//	openglmodes_t *cur = glmodes;
				//	while (cur->str) {
				//		ImGui::PushID(cur);
				//		if (ImGui::Button(cur->str, size)) {
				//			log("test1 %d\n", cur->val);
				//		}
				//		ImGui::PopID();
				//		cur++;
				//	}
				//	EndButtonDropDown();
				//}

				ImGui::PopID();
				break;
			}

			default:
				ImGui::PushID(il);
				ImGui::Text("op_render_editor: input[%d].type: %d", i, il->type);
				//ImGui::DragFloat(il->name, &il->val_f);
				ImGui::PopID();
				
		}	// end switch

		if (il->changed) {
			il->owner->forcereload++;
		}
	} // end for

	//if (op_did_inputs_change(op)) {
	//	op->forcereload++;
	//	op_set_inputs_unchanged(op);
	//}
	
}

static int between(int min, int val, int max) {
	return (min <= val) && (max >= val);
}

LinkOutput *op_getOutput(Op *op, int id) {
	if ( ! between(0, id, op->number_of_outputs) )
		return NULL;
	return &op->default_link_outputs[id];
}
Link *op_getInput(Op *op, int id) {
	if ( ! between(0, id, op->number_of_inputs) )
		return NULL;
	return &op->default_link_inputs[id];
}
void op_render    (Op *op         ) { log("op_render    () is not implemented for Op%s", op->name); }
void op_update    (Op *op         ) { log("op_update    () is not implemented for Op%s", op->name); }




void op_dump_file (Op *op, FILE *f) {
	fprintf(f, "%s pos %.0f %.0f size %.0f %.0f selected %d perm %d name %s ",
		op->shortname,
		op->pos.x,
		op->pos.y,
		op->size.x,
		op->size.y,
		op->selected,
		op->is_permanent,
		op->name
	);
	op->write_extra_data(op, f);
	fprintf(f, "\n");
}
void op_dump_links(Op *op, FILE *f) {
	for (int i=0; i<op->number_of_outputs; i++) {
		LinkOutput *lo = op->default_link_outputs + i;
		for (auto link : *lo->inputlinks) {
			fprintf(f, "Link %s [%d] to %s [%d]\n",
					op->name,
					i,
					link->owner->name,
					link->local_id
			);
		}
	
	}
}

void op_resize_minimal(Op *op) {
	op->size.x = (float)strlen(op->name) * 7.0f;
	if (op->size.x < 60.0f)
		op->size.x = 60.0f;
	int max = op->number_of_inputs;
	if (op->number_of_outputs > max)
		max = op->number_of_outputs;
	op->size.y = (float)max * 20.0f;
}

void op_delete_all_links(Op *op) {
	// delete out links
	op->default_link_outputs->inputlinks->clear();

	// delete all in links
	for (int i=0; i<op->number_of_inputs; i++) {
		Link *in = op->default_link_inputs + i;

		for (auto output : *in->cached_backlinks) {
			output->inputlinks->remove(in);
		}
	}
}

void op_destroy(Op *op) {
	delete op;
}



void op_write_extra_data(Op *op, FILE *f) {}
void op_read_extra_data(Op *op, char *line) {}

void op_on_link_connect(Op *op, int id) {}
void op_on_link_disconnect(Op *op, int id) {}
void op_on_link_update(Op *op, int id) {}