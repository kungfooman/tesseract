#include "op_all.h"

#include "op_cube.h"
#include "op_mesh_editor.h"
#include "op_renderer.h"
#include "op_random_shit.h"
//#include "op_timer.h"
//#include "op_logger.h"
#include "op_checkerboard.h"
#include "op_keyboard.h"
#include "op_logger.h"
#include "op_switch.h"
#include "op_javascript.h"
#include "op_craft.h"
#include "op_perspective.h"
#include "op_matrix_ortho.h"
#include "op_matrix.h"
#include "op_matrix_multiply.h"
#include "op_matrix_rotate.h"
#include "op_matrix_translate.h"
#include "op_matrix_scale.h"
#include "op_matrix_identity.h"
#include "op_value.h"
#include "op_divide.h"
#include "op_multiply.h"
#include "op_plus.h"
#include "op_minus.h"
#include "op_sin.h"
#include "op_cos.h"
#include "op_negate.h"
#include "op_if.h"
#include "op_expression.h"
#include "op_tofloat.h"
#include "op_gl_fragment_shader.h"
#include "op_gl_vertex_shader.h"
#include "op_gl_program.h"
#include "op_gl_framebuffer.h"
#include "op_prefab.h"
#include "op_prefab_input.h"
#include "op_prefab_output.h"
#include "op_gl_buffer.h"
#include "op_gl_attrib.h"
#include "op_gl_uniform.h"
#include "op_astar.h"
#include "op_soil.h"
#include "op_file.h"
#include "op_xmodel.h"
#include "op_gl_legacy.h"
//#include "op_helix2.h"
//#include "op_helix.h"
//#include "op_color.h"
//#include "op_value.h"
//#include "op_perspective.h"

//Op *create_op_by_name(char *name) {
//
//}

oplist_t available_ops[] = {
	//{ "Timer", create_timer },
	{ "Checkerboard"      , (Op *(*)())op_checkerboard_new         },
	{ "Keyboard"          , (Op *(*)())op_keyboard_new             },
	{ "Logger"            , (Op *(*)())op_logger_new               },
	{ "Switch"            , (Op *(*)())op_switch_new               },
	{ "JavaScript"        , (Op *(*)())op_javascript_new           },
	{ "Craft"             , (Op *(*)())op_craft_new                },
	{ "Perspective"       , (Op *(*)())op_perspective_new          },
	{ "Ortho"             , (Op *(*)())op_ortho_new                },
	{ "Matrix"            , (Op *(*)())op_matrix_new               },
	{ "MatrixMultiply"    , (Op *(*)())op_matrix_multiply_new      },
	{ "MatrixRotate"      , (Op *(*)())op_matrix_rotate_new        },
	{ "MatrixTranslate"   , (Op *(*)())op_matrix_translate_new     },
	{ "MatrixScale"       , (Op *(*)())op_matrix_scale_new         },
	{ "MatrixIdentity"    , (Op *(*)())op_matrix_identity_new      },
	{ "Value"             , (Op *(*)())op_value_new                },
	{ "Divide"            , (Op *(*)())op_divide_new               },
	{ "Multiply"          , (Op *(*)())op_multiply_new             },
	{ "Plus"              , (Op *(*)())op_plus_new                 },
	{ "Minus"             , (Op *(*)())op_minus_new                },
	{ "Sin"               , (Op *(*)())op_sin_new                  },
	{ "Cos"               , (Op *(*)())op_cos_new                  },
	{ "Negate"            , (Op *(*)())op_negate_new               },
	{ "If"                , (Op *(*)())op_if_new                   },
	{ "Expression"        , (Op *(*)())op_expression_new           },
	{ "ToFloat"           , (Op *(*)())op_tofloat_new              },
	{ "Cube"              , (Op *(*)())op_cube_new                 },
	{ "MeshEditor"        , (Op *(*)())op_mesh_editor_new          },
	{ "Renderer"          , (Op *(*)())op_renderer_new             },
	{ "RandomShit"        , (Op *(*)())op_random_shit_new          },
	{ "FragmentShader"    , (Op *(*)())op_fragment_shader_new      },
	{ "VertexShader"      , (Op *(*)())op_vertex_shader_new        },
	{ "Program"           , (Op *(*)())op_program_new              },
	{ "Framebuffer"       , (Op *(*)())op_framebuffer_new          },
	{ "Prefab"            , (Op *(*)())op_prefab_new               },
	{ "PrefabInput"       , (Op *(*)())op_prefab_input_new         },
	{ "PrefabOutput"      , (Op *(*)())op_prefab_output_new        },
	{ "GLBuffer"          , (Op *(*)())op_gl_buffer_new            },
	{ "GLAttrib"          , (Op *(*)())op_gl_attrib_new            },
	{ "GLUniform"         , (Op *(*)())op_gl_uniform_new           },
	{ "AStar"             , (Op *(*)())op_astar_new                },
	{ "SOIL"              , (Op *(*)())op_soil_new                 },
	{ "File"              , (Op *(*)())op_file_new                 },
	{ "XModel"            , (Op *(*)())op_xmodel_new               },
	{ "GLLegacy"          , (Op *(*)())op_gl_legacy_new            },
	//{ "Helix", (Op *(*)())op_helix_new },
	//{ "Helix", create_helix },
	//{ "Color", create_color },
	NULL
};

OpMatrix           *craft_matrix_persp        = NULL;

void init_systems();

#if 0
void InitPermanentOps() {
	log("Init permanent ops...\n");

	craft_matrix_persp      = op_matrix_new();
	
	perm.add((Op *)craft_matrix_persp        );

	// please use no spaces, the scanf-read-file function would fail
	strcpy( craft_matrix_persp       ->name, "MatPersp"        );


	for (auto op : perm.all) {
		op->is_permanent = 1;
		//// every node needs to send its value all the time, because I set the values directly, so nobody keeps the update-flow running
		//if (0)
		//	op->type = OpType::OP_TYPE_DELIVERER;
		//else
		//	op->type = OpType::OP_TYPE_RECEIVER; // dont execute anything atm
	}
	
	//craft_matrix_persp->type = OpType::OP_TYPE_REQUESTER;

	init_systems();
	//perm.loadFile("perm.txt");
}


#endif