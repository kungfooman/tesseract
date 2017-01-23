//#include "craftstructs.h"

#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
//#include "imgui_impl_glfw.h"
#include <stdio.h>
//#include "libs\glfw\include\GLFW/glfw3.h"
#include <Windows.h>
#include <list>

#include "op.h"
#include "op_renderer.h"

#include "opsystem.h"

#define SHORTNAME "Renderer"
#define CLASSNAME OpRenderer
#define METHOD(name) op_renderer_##name
#define STRUCTNAME struct op_renderer_s

void METHOD(init)(CLASSNAME *op) {

	op_init((Op *)op);
	strcpy( op->name, SHORTNAME );
	op->size = ImVec2(256, 256);
	op->pos = ImVec2(250, 250);
	
	op->number_of_inputs = 9;
	op_init_link((Op *)op, 0, "BufPos", OP_TYPE_GLBUFFER );
	op_init_link((Op *)op, 1, "BufCol", OP_TYPE_GLBUFFER );
	op_init_link((Op *)op, 2, "AttPos", OP_TYPE_GLATTRIB );
	op_init_link((Op *)op, 3, "AttCol", OP_TYPE_GLATTRIB );
	op_init_link((Op *)op, 4, "Mat", OP_TYPE_MATRIX );
	op_init_link((Op *)op, 5, "Prog", OP_TYPE_PROGRAM );
	op_init_link((Op *)op, 6, "UniMat", OP_TYPE_GLUNIFORM );
	op_init_link((Op *)op, 7, "count", OP_TYPE_FLOAT ); // meh, normally int, but atm everything just works with floats... might change late or kick out int at all
	op_init_link((Op *)op, 8, "drawmode", OP_TYPE_GLDRAWMODE );
	op->number_of_outputs = 1;
	op_init_link_output((Op *)op, 0, "Mesh", OP_TYPE_MESH );

	op->type             = OpType::OP_TYPE_REQUESTER;
	op->render_editor    = METHOD(render_editor);
	op->render           = METHOD(render);
	op->update           = METHOD(update);
	op->write_extra_data = METHOD(write_extra_data);
	op->read_extra_data  = METHOD(read_extra_data);
	op->draw_in_scene    = METHOD(draw_in_scene);
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	
	ImGui::SetCursorPos(op->pos);
	ImGui::Text("foooo");
	//ImGui::Image((ImTextureID)op->textureid, op->size/* + ImVec2(-20,-20)*/);
	//
	//ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	//ImGui::Text("Checkboard texid=%d", op->textureid);


	

	op_post_render((Op *)op);
}


extern std::list<Mesh *> meshes;


void METHOD(update)(CLASSNAME *op) {
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
	//ImGui::Text("prog: %d", op->default_link_inputs[2].opengl_id);
	//ImGui::Text("textureid: %x", op->textureid);
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	//fprintf(f, "x %f y %f z %f n %f",
	//	op->x,
	//	op->y,
	//	op->z,
	//	op->n
	//);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	//sscanf(line, "x %f y %f z %f n %f",
	//	&op->x,
	//	&op->y,
	//	&op->z,
	//	&op->n
	//);
}

extern std::vector<float> points;

void METHOD(draw_in_scene)(CLASSNAME *op) {
	
/*
	op_init_link((Op *)op, 0, "BufPos", OP_TYPE_GLBUFFER );
	op_init_link((Op *)op, 1, "BufCol", OP_TYPE_GLBUFFER );
	op_init_link((Op *)op, 2, "AttPos", OP_TYPE_GLATTRIB );
	op_init_link((Op *)op, 3, "AttCol", OP_TYPE_GLATTRIB );
	op_init_link((Op *)op, 4, "Mat", OP_TYPE_MATRIX );
	op_init_link((Op *)op, 5, "Prog", OP_TYPE_PROGRAM );
	op_init_link((Op *)op, 6, "UniMat", OP_TYPE_GLUNIFORM );
	op_init_link((Op *)op, 7, "count", OP_TYPE_INT );
	op_init_link((Op *)op, 8, "drawmode", OP_TYPE_GLDRAWMODE );
*/






	glUseProgram(op->default_link_inputs[5].opengl_id);

	
	glUniformMatrix4fv(op->default_link_inputs[6].opengl_id, 1, GL_FALSE, op->default_link_inputs[4].matrix);
	//glUniformMatrix4fv(op->default_link_inputs[6].opengl_id, 1, GL_FALSE, matrix);



	glBindBuffer(GL_ARRAY_BUFFER, op->default_link_inputs[0].opengl_id);
	glVertexAttribPointer(op->default_link_inputs[2].opengl_id, /*components*/4, GL_FLOAT, GL_FALSE, 0, 0); // https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
	glEnableVertexAttribArray(op->default_link_inputs[2].opengl_id);
	
	glBindBuffer(GL_ARRAY_BUFFER, op->default_link_inputs[1].opengl_id);
	glVertexAttribPointer(op->default_link_inputs[3].opengl_id, /*components*/4, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(op->default_link_inputs[3].opengl_id);
	
	glDrawArrays((int)op->default_link_inputs[8].val_f, 0, /*count*/(int)op->default_link_inputs[7].val_f);
	//log("count: %d\n", (int)op->default_link_inputs[7].val_f);
	glDisableVertexAttribArray(op->default_link_inputs[2].opengl_id);
	glDisableVertexAttribArray(op->default_link_inputs[3].opengl_id);




	for (auto sys : systems) {
		for (auto astar : sys->astars) {
			for (auto node : astar->nodes) {
			
			
			
				GLuint vertices = node->buffer;

				glBindBuffer(GL_ARRAY_BUFFER, vertices);
				glVertexAttribPointer(op->default_link_inputs[2].opengl_id, /*components*/3, GL_FLOAT, GL_FALSE, 0, 0); // https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
				glEnableVertexAttribArray(op->default_link_inputs[2].opengl_id);
	
				glBindBuffer(GL_ARRAY_BUFFER, op->default_link_inputs[1].opengl_id);
				glVertexAttribPointer(op->default_link_inputs[3].opengl_id, /*components*/4, GL_FLOAT, GL_TRUE, 0, 0);
				glEnableVertexAttribArray(op->default_link_inputs[3].opengl_id);
	
				//glDrawArrays((int)op->default_link_inputs[8].val_f, 0, /*count*/(int)op->default_link_inputs[7].val_f);
				glDrawArrays((int)op->default_link_inputs[8].val_f, 0, /*count*/36);
				glDisableVertexAttribArray(op->default_link_inputs[2].opengl_id);
				glDisableVertexAttribArray(op->default_link_inputs[3].opengl_id);
			}
		}
	}

	//glDisable(GL_DEPTH_TEST);

	if (points.size()) {
		GLuint pointbuffer;
		glGenBuffers(1,  &pointbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, pointbuffer);
		glBufferData(GL_ARRAY_BUFFER, points.size() * 12, (GLvoid *)points.data(), GL_STATIC_DRAW); // 3 floats = 12 bytes per point

		glPointSize(10);

		glVertexAttribPointer(op->default_link_inputs[2].opengl_id, /*components*/3, GL_FLOAT, GL_FALSE, 0, 0); // https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
		glEnableVertexAttribArray(op->default_link_inputs[2].opengl_id);
	
		glBindBuffer(GL_ARRAY_BUFFER, op->default_link_inputs[1].opengl_id);
		glVertexAttribPointer(op->default_link_inputs[3].opengl_id, /*components*/4, GL_FLOAT, GL_TRUE, 0, 0);
		glEnableVertexAttribArray(op->default_link_inputs[3].opengl_id);
	
		glDrawArrays(GL_POINTS, 0, points.size());
		glDisableVertexAttribArray(op->default_link_inputs[2].opengl_id);
		glDisableVertexAttribArray(op->default_link_inputs[3].opengl_id);


		glDeleteBuffers(1, &pointbuffer);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

CLASSNAME *METHOD(new)() {
	//CLASSNAME *ret = (CLASSNAME *) malloc(sizeof(CLASSNAME));
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
