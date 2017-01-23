#include <kung/include_gl.h>
#include <SDL.h>
#include "imgui/examples/sdl_opengl3_example/imgui_impl_sdl_gl3.h"

#include "engine.h"

extern SDL_Window *screen;

void render_imgui() {

	static int first = 1;
	if (first) {
		gl3wInit();
		ImGui_ImplSdlGL3_Init(screen);
		first = 0;
	}

	static bool show_test_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImColor(255, 12, 12);

	ImGui_ImplSdlGL3_NewFrame(screen);
	
	


	
    ImGuiIO& io = ImGui::GetIO();
	float mousex, mousey;
	int w, h;
    SDL_GetWindowSize(screen, &w, &h);
	UI::getcursorpos(mousex, mousey);
	//mousex *= (float)w;
	//mousey *= (float)h;
	//io.MousePos = ImVec2(mousey, mousex);

	ImGui::Begin("meh");

	char buf[256];
	sprintf(buf, "mouse %f %f w %d h %d", mousex, mousey, w, h);

	ImGui::Button(buf);
	ImGui::End();
	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	{
	static float f = 0.0f;
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	if (ImGui::Button("Test Window")) show_test_window ^= 1;
	if (ImGui::Button("Another Window")) show_another_window ^= 1;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
	ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Another Window", &show_another_window);
	ImGui::Text("Hello");
	ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::ShowTestWindow(&show_test_window);
	}


	// Rendering
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();

}