#ifndef OP_CHECKERBOARD_H
#define OP_CHECKERBOARD_H

#define SHORTNAME "Checkerboard"
#define CLASSNAME OpCheckerboard
#define METHOD(name) op_checkerboard_##name
#define STRUCTNAME struct op_checkerboard_s

#include "op.h"

STRUCTNAME {
	#include "op_base.h"
	int textureid;
	GLubyte *checkImage = NULL;
	int &width  = default_link_inputs[0].val_i;
	int &height = default_link_inputs[1].val_i;
	int &and_i  = default_link_inputs[2].val_i;
	int &and_j  = default_link_inputs[3].val_i;
	//int *ptr_width ;
	//int *ptr_height;
	//int *ptr_and_i ;
	//int *ptr_and_j ;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);
      void METHOD(makeCheckImage    )(CLASSNAME *op);
      void METHOD(generate_texture  )(CLASSNAME *op);



#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif