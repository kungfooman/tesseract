#include <kung/include_imgui.h>
#include "opsystem_imgui_console.h" // log()
#include <vector>
#include "opsystem_hierarchy.h"

int next_node_id = 0;
#include <algorithm>


#include <kung/include_duktape.h>
#include "op.h"

Node::Node(char *name_, char *opsystem_filename_) : opsystem(opsystem_filename_)
{
	//opsystem = new OpSystem((char *)opsystem_filename_.c_str());
	id = next_node_id++;
	strcpy(name, name_);
	//strcpy(opsystem->name, opsystem_filename_.c_str());
}

	int Node::hasNodeAsParent(Node *node) {
		Node *currentNode = this;
		do {
			if (currentNode == node)
				return true;
		} while (currentNode = currentNode->parent);
		return false;
	}
	
	int Node::detachFromParent() {
		if ( ! this->parent)
			return true;
		//int idx = this->parent->childs.remo(this)
		
		// remove this node out of this->parent->childs

		// whatever: http://stackoverflow.com/questions/39912/how-do-i-remove-an-item-from-a-stl-vector-with-a-certain-value
		//auto &parentchilds = this->parent->childs;

		//auto ret1 = std::remove(childs.begin(), childs.end(), this);
		//// Return Value: An iterator pointing to the new location of the element that followed the last element erased by the function call.
		//// This is the container end if the operation erased the last element in the sequence.
		//
		//auto ret = childs.erase(ret1, childs.end());
		int i=0;
		for (auto it : this->parent->childs)
		{
			if (it == this) {
				this->parent->childs.erase(this->parent->childs.begin() + i);
				break;
			}
			i++;
		}

		//auto toRemove = std::find(this->parent->childs.begin(), this->parent->childs.end(), this);
		//this->parent->childs.erase(toRemove);
		//
		//if (toRemove != this->parent->childs.end()) {
		//	log("successfully deleted node out of parents childs array\n");
		//} else {
		//	log("warning, %s is child of %s, but not in the parents childs array\n", this->name.c_str(), this->parent->name.c_str());
		//}

		// fuckin bullshit, no clue how to check if the container actually deleted something.. cba for std::find now
		return 1;


		//if (idx != -1) {
		//	this.parent.childs.splice(idx, 1) // remove inplace
		//	return true;
		//}
		//log("warning, %s is child of %s, but not in the parents childs array\n", this->name.c_str(), this->parent->name.c_str());
		return false;
	}

int Node::addChild(Node *child) {
	if (this->hasNodeAsParent(child)) {
		log("Can't add a parent as child\n");
		return 0;
	}
	child->detachFromParent();
	child->parent = this;
	childs.push_back(child);
	return 1;
}

Node *root = NULL;

Node *selected_node = NULL;
Node *node_current_rendering = NULL;

extern int treenode_toggled;
extern int framecounter;
Node *node_dragged = NULL;
Node *node_dragged_target = NULL;

void render_subnodes(Node *node) {
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;

	if (selected_node && node==selected_node)
	//if (node->selected)
		node_flags ^= ImGuiTreeNodeFlags_Selected;

	//ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()*3); // Increase spacing to differentiate leaves from expanded contents.

	// Node
	node_current_rendering = node;
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)node, node_flags, "%s", node->name);
	node_current_rendering = NULL;

	if (ImGui::IsItemHoveredRect() && ImGui::IsMouseDragging(0, 1.0f) && node_dragged==NULL) {
		//log("drag node: %s\n",)
		node_dragged = node;
	}

	if (ImGui::IsItemHoveredRect() && ImGui::IsMouseReleased(0) && node_dragged) {
		log("add %s to %s\n", node_dragged->name, node->name);

		node_dragged_target = node;
		//node_dragged = NULL;
	}
	
	if (treenode_toggled==0 && ImGui::IsItemClicked()) {
		//node_clicked = i;
		//node->selected = !node->selected; // this is set in ImGui.cpp aswell, to TOGGLE it... so fucked up... framecounter has a difference of 2-6 frames
		
		// either deselect, if already selected, or select the node
		if (selected_node && node==selected_node)
			selected_node = NULL;
		else {
			selected_node = node;
			OpSystem::SetCurrent(&selected_node->opsystem);
		}
		//log("node clicked: %d framecounter=%d\n", treenode_toggled, framecounter);
	}
	if (node_open)
	{		
		for (auto child : node->childs) {
			render_subnodes(child);
		}

		ImGui::TreePop();
	}



	//ImGui::PopStyleVar();
}

void update_nodes(Node *node) {
	node->opsystem.update();
	for (auto child : node->childs) {
		update_nodes(child);
	}
}

void update_nodes_3d(Node *node) {
	for (auto renderable : node->opsystem.renderables) {
		renderable->draw_in_scene(renderable);
	}
	//for (auto cube : node->opsystem.cubes) {
	//	op_cube_draw_in_scene(cube, NULL);
	//}
	for (auto child : node->childs) {
		update_nodes_3d(child);
	}
}

