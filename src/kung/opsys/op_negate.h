#ifndef OP_Negate_H
#define OP_Negate_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Negate"
#define CLASSNAME OpNegate
#define METHOD(name) op_negate_##name
#define STRUCTNAME struct op_negate_s

STRUCTNAME {
	#include "op_base.h"
	float &val = default_link_inputs[0].val_f;
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