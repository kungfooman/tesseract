#ifndef OP_RANDOM_SHIT_H
#define OP_RANDOM_SHIT_H

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "RandomShit"
#define CLASSNAME OpRandomShit
#define METHOD(name) op_random_shit_##name
#define STRUCTNAME struct op_random_shit_s

#include "op.h"
#include "op_random_shit.h"

STRUCTNAME {
	#include "op_base.h"
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);



#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif