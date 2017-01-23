#ifndef OP_CRAFT_H
#define OP_CRAFT_H

#include "op.h"
#include "op_craft.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Craft"
#define CLASSNAME OpCraft
#define METHOD(name) op_craft_##name
#define STRUCTNAME struct op_craft_s

STRUCTNAME {
	#include "op_base.h"
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