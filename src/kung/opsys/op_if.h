#ifndef OP_IF_H
#define OP_IF_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "If"
#define CLASSNAME OpIf
#define METHOD(name) op_if_##name
#define STRUCTNAME struct op_if_s

STRUCTNAME {
	#include "op_base.h"
	float &val_a = default_link_inputs[0].val_f;
	float &val_b = default_link_inputs[1].val_f;
	float tmp_val;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new             )();
      void METHOD(init            )(CLASSNAME *op);
      void METHOD(render          )(CLASSNAME *op);
      void METHOD(update          )(CLASSNAME *op);
      void METHOD(render_editor   )(CLASSNAME *op);

#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif