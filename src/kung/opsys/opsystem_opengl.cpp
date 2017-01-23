#include <kung/include_gl.h>

#include "opsystem_opengl.h"
#include "opsystem_imgui_console.h"
#include "stdio.h"
#include "stdlib.h"

int openglstarted;

//char *load_file(const char *path);

GLuint opengl_make_shader(GLenum type, const char *source, int *out_error) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		GLchar *info = (GLchar *)calloc(length, sizeof(GLchar));
		glGetShaderInfoLog(shader, length, NULL, info);
		log("glCompileShader failed:\n%s\n", info);
		free(info);
		*out_error = 1;
		glDeleteShader(shader);
		return 0;
	} else {
		*out_error = 0;
	}
	return shader;
}

GLuint opengl_load_shader(GLenum type, const char *path, int *out_error) {
	__asm int 3; // meh
	// todo implement
	//char *data = load_file(path);
	//GLuint result = opengl_make_shader(type, data, out_error);
	//free(data);
	//return result;
	return 0;
}

GLuint opengl_make_program(GLuint shader1, GLuint shader2, int *out_error) {
	GLuint program = glCreateProgram();
	glAttachShader(program, shader1);
	glAttachShader(program, shader2);
	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar *info = (GLchar *)calloc(length, sizeof(GLchar));
		glGetProgramInfoLog(program, length, NULL, info);
		log("glLinkProgram failed: %s\n", info);
		free(info);
		*out_error = 1;
		glDeleteProgram(program);
		return 0;
	} else {
		*out_error = 0;
	}
	glDetachShader(program, shader1);
	glDetachShader(program, shader2);
	//glDeleteShader(shader1);
	//glDeleteShader(shader2);
	return program;
}