#ifndef OP_CUBE_H
#define OP_CUBE_H

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Cube"
#define CLASSNAME OpCube
#define METHOD(name) op_cube_##name
#define STRUCTNAME struct op_cube_s

#include "op.h"

STRUCTNAME {
	#include "op_base.h"
	float &x = default_link_inputs[0].val_f;
	float &y = default_link_inputs[1].val_f;
	float &z = default_link_inputs[2].val_f;
	float &n = default_link_inputs[3].val_f;
	Mesh *mesh;
	
	//float data[72];
	float data[12 * 9];
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);
      void METHOD(draw_in_scene     )(CLASSNAME *op);


#define IS_CUBE(op)   ( (int)op->update == (int)op_cube_update   )

#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif