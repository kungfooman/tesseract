#ifndef OP_VALUE_H
#define OP_VALUE_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Value"
#define CLASSNAME OpValue
#define METHOD(name) op_value_##name
#define STRUCTNAME struct op_value_s

STRUCTNAME {
	#include "op_base.h"
	float &val = default_link_inputs[0].val_f;
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