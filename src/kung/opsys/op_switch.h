#ifndef OP_SWITCH_H
#define OP_SWITCH_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Switch"
#define CLASSNAME OpSwitch
#define METHOD(name) op_switch_##name
#define STRUCTNAME struct op_switch_s

STRUCTNAME {
	#include "op_base.h"
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new             )();
      void METHOD(init            )(CLASSNAME *op);
      void METHOD(render          )(CLASSNAME *op);
      void METHOD(update          )(CLASSNAME *op);
      void METHOD(render_editor   )(CLASSNAME *op);

#define IS_SWITCH(op)   ( (int)op->update == (int)op_switch_update   )

#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif