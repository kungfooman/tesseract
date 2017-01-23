#include <kung/include_imgui.h>
#include "opsystem_imgui_console.h" // log()
#include <kung/include_duktape.h>

bool IsKeyPressedMap(ImGuiKey key, bool repeat = true);
bool IsKeyPressedMap(ImGuiKey key, bool repeat);
void SaveIniSettingsToDisk(const char* ini_filename); // was a static function in ImGui

char replbuffer[4096] = {0};
char replbuffer_update[4096] = {0};
int update_replbuffer = 0;
void ImStrncpy(char* dst, const char* src, int count);

int duk_func_repl_set_text(duk_context *ctx) {
	char *str = (char *)duk_to_string(ctx, 0);
	//log("update repl buffer to: \"%s\"\n", str);
	strcpy(replbuffer, str);
	return 0;
}

char repl_filename[128] = {"tmp.txt"};
extern int autocomplete;
extern int repl_execute;
int repl_callback(ImGuiTextEditCallbackData *data) {
	if (autocomplete) {
		char returnbuffer[256] = {0};
		js_call("get_auto_completion", "siiii", replbuffer, data->CursorPos, data->SelectionStart, data->SelectionEnd, returnbuffer);
		//log("got returnbuffer: %s\n", returnbuffer);
		data->InsertChars(data->CursorPos, returnbuffer);
		autocomplete = 0;
	}
	//log("callback called\n");
	return 0;
}

void imgui_op_repl2() {	
	ImGui::InputTextMultiline("", replbuffer, sizeof replbuffer, ImGui::GetWindowSize() + ImVec2(-15, -35), ImGuiInputTextFlags_CallbackAlways, repl_callback);

	if (ImGui::IsItemActive()) {
		//log("active\n");
		/*
		comment this shit in imgui.c to make this work:
		bool ctrl_enter_for_new_line = (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
        if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
        {
            //SetActiveID(0);
            //enter_pressed = true;
        }
		*/

		//if (ImGui::GetIO().KeyCtrl && IsKeyPressedMap(ImGuiKey_Enter, 0)) {
		if (repl_execute) {
			repl_execute = 0;
			ImGuiContext *g = ImGui::GetCurrentContext();
			int select_start = g->InputTextState.StbState.select_start;
			int select_end = g->InputTextState.StbState.select_end;
			js_call("shittyconsole", "sii", replbuffer, select_start, select_end);
		}

	}

	ImGui::InputText("Filename", repl_filename, sizeof repl_filename);
	ImGui::SameLine(0, 5);
	if (ImGui::Button("Save")) {
		js_call("file_put_contents", "ss", repl_filename, replbuffer);
	}
	ImGui::SameLine(0, 5);
	if (ImGui::Button("Load")) {
		js_eval((char *)std::string("repl_set_text(file_get_contents(\""+std::string(repl_filename)+"\"))").c_str());
	}
}