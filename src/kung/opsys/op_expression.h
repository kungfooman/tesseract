#ifndef OP_EXPRESSION_H
#define OP_EXPRESSION_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Expression"
#define CLASSNAME OpExpression
#define METHOD(name) op_expression_##name
#define STRUCTNAME struct op_expression_s

STRUCTNAME {
	#include "op_base.h"
	char code[128];
	int execid;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);

#define IS_EXPRESSION(op)   ( (int)op->update == (int)op_expression_update   )

#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif