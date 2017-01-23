#define _CRT_SECURE_NO_WARNINGS

#ifndef OP_H
#define OP_H

//#include "craftstructs.h"
#include "include_imgui.h"

//#include "imgui_impl_glfw.h"
#include <stdio.h>
//#include "libs\glfw\include\GLFW/glfw3.h"

//#include "glfw\include\GLFW/glfw3.h"
#include <Windows.h>
#include <list>
#include <queue>

#include <kung/mesh.h>
#include <kung/include_glm.h>
#include <kung/include_physfs.h>
// meh
#define FILE PHYSFS_File
#define fprintf PHYSFS_fprintf
#define fclose PHYSFS_close

// 0 = none 1 = float, 1 = int, 2 = string, 3=whatever
#define OP_TYPE_FLOAT 1
#define OP_TYPE_INT 2
#define OP_TYPE_MATRIX 3
#define OP_TYPE_MESH 4
#define OP_TYPE_VERTEXSHADER 5
#define OP_TYPE_FRAGMENTSHADER 6
#define OP_TYPE_PROGRAM 7
#define OP_TYPE_GLBUFFER 8
#define OP_TYPE_PREFAB_INPUT 9
#define OP_TYPE_PREFAB_OUTPUT 10
#define OP_TYPE_GLATTRIB 11
#define OP_TYPE_GLUNIFORM 12
#define OP_TYPE_GLDRAWMODE 13

void DrawLine(float a_x, float a_y, float b_x, float b_y);
void DrawRect(float a_x, float a_y, float b_x, float b_y);
int log(char *format, ...);

struct op_s;

class LinkOutput;

struct link_s {
	// management stuff
	ImVec2 pos; // so a node knows where to draw the line to (and from, calculated only once) could even be dynamically located per editor
	struct op_s *owner;
	int local_id = 0; // e.g. Color Op: r=0 g=1 b=2 a=3, to reassign the links at save/load time
	char name[16];
	int changed = 1; // OpValue will set this to 1 if changed 
	int type = OP_TYPE_FLOAT;
	std::list<LinkOutput *> *cached_backlinks;
	
	// Actual data, the same data needs to be in LinkOutput, because an Op only saves the calculated data in its LinkOutputs
	// The OpSystem::Update() will then distribute the LinkOutput value into the LinkInputs
	float val_f = 0.0;
	int val_i = 0;
	char val_str[128] = {"no text set"};
	float matrix[16];
	glm::mat4x4 glm_matrix;
	Mesh *mesh = NULL;
	GLuint opengl_id = 0; 	// make advanced classes for Program/Vertex/Fragment?



	void clean() {
		//val_f = 0.0;
		//val_i = 0;
		//strcpy(val_str, "no text set");
		opengl_id = 0;
	}
};

typedef struct link_s Link;


typedef struct linkline_s {
	LinkOutput *from;
	Link *to;
	//ImVec2 *line_start;
	//ImVec2 *line_end;
} linkline_t;

extern std::list<linkline_t *> linklines;

extern LinkOutput *link_from;
extern Link *link_to;

class LinkOutput {
public:
	std::list<Link *> *inputlinks;
	ImVec2 pos;
	char name[64];
	int type;
	struct op_s *owner;
	int local_id; // prefab remapping
	//void setMatrix(glm::mat4x4 matrix);
	void setMatrix(float *matrix);
	int changed = 0;

	// Actual data, the same data needs to be in LinkOutput, because an Op only saves the calculated data in its LinkOutputs
	// The OpSystem::Update() will then distribute the LinkOutput value into the LinkInputs
	float val_f = 0.0;
	int val_i = 0;
	char val_str[128] = {"no text set"};
	float matrix[16];
	glm::mat4x4 glm_matrix;
	Mesh *mesh = NULL;
	GLuint opengl_id = 0; 	// make advanced classes for Program/Vertex/Fragment?
};

typedef enum {
	OP_TYPE_NONE,		// Set op->make_sender_when_type_none=1 in op_$type_init(), e.g. OpCheckerboard doesn't work otherwise
	OP_TYPE_SENDER,		// update() is called every system->update(). An Op is a Sender when it has 0 incoming connections (OpKeyboard is always a Sender).
	OP_TYPE_RECEIVER,	// op->update() is called by a Sender, then the Receiver actively calls update() on choosen childs to keep the flow (e.g. OpSwitch)
	OP_TYPE_REQUESTER,	// A op without outputs is often a Requester, the Requester has the call graph for all Deliverers and calls them on op->request(op)
	OP_TYPE_DELIVERER,	// A Deliverer only updates the values on the outgoing connections, will not call any other op->update().
	OP_TYPE_META		// These Op's help to add meta information to an OpSystem, e.g. OpExpression, OpPrefabInput, OpPrefabOutput
} OpType;

class OpSystem;

// don't even bother to try initialise values here, it just will be malloc()'ed
#undef STRUCTNAME
#undef SHORTNAME
//#define FUCKYOU "fuckyou"
#define STRUCTNAME struct op_s
#define SHORTNAME "Op"
struct op_s {
#include "op_base.h"
};
//#undef FUCKYOU
//#undef STRUCTNAME

typedef struct op_s Op;

// each class can assign id's to their individual Link's, maybe I can make it way more generic at some point, but specialized is nice as well
LinkOutput *	op_getOutput(Op *op, int id);
		Link *	op_getInput(Op *op, int id);	
		void	op_init(Op *op);
		void	op_init_link(Op *op, int id, char *name, int type /* ,..., maybe as default value */);
		void	op_init_link_output(Op *op, int id, char *name, int type /* , ..., maybe as default value */);
		int		op_did_inputs_change(Op *op);
		void	op_set_inputs_unchanged(Op *op);
		void	op_pre_render(Op *op);
		void	op_post_render(Op *op);
		void	op_render(Op *op);
		void	op_update(Op *op);
		void	op_changed(Op *op);
		void	op_render_editor(Op *op);
		void	op_dump_file(Op *op, FILE *f);
		void	op_dump_links(Op *op, FILE *f);
		void	op_resize_minimal(Op *op);
		void	op_destroy(Op *op);
		void	op_request(Op *op);
		void	op_generate_callgraph(Op *op, std::list<Op *> *callgraph);
		void	op_write_extra_data(Op *op, FILE *f);
		void	op_read_extra_data(Op *op, char *line);
		void	op_delete_all_links(Op *op);
		void	op_on_link_connect(Op *op, int id);
		void	op_on_link_disconnect(Op *op, int id);
		void	op_on_link_update(Op *op, int id);

extern Op *dragged_op;

#endif