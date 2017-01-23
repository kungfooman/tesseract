#ifndef OP_MESH_EDITOR_H
#define OP_MESH_EDITOR_H

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "MeshEditor"
#define CLASSNAME OpMeshEditor
#define METHOD(name) op_mesh_editor_##name
#define STRUCTNAME struct op_mesh_editor_s

#include "op.h"

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