void render_root() {
	render_subnodes(root);

	if (node_dragged && node_dragged_target) {
		node_dragged_target->addChild( node_dragged );
		node_dragged = NULL;
		node_dragged_target = NULL;
	}

	if (ImGui::IsMouseReleased(0)) {
		node_dragged = NULL;
		node_dragged_target = NULL;
	
	}
}

void dump_node(Node *node, PHYSFS_File *f) {

	if (node != root) {
		PHYSFS_fprintf(f, "\ttmp = new Node(\"%s\", \"%s\")\n", node->name, node->opsystem.name);
		PHYSFS_fprintf(f, "\tstack.peek().addChild( tmp );\n");
		PHYSFS_fprintf(f, "\tstack.push(tmp);\n");
	}
	for (auto child : node->childs) {
		dump_node(child, f);
	}

	if (node != root)
		PHYSFS_fprintf(f, "\tstack.pop(); // %s\n", node->name);
}
void save_nodes(char *filename) {
	char *savepath = "javascript/hierarchy_dumped.js";
	PHYSFS_File *f = PHYSFS_openWrite(savepath);

	if (f == NULL) {
		log("Can't open %s for writing\n", savepath);
		return;
	}

	PHYSFS_fprintf(f, "function add_hierarchy_to_root() {\n");
	PHYSFS_fprintf(f, "\tcreate_hierarchy(root);\n");
	PHYSFS_fprintf(f, "}\n");

	PHYSFS_fprintf(f, "function create_hierarchy(add_to) {\n");
	PHYSFS_fprintf(f, "\tstack = new Stack();\n");
	PHYSFS_fprintf(f, "\tstack.push(add_to);\n\n");
	dump_node(root, f);
	PHYSFS_fprintf(f, "\n\tstack.pop(); // pop add_to, not really needed\n");
	PHYSFS_fprintf(f, "}\n");
	PHYSFS_close(f);
	log("Successfully wrote %s\n", savepath);
}
void load_nodes(char *filename) {
	js_call("add_hierarchy_to_root", "");
}
void render_hierarchy() {

	static int first = 1;



	if (first) {
		//Node *a = new Node("a");
		//Node *aa = new Node("aa");
		//a->addChild(aa);
		//Node *b = new Node("b");
		//Node *bb = new Node("bb");
		//b->addChild(bb);
		//Node *c = new Node("c");
		//Node *cc = new Node("cc");
		//c->addChild(cc);
		//root.addChild(a);
		//root.addChild(b);
		//root.addChild(c);
		//js_call("add_hierarchy_to_root", "");
		first = 0;
	}
	
	if (ImGui::Button("New")) {
		Node *newNode = new Node("newchild", "opsystem/default.opsys");
		if (selected_node)
			selected_node->addChild(newNode);
		else
			root->addChild(newNode);
	}
	ImGui::SameLine(0, 5);
	if (ImGui::Button("Delete")) {
		if (selected_node) {
			if (selected_node != root)
			{
				selected_node->detachFromParent();
				delete selected_node;
				selected_node = NULL;
			} else {
				log("cant delete root node\n");
			}
			
		} else {
			log("no node selected\n");
		}
	}
	static char filename[128] = {"nodes.txt"};
	ImGui::InputText("filename", filename, sizeof filename);
	if (ImGui::Button("Save")) {
		save_nodes(filename);
	}
	if (ImGui::Button("Load")) {
		load_nodes(filename);
	}
	
	render_root();
	ImGui::Text("Node.id = %d", root->id);
	if (node_dragged)
		ImGui::Text("node_dragged=%s", node_dragged->name);
}



int duk_func_node_new(duk_context *ctx) {
	char *name = (char *)duk_to_string(ctx, 0);
	char *opsystem_filename = (char *)duk_to_string(ctx, 1);
	Node *newNode = new Node(name, opsystem_filename);
	duk_push_int(ctx, (int)newNode);
	return 1;
}
int duk_func_node_get_root(duk_context *ctx) {
	duk_push_int(ctx, (int)root);
	return 1;
}
int duk_func_node_get_selected_node(duk_context *ctx) {
	duk_push_int(ctx, (int)selected_node);
	return 1;
}
int duk_func_node_add_child(duk_context *ctx) {
	
	Node *parent = (Node *)duk_to_int(ctx, 0);
	Node *newChild = (Node *)duk_to_int(ctx, 1);

	if (parent == NULL) {
		log("node_add_child: parent == NULL\n");
		return 0;
	}

	int ret = parent->addChild( newChild );
	duk_push_int(ctx, ret);
	return 1;
}

void opsystem_hierarchy_add_javascript_bindings() {
	struct funcis funcs[] = {
		{"node_new"                  , duk_func_node_new                },
		{"node_get_root"             , duk_func_node_get_root           },
		{"node_get_selected_node"    , duk_func_node_get_selected_node  },
		{"node_add_child"            , duk_func_node_add_child          },
		{NULL, NULL}
	};

	for (int i=0; funcs[i].name; i++) {
		js_register_function(ctx, funcs[i].name, funcs[i].func);
	}

}