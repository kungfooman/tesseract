#ifndef OP_PREFAB_OUTPUT_H
#define OP_PREFAB_OUTPUT_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "PrefabOutput"
#define CLASSNAME OpPrefabOutput
#define METHOD(name) op_prefab_output_##name
#define STRUCTNAME struct op_prefab_output_s

STRUCTNAME {
	#include "op_base.h"
	int outputid;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);

#define IS_PREFAB_OUTPUT(op)   ( (int)op->update == (int)op_prefab_output_update   )

#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif