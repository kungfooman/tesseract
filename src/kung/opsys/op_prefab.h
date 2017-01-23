#ifndef OP_PREFAB_H
#define OP_PREFAB_H

#include "op.h"
#include <vector>

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Prefab"
#define CLASSNAME OpPrefab
#define METHOD(name) op_prefab_##name
#define STRUCTNAME struct op_prefab_s

STRUCTNAME {
	#include "op_base.h"
	OpSystem *subsys;
	std::vector<LinkOutput *> internal_outputs;
	char filename[64];
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new             )();
      void METHOD(init            )(CLASSNAME *op);
      void METHOD(render          )(CLASSNAME *op);
      void METHOD(update          )(CLASSNAME *op);
      void METHOD(render_editor   )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);

#define IS_PREFAB(op)   ( (int)op->update == (int)op_prefab_update   )

#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif