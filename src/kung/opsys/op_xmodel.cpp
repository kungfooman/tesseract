//#include "craftstructs.h"
#include <kung/matrix.h>
#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
//#include "imgui_impl_glfw.h"
#include <stdio.h>
//#include "libs\glfw\include\GLFW/glfw3.h"
#include <Windows.h>
#include <list>

#include "op.h"
#include "op_xmodel.h"


#define SHORTNAME "XModel"
#define CLASSNAME OpXModel
#define METHOD(name) op_xmodel_##name
#define STRUCTNAME struct op_xmodel_s




#include <kung/include_duktape.h>

#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "primitives_3d.h"

#include "format_cod2_xmodel.h"

struct duk_heaphdr {
	duk_uint32_t h_flags;

#if defined(DUK_USE_REFERENCE_COUNTING)
#if defined(DUK_USE_REFCOUNT16)
	duk_uint16_t h_refcount16;
#else
	duk_size_t h_refcount;
#endif
#endif

#if defined(DUK_USE_HEAPPTR16)
	duk_uint16_t h_next16;
#else
	duk_heaphdr *h_next;
#endif

#if defined(DUK_USE_DOUBLE_LINKED_HEAP)
	/* refcounting requires direct heap frees, which in turn requires a dual linked heap */
#if defined(DUK_USE_HEAPPTR16)
	duk_uint16_t h_prev16;
#else
	duk_heaphdr *h_prev;
#endif
#endif

	/* When DUK_USE_HEAPPTR16 (and DUK_USE_REFCOUNT16) is in use, the
	 * struct won't align nicely to 4 bytes.  This 16-bit extra field
	 * is added to make the alignment clean; the field can be used by
	 * heap objects when 16-bit packing is used.  This field is now
	 * conditional to DUK_USE_HEAPPTR16 only, but it is intended to be
	 * used with DUK_USE_REFCOUNT16 and DUK_USE_DOUBLE_LINKED_HEAP;
	 * this only matter to low memory environments anyway.
	 */
#if defined(DUK_USE_HEAPPTR16)
	duk_uint16_t h_extra16;
#endif
};

struct duk_hobject {
	struct duk_heaphdr hdr;

	/*
	 *  'props' contains {key,value,flags} entries, optional array entries, and
	 *  an optional hash lookup table for non-array entries in a single 'sliced'
	 *  allocation.  There are several layout options, which differ slightly in
	 *  generated code size/speed and alignment/padding; duk_features.h selects
	 *  the layout used.
	 *
	 *  Layout 1 (DUK_USE_HOBJECT_LAYOUT_1):
	 *
	 *    e_size * sizeof(duk_hstring *)         bytes of   entry keys (e_next gc reachable)
	 *    e_size * sizeof(duk_propvalue)         bytes of   entry values (e_next gc reachable)
	 *    e_size * sizeof(duk_uint8_t)           bytes of   entry flags (e_next gc reachable)
	 *    a_size * sizeof(duk_tval)              bytes of   (opt) array values (plain only) (all gc reachable)
	 *    h_size * sizeof(duk_uint32_t)          bytes of   (opt) hash indexes to entries (e_size),
	 *                                                      0xffffffffUL = unused, 0xfffffffeUL = deleted
	 *
	 *  Layout 2 (DUK_USE_HOBJECT_LAYOUT_2):
	 *
	 *    e_size * sizeof(duk_propvalue)         bytes of   entry values (e_next gc reachable)
	 *    e_size * sizeof(duk_hstring *)         bytes of   entry keys (e_next gc reachable)
	 *    e_size * sizeof(duk_uint8_t) + pad     bytes of   entry flags (e_next gc reachable)
	 *    a_size * sizeof(duk_tval)              bytes of   (opt) array values (plain only) (all gc reachable)
	 *    h_size * sizeof(duk_uint32_t)          bytes of   (opt) hash indexes to entries (e_size),
	 *                                                      0xffffffffUL = unused, 0xfffffffeUL = deleted
	 *
	 *  Layout 3 (DUK_USE_HOBJECT_LAYOUT_3):
	 *
	 *    e_size * sizeof(duk_propvalue)         bytes of   entry values (e_next gc reachable)
	 *    a_size * sizeof(duk_tval)              bytes of   (opt) array values (plain only) (all gc reachable)
	 *    e_size * sizeof(duk_hstring *)         bytes of   entry keys (e_next gc reachable)
	 *    h_size * sizeof(duk_uint32_t)          bytes of   (opt) hash indexes to entries (e_size),
	 *                                                      0xffffffffUL = unused, 0xfffffffeUL = deleted
	 *    e_size * sizeof(duk_uint8_t)           bytes of   entry flags (e_next gc reachable)
	 *
	 *  In layout 1, the 'e_next' count is rounded to 4 or 8 on platforms
	 *  requiring 4 or 8 byte alignment.  This ensures proper alignment
	 *  for the entries, at the cost of memory footprint.  However, it's
	 *  probably preferable to use another layout on such platforms instead.
	 *
	 *  In layout 2, the key and value parts are swapped to avoid padding
	 *  the key array on platforms requiring alignment by 8.  The flags part
	 *  is padded to get alignment for array entries.  The 'e_next' count does
	 *  not need to be rounded as in layout 1.
	 *
	 *  In layout 3, entry values and array values are always aligned properly,
	 *  and assuming pointers are at most 8 bytes, so are the entry keys.  Hash
	 *  indices will be properly aligned (assuming pointers are at least 4 bytes).
	 *  Finally, flags don't need additional alignment.  This layout provides
	 *  compact allocations without padding (even on platforms with alignment
	 *  requirements) at the cost of a bit slower lookups.
	 *
	 *  Objects with few keys don't have a hash index; keys are looked up linearly,
	 *  which is cache efficient because the keys are consecutive.  Larger objects
	 *  have a hash index part which contains integer indexes to the entries part.
	 *
	 *  A single allocation reduces memory allocation overhead but requires more
	 *  work when any part needs to be resized.  A sliced allocation for entries
	 *  makes linear key matching faster on most platforms (more locality) and
	 *  skimps on flags size (which would be followed by 3 bytes of padding in
	 *  most architectures if entries were placed in a struct).
	 *
	 *  'props' also contains internal properties distinguished with a non-BMP
	 *  prefix.  Often used properties should be placed early in 'props' whenever
	 *  possible to make accessing them as fast a possible.
	 */

#if defined(DUK_USE_HEAPPTR16)
	/* Located in duk_heaphdr h_extra16.  Subclasses of duk_hobject (like
	 * duk_hcompiledfunction) are not free to use h_extra16 for this reason.
	 */
#else
	duk_uint8_t *props;
#endif

	/* prototype: the only internal property lifted outside 'e' as it is so central */
#if defined(DUK_USE_HEAPPTR16)
	duk_uint16_t prototype16;
#else
	struct duk_hobject *prototype;
#endif

#if defined(DUK_USE_OBJSIZES16)
	duk_uint16_t e_size16;
	duk_uint16_t e_next16;
	duk_uint16_t a_size16;
#if defined(DUK_USE_HOBJECT_HASH_PART)
	duk_uint16_t h_size16;
#endif
#else
	duk_uint32_t e_size;  /* entry part size */
	duk_uint32_t e_next;  /* index for next new key ([0,e_next[ are gc reachable) */
	duk_uint32_t a_size;  /* array part size (entirely gc reachable) */
#if defined(DUK_USE_HOBJECT_HASH_PART)
	duk_uint32_t h_size;  /* hash part size or 0 if unused */
#endif
#endif
};

struct duk_hbufferobject {
	/* Shared object part. */
	struct duk_hobject obj;

	/* Underlying buffer (refcounted), may be NULL. */
	struct duk_hbuffer *buf;

	/* Slice and accessor information.
	 *
	 * Because the underlying buffer may be dynamic, these may be
	 * invalidated by the buffer being modified so that both offset
	 * and length should be validated before every access.  Behavior
	 * when the underlying buffer has changed doesn't need to be clean:
	 * virtual 'length' doesn't need to be affected, reads can return
	 * zero/NaN, and writes can be ignored.
	 *
	 * Note that a data pointer cannot be precomputed because 'buf' may
	 * be dynamic and its pointer unstable.
	 */

	duk_uint_t offset;       /* byte offset to buf */
	duk_uint_t length;       /* byte index limit for element access, exclusive */
	duk_uint8_t shift;       /* element size shift:
	                          *   0 = u8/i8
	                          *   1 = u16/i16
	                          *   2 = u32/i32/float
	                          *   3 = double
	                          */
	duk_uint8_t elem_type;   /* element type */
	duk_uint8_t is_view;
};

extern "C" struct duk_hbufferobject *duk_to_buffer_kung(duk_context *ctx, duk_idx_t index, duk_size_t *out_size, duk_uint_t mode);

//float javascript_viewmatrix[16] = {0};
/*
buf = new Int32Array(16);
for (var i=0; i<16; i++)
buf[i] = i*i;
set_viewmatrix(buf)


memory_read_int(328846048 + 4*5)	

set_viewmatrix([1,2,3,4,5,6,7,8,9,10,11])

*/
// http://wiki.duktape.org/HowtoBuffers.html

//int duk_func_set_viewmatrix       (duk_context *ctx) {
//	unsigned int size;
//	struct duk_hbufferobject *buf;
//	buf = duk_to_buffer_kung(ctx, 0, &size, DUK_BUF_MODE_DONTCARE);













void METHOD(init)(CLASSNAME *op) {

	op_init((Op *)op);
	strcpy( op->name, SHORTNAME );
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 250);

	op->number_of_inputs = 9;
	op_init_link((Op *)op, 0, "BufPos", OP_TYPE_GLBUFFER );

	op->number_of_inputs = 1;
	op_init_link((Op *)op, 0, "MatModel", OP_TYPE_MATRIX );
	//op_init_link((Op *)op, 1, "MatP", OP_TYPE_MATRIX );

	//op_init_link((Op *)op, 1, "BufCol", OP_TYPE_GLBUFFER );
	//op_init_link((Op *)op, 2, "AttPos", OP_TYPE_GLATTRIB );
	op_init_link((Op *)op, 3, "AttCol", OP_TYPE_GLATTRIB );
	op_init_link((Op *)op, 4, "Mat", OP_TYPE_MATRIX );
	op_init_link((Op *)op, 5, "Prog", OP_TYPE_PROGRAM );
	op_init_link((Op *)op, 6, "UniMat", OP_TYPE_GLUNIFORM );
	op_init_link((Op *)op, 7, "count", OP_TYPE_FLOAT ); // meh, normally int, but atm everything just works with floats... might change late or kick out int at all
	op_init_link((Op *)op, 8, "drawmode", OP_TYPE_GLDRAWMODE );
	op->number_of_outputs = 0;
	op_init_link_output((Op *)op, 0, "Buffer", OP_TYPE_GLBUFFER);

	// first i will call: new Float32Array(1024)
	// Then I will increase the refcounter for that object, so Duktape won't render the memory location useless
	// Then I put that the variable name into MeshEditor, so i can modify float* pointer
	op->mesh = new Mesh();

	op->resize_minimal(op);
	op->type             = OpType::OP_TYPE_REQUESTER;
	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
	op->draw_in_scene    = METHOD(draw_in_scene);

	
    glGenBuffers(1, &op->default_link_outputs[0].opengl_id);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	
	//ImGui::SetCursorPos(op->pos);
	//ImGui::Image((ImTextureID)op->textureid, op->size/* + ImVec2(-20,-20)*/);
	//
	//ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	//ImGui::Text("Checkboard texid=%d", op->textureid);

	op_post_render((Op *)op);
}


void METHOD(update)(CLASSNAME *op) {



	if ( ! op->forcereload)
		return;
	if (op->xmodel)
		return;

	//log("reload american radio..\n");

	//log("OPOPOPOP Update cube %d forcereload=%d\n", op, op->forcereload);
	//op->mesh->x = op->x;
	//op->mesh->y = op->y;
	//op->mesh->z = op->z;
	//op->mesh->n = op->n;
	//op->mesh->init(); // todo: need a way to just update data without making new buffer
	//make_cube_wireframe(op->data, op->x, op->y, op->z, op->n);

	auto mat = glm::make_mat4x4(op->default_link_inputs[0].matrix);

	//kung_cube(op->data, op->x, op->y, op->z, mat);
	//buffer = gen_buffer(sizeof(data), data);





	//glBindBuffer(GL_ARRAY_BUFFER, op->default_link_outputs[1].opengl_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof op->data, op->data, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);



	op->default_link_outputs[0].changed++;

}

	extern float global_view_matrix[16];
	extern float global_projection_matrix[16];
void METHOD(render_legacy_code)(CLASSNAME *op) {

	float *matrix_model = op->default_link_inputs[0].matrix;

	float matrix_modelview[16];
	mat_multiply(matrix_modelview, global_view_matrix, matrix_model);

	op->xmodel->renderLegacyOpenGL(matrix_modelview, global_projection_matrix);
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);


	if (ImGui::Button("do shit") ) {


		duk_push_global_object(ctx);
		int ret = duk_get_prop_string(ctx, -1, "test");
		log("ret: %d", ret);

		unsigned int size;
		struct duk_hbufferobject *buf;
		if (ret) {
			buf = duk_to_buffer_kung(ctx, -1, &size, DUK_BUF_MODE_DONTCARE);
			
			//memcpy(javascript_viewmatrix, (void *)( ((int)buf->buf) + 20 ), 16 * 4);
			float *arr = (float *)( ((int)buf->buf) + 20  );
			//duk_hbuffer *hbuf = buf->buf;
			log("buf = %d size=%d %.2f %.2f %.2f off=%d llen=%d\n", buf, size, arr[0], arr[1], arr[2], buf->offset, buf->length);
		} else {
			//log("value does not exist\n");
		}
		duk_pop(ctx); // pop the getpropstring
		
	}

	if (op->xmodel == NULL) {
		ImGui::Text("op->xmodel == NULL");
	} else if (op->xmodel->load_success == 0) {
		ImGui::Text("xmodel->load_success == 0");
	} else {
		int i=0;
		for (auto mesh : op->xmodel->meshes) {
			ImGui::Text("meshes[%d] name=%s", i, mesh.name.c_str());
			i++;
		}
		i=0;
		for (auto texture : op->xmodel->textures) {
			ImGui::Text("textures[%d]", i);
			ImGui::Text("name=%s", texture->filename);
			ImGui::Text("textureid=%d", texture->textureid);
			i++;
		}
	}
	

	//ImGui::Text("address: %x", op->checkImage);
	//ImGui::Text("textureid: %x", op->textureid);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	fprintf(f, "filename %s",
		op->filename
	);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	sscanf(line, "filename %s",
		op->filename
	);

	op->xmodel = Format::CallOfDuty2::XModel::getByFilename(op->filename);
}

//#include "main_shaders.h"

/*
	program = load_program(
		"shaders/line2_vertex.glsl", "shaders/line2_fragment.glsl");
	line2_attrib.program = program;
	line2_attrib.position = glGetAttribLocation(program, "position");
	line2_attrib.color = glGetAttribLocation(program, "attr_color");
	line2_attrib.matrix = glGetUniformLocation(program, "matrix");
*/
extern float player_matrix[16];

void METHOD(draw_in_scene)(CLASSNAME *op) {

/*
	op_init_link((Op *)op, 0, "BufPos", OP_TYPE_GLBUFFER );
	op_init_link((Op *)op, 1, "BufCol", OP_TYPE_GLBUFFER );
	op_init_link((Op *)op, 2, "AttPos", OP_TYPE_GLATTRIB );
	op_init_link((Op *)op, 3, "AttCol", OP_TYPE_GLATTRIB );
	op_init_link((Op *)op, 4, "Mat", OP_TYPE_MATRIX );
	op_init_link((Op *)op, 5, "Prog", OP_TYPE_PROGRAM );
	op_init_link((Op *)op, 6, "UniMat", OP_TYPE_GLUNIFORM );
	op_init_link((Op *)op, 7, "count", OP_TYPE_FLOAT ); // meh, normally int, but atm everything just works with floats... might change late or kick out int at all
	op_init_link((Op *)op, 8, "drawmode", OP_TYPE_GLDRAWMODE );
*/

				// is set by update();
				GLuint vertices = op->default_link_outputs[0].opengl_id;

				//Attrib *a = &line2_attrib;
				Attrib *a = NULL;

				

				glUseProgram(a->program);

				

				glUniformMatrix4fv(a->matrix, 1, GL_FALSE, player_matrix);


				glBindBuffer(GL_ARRAY_BUFFER, vertices);
				glVertexAttribPointer(a->position, /*components*/3, GL_FLOAT, GL_FALSE, 0, 0); // https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
				glEnableVertexAttribArray(a->position);
	
				glBindBuffer(GL_ARRAY_BUFFER, vertices); // vertices as color...
				glVertexAttribPointer(a->color, /*components*/4, GL_FLOAT, GL_TRUE, 0, 0);
				glEnableVertexAttribArray(a->color);
	
				//glDrawArrays((int)op->default_link_inputs[8].val_f, 0, /*count*/(int)op->default_link_inputs[7].val_f);
				glDrawArrays((int)op->default_link_inputs[8].val_f, 0, /*count*/36);
				glDisableVertexAttribArray(a->color);
				glDisableVertexAttribArray(a->position);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

CLASSNAME *METHOD(new)() {
	//CLASSNAME *ret = (CLASSNAME *) malloc(sizeof(CLASSNAME));
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
