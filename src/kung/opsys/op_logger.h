#ifndef OP_LOGGER_H
#define OP_LOGGER_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Logger"
#define CLASSNAME OpLogger
#define METHOD(name) op_logger_##name
#define STRUCTNAME struct op_logger_s

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