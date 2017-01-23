#ifndef OP_ASTAR_H
#define OP_ASTAR_H

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "AStar"
#define CLASSNAME OpAStar
#define METHOD(name) op_astar_##name
#define STRUCTNAME struct op_astar_s

#include "op.h"


class AstarNode {
public:
	float x,y,z;
	GLuint buffer;
	float data[12 * 9];
};

STRUCTNAME {
	#include "op_base.h"
	float &x = default_link_inputs[0].val_f;
	float &y = default_link_inputs[1].val_f;
	float &z = default_link_inputs[2].val_f;
	float &n = default_link_inputs[3].val_f;
	Mesh *mesh;
	
	//float data[72];
	float data[12 * 9];

	std::vector<AstarNode *> nodes;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);
	  //
      void METHOD(add_node          )(CLASSNAME *op, float x, float y, float z);


#define IS_ASTAR(op)   ( (int)op->update == (int)op_astar_update   )

#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif