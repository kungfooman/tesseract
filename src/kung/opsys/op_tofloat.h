#ifndef OP_TOFLOAT_H
#define OP_TOFLOAT_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "ToFloat"
#define CLASSNAME OpToFloat
#define METHOD(name) op_tofloat_##name
#define STRUCTNAME struct op_tofloat_s

STRUCTNAME {
	#include "op_base.h"
	char code[128];
	float &val = default_link_outputs[0].val_f;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);


#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif