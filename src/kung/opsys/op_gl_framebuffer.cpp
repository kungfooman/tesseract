
#include "opsystem_opengl.h"
#include "op.h"
#include "op_gl_framebuffer.h"
//#include "util.h"

#include <kung/include_duktape.h>


#define SHORTNAME "Framebuffer"
#define CLASSNAME OpFramebuffer
#define METHOD(name) op_framebuffer_##name
#define STRUCTNAME struct op_framebuffer_s

//void METHOD(make_program)(CLASSNAME *op) {
//	if (op->program)
//		glDeleteProgram(op->program);
//	GLuint program = glCreateProgram();
//	if ( ! op->vertexshader) {
//		log("op->vertexshader not set for %s\n", op->name);
//		goto error;
//	}
//	if ( ! op->fragmentshader) {
//		log("shader2 not set for %s\n", op->name);
//		goto error;
//	}
//	glAttachShader(program, op->vertexshader);
//	glAttachShader(program, op->fragmentshader);
//	glLinkProgram(program);
//	GLint status;
//	glGetProgramiv(program, GL_LINK_STATUS, &status);
//	if (status == GL_FALSE) {
//		GLint length;
//		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
//		GLchar *info = (GLchar *)calloc(length, sizeof(GLchar));
//		glGetProgramInfoLog(program, length, NULL, info);
//		log("glLinkProgram failed: %s\n", info);
//		free(info);
//		
//		goto error;
//	} else {
//		op->haserror = 0;
//	}
//	glDetachShader(program, op->vertexshader);
//	glDetachShader(program, op->fragmentshader);
//	//glDeleteShader(shader1);
//	//glDeleteShader(shader2);
//	op->program = program;
//	log("update program: %d\n", op->program);
//	return;
//error:
//	if (program)
//		glDeleteProgram(program);
//	if (op->program)
//		glDeleteProgram(op->program);
//	op->program = 0;
//	op->haserror = 1;
//}

void METHOD(init)(CLASSNAME *op) {
	op_init((Op *)op);
	op->type = OP_TYPE_DELIVERER;
	op->size = ImVec2(300, 30);
	op->pos = ImVec2(250, 300);
	strcpy( op->name, SHORTNAME );
	
	//op->fragmentshader = 0;
	//op->vertexshader = 0;

	op->number_of_inputs = 2;
	op_init_link((Op *) op, 0, "Frag", OP_TYPE_FRAGMENTSHADER);
	op_init_link((Op *) op, 1, "Vert", OP_TYPE_VERTEXSHADER);
	op->number_of_outputs = 1;
	op_init_link_output((Op *)op, 0, "Prog", OP_TYPE_PROGRAM);
	
	op->resize_minimal(op);

	op->render_editor      = METHOD(render_editor);
	op->render             = METHOD(render);
	op->update             = METHOD(update);
	op->write_extra_data   = METHOD(write_extra_data);
	op->read_extra_data    = METHOD(read_extra_data);
	op->on_link_disconnect = METHOD(on_link_disconnect);
	op->on_link_update     = METHOD(on_link_update);

	
}

void METHOD(render)(CLASSNAME *op) {
	op_pre_render((Op *)op);
	ImGui::SetCursorPos(op->pos + ImVec2(5,5));
	//ImGui::Text("%d", op->program);


    //ImGuiIO& io = ImGui::GetIO();
    ////static ImVec2 imagePos = ImVec2(textureX/2, textureY/2); // If mouse cursor is outside the screen, use center of image as zoom point
    //ImVec2 cursorPos = ImGui::GetCursorScreenPos();          // "cursor" is where imgui will draw the image
    //ImVec2 mousePos = io.MousePos;

    //////////////////////////////////////////////////////
    // Draw the image/texture, filling the whole window //
    //////////////////////////////////////////////////////
    //ImGui::Image(reinterpret_cast<ImTextureID>(texture), size, ImVec2(0, 0), ImVec2(1, -1));
    ImGui::Image((ImTextureID)((size_t)op->texture), ImVec2(100,200), ImVec2(0, 0), ImVec2(1, -1));	

	op_post_render((Op *)op);
}


//void METHOD(dispatch)(CLASSNAME *op) {
//	switch(event) {
//		case OP_EVENT_CREATE:
//
//			break;
//		case OP_EVENT_DELETE:
//
//			break;
//		case OP_EVENT_RESIZE:
//
//			break;
//	}
//}


void METHOD(update)(CLASSNAME *op) {

	//if (op->fragmentshader && op->vertexshader && op->haserror==0 && op->program==0) {
	//	METHOD(make_program)(op);
	//	op->default_link_outputs[0].changed++;
	//}

	//auto *ol = op->default_link_outputs + 0;
	//for (auto il : *ol->inputlinks) {
	//	il->opengl_id = op->program;
	//	//if (op->type == OpType::OP_TYPE_SENDER)
	//	//	il->owner->update(il->owner);
	//}
}

void METHOD(render_editor)(CLASSNAME *op) {
	op_render_editor((Op *)op);
	//ImGui::Text("fragmentshader: %d", op->fragmentshader);
	//ImGui::Text("vertexshader: %d", op->vertexshader);
	//ImGui::Text("program: %d", op->program);



	







	if ( ImGui::Button("Make Framebuffer") ) {

//The Problem was that either iOS or Cocos2D (or both) can have a unique framebuffer. The handle of that unique frame buffer would be different than 0, and may be different each time.
//To solve this, I have to grab the current FBO's handle, do my custom Framebuffer stuff and then re-apply the FBO's handle after I'm done.
//Creates a variable to reference the original Frame Buffer Object
GLint oldFBO;

//Assigns the currently used FBO's handle (which is a 'GLint') to the variable 'oldFBO'
glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);

		////////////////////////////////////////////////////////////////////////
		// Create and bind framebuffer, attach a depth buffer to it           //
		// Create the texture to render to, and attach it to the framebuffer  //
		////////////////////////////////////////////////////////////////////////
		glGenFramebuffers(1, &op->framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, op->framebuffer);

		glGenRenderbuffers(1, &op->depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, op->depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, op->resx, op->resy);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, op->depthbuffer);

		glGenTextures(1, &op->texture);
		glBindTexture(GL_TEXTURE_2D, op->texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, op->resx, op->resy, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, op->texture, 0);
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			log("Error in setting up the framebuffer\n");
		} else {
			log("framebuffer made successfully\n");
		}




//here is when you would create or manipulate custom framebuffers.//
//After that, You set the original FBO as the current Framebuffer
glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
	}



	if (ImGui::Button("Render Framebuffer")) {
	

//The Problem was that either iOS or Cocos2D (or both) can have a unique framebuffer. The handle of that unique frame buffer would be different than 0, and may be different each time.
//To solve this, I have to grab the current FBO's handle, do my custom Framebuffer stuff and then re-apply the FBO's handle after I'm done.
//Creates a variable to reference the original Frame Buffer Object
GLint oldFBO;

//Assigns the currently used FBO's handle (which is a 'GLint') to the variable 'oldFBO'
glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);

    //if (textureX != size.x || textureY != size.y) {
        //textureX = size.x;
        //textureY = size.y;
        //cam_width = cam_height*textureX/float(textureY);
	
		GLsizei textureX = 100;
		GLsizei textureY = 200;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, op->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureX, textureY, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, op->framebuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureX, textureY);





		    glBindFramebuffer(GL_FRAMEBUFFER, op->framebuffer);
    glViewport(0,0,textureX,textureY);

	glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //glUseProgram(cube_program);


	
//here is when you would create or manipulate custom framebuffers.//
//After that, You set the original FBO as the current Framebuffer
glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);

    }

    //draw_cube();


	
}

void METHOD(write_extra_data)(CLASSNAME *op, FILE *f) {
	//fprintf(f, "filename %s",
	//	op->filename
	//);
}

void METHOD(read_extra_data)(CLASSNAME *op, char *line) {
	//sscanf(line, "filename %[^\n]",
	//	&op->filename
	//);
}

void METHOD(on_link_disconnect)(CLASSNAME *op, int id) {
	//log("disconnect id: %d\n", id);
	//switch (op->default_link_inputs[id].type) {
	//	case OP_TYPE_FRAGMENTSHADER:
	//		op->fragmentshader = 0;
	//		break;
	//	case OP_TYPE_VERTEXSHADER:
	//		op->vertexshader = 0;
	//		break;
	//}
}

void METHOD(on_link_update)(CLASSNAME *op, int id) {
	//log("link changed id: %d\n", id);
	//METHOD(make_program)(op);
	//switch (op->default_link_inputs[id].type) {
	//	case OP_TYPE_FRAGMENTSHADER:
	//		//op->fragmentshader = 0;
	//		break;
	//	case OP_TYPE_VERTEXSHADER:
	//		//op->vertexshader = 0;
	//		break;
	//}
}


CLASSNAME *METHOD(new)() {
	CLASSNAME *ret = new CLASSNAME;
	METHOD(init)(ret);
	return ret;
}
