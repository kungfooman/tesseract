#ifndef OP_XMODEL_H
#define OP_XMODEL_H

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "XModel"
#define CLASSNAME OpXModel
#define METHOD(name) op_xmodel_##name
#define STRUCTNAME struct op_xmodel_s



#include "op.h"

#include "format_cod2_xmodel.h"

STRUCTNAME {
	#include "op_base.h"
	float &x = default_link_inputs[0].val_f;
	float &y = default_link_inputs[1].val_f;
	float &z = default_link_inputs[2].val_f;
	float &n = default_link_inputs[3].val_f;
	Mesh *mesh;
	
	//float data[72];
	float data[12 * 9];
	char filename[256];
	Format::CallOfDuty2::XModel *xmodel = NULL;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);
      void METHOD(draw_in_scene     )(CLASSNAME *op);
	  void METHOD(render_legacy_code)(CLASSNAME *op);


#define IS_XMODEL(op)   ( (int)op->update == (int)op_xmodel_update   )

#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif