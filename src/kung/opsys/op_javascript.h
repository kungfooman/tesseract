#ifndef OP_JAVASCRIPT_H
#define OP_JAVASCRIPT_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "JavaScript"
#define CLASSNAME OpJavaScript
#define METHOD(name) op_javascript_##name
#define STRUCTNAME struct op_javascript_s

STRUCTNAME {
	#include "op_base.h"
	char code[128] = {"log(\"JS\")"};
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