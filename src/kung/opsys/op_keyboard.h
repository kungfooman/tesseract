#ifndef OP_KEYBOARD_H
#define OP_KEYBOARD_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Keyboard"
#define CLASSNAME OpKeyboard
#define METHOD(name) op_keyboard_##name
#define STRUCTNAME struct op_keyboard_s

STRUCTNAME {
	#include "op_base.h"
	int textureid;
	GLubyte *checkImage = NULL;
	std::list<char *> events;
};

typedef STRUCTNAME CLASSNAME;

extern std::list<OpKeyboard *> keyboards;

CLASSNAME *METHOD(new             )();
      void METHOD(init            )(CLASSNAME *op);
      void METHOD(render          )(CLASSNAME *op);
      void METHOD(update          )(CLASSNAME *op);
      void METHOD(render_editor   )(CLASSNAME *op);
      void METHOD(destroy         )(CLASSNAME *op);

#define IS_KEYBOARD(op) ( (int)op->update == (int)op_keyboard_update )

#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif