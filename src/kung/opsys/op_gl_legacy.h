#ifndef OP_GL_LEGACY_H
#define OP_GL_LEGACY_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "GLLegacy"
#define CLASSNAME OpGLLegacy
#define METHOD(name) op_gl_legacy_##name
#define STRUCTNAME struct op_gl_legacy_s

STRUCTNAME {
	#include "op_base.h"
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new             )();
      void METHOD(init            )(CLASSNAME *op);
      void METHOD(render          )(CLASSNAME *op);
      void METHOD(update          )(CLASSNAME *op);
      void METHOD(render_editor   )(CLASSNAME *op);
      void METHOD(render_legacy_code)(CLASSNAME *op);


#define IS_GL_LEGACY(op)   ( (int)op->update == (int)op_gl_legacy_update   )

#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif