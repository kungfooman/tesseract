#include "opsystem.h"

#include <kung/include_duktape.h>
#include "opsystem_imgui_console.h"
#include "opsystem_repl.h"
#include "opsystem_utils.h"
#include "op_prefab.h"
#include <list>
#include "imgui/imgui_dock.h"
#include "opsystem_hierarchy.h"
#include "opsystem_explorer.h"
#include "op_file.h"

int imgui_line(ImVec2 a, ImVec2 b) {

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->AddLine(a, b, 0xffffffff, 2.0f);
	return 0;
}

int imgui_point(ImVec2 a, float radius = 1.0f, unsigned int color = 0xffffff) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->AddCircleFilled(a, radius, color);
	return 0;
}






int scissor_active = 0;
ImVec2 scissor_from;
ImVec2 scissor_to;

int rect_active = 0;
ImVec2 rect_from;
ImVec2 rect_to;



void update_op_system() {

	for (auto sys : systems)
	{
		sys->update();
	}
}

const char *get_filename_ext(const char *filename);
#include "op_xmodel.h"

void render_imgui_ops() {

	if (cursys == NULL) {
		ImGui::Text("cursys == NULL");
		return;
	}

	//ImGui::SetCursorPos
	//ImGui::Text("isprefab = %d", cursys->prefab);
	ImGui::Button("Parent: ");
	if (cursys->prefab) {
		ImGui::SameLine(0, 5);
		if (ImGui::Button(cursys->prefab->system->name))
			OpSystem::SetCurrent(cursys->prefab->system);
	}

	update_op_system();
	cursys->render();

	if ( ! ImGui::IsAnyItemHovered()) {
		ImVec2 winpos = ImGui::GetWindowPos();
		//log("winpos %.2f %.2f\n", winpos.x, winpos.y);

		if (ImGui::IsMouseClicked(0)) {
			scissor_from = ImGui::GetMousePos() - winpos;
			scissor_to = ImGui::GetMousePos() - winpos; // reset end pos, otherwise it might fuck up shit
			scissor_active = 1;
		}

		if (ImGui::IsMouseClicked(1)) {
				rect_from = ImGui::GetMousePos() - winpos;
				rect_to = ImGui::GetMousePos() - winpos; // reset end pos, otherwise it might fuck up shit
				rect_active = 1;
		}

		if (ImGui::IsMouseReleased(0) && ImGui::IsWindowFocused()) {
			if (scissor_from == ImGui::GetMousePos() - winpos) {
				cursys->unselectAll();
				//ops_tab = 0;
			}
		}

		if (ImGui::IsMouseDragging() && scissor_active) {
			scissor_to = ImGui::GetMousePos() - winpos;
			if (scissor_active)
				DrawLine(scissor_from.x, scissor_from.y, scissor_to.x, scissor_to.y);
		}

		if (ImGui::IsMouseDragging(1) && rect_active) {
			rect_to = ImGui::GetMousePos() - winpos;
			DrawRect(rect_from.x, rect_from.y, rect_to.x, rect_to.y);

			for (auto op : cursys->all) {
				//op->color = 0xffffffff;
				op->selected = 0;
			}
			for (auto op : cursys->all) {
				ImRect rect(op->pos, op->pos + op->size);
				rect.Min = op->pos;
				rect.Max = op->pos + op->size;
					
				float box_left   = rect_from.x;
				float box_right  = rect_to.x;
				float box_top    = rect_from.y;
				float box_bottom = rect_to.y;
				float left   = op->pos.x;
				float right  = op->pos.x + op->size.x;
				float top    = op->pos.y;
				float bottom = op->pos.y + op->size.y;


				int a = between(box_left,  left, box_right );
				int b = between(box_left, right, box_right );
				int c = between(box_top,    top, box_bottom);
				int d = between(box_top, bottom, box_bottom);

				// did the op cage the selectbox instead? aka being in middle of it
				int e = between(left, box_left  , right);
				int f = between(left, box_right , right);
				int g = between( top, box_top   , bottom);
				int h = between( top, box_bottom, bottom);

				int select = 0;

				// cornered a vertical and a horizontal line?
				if (a+b && c+d)
				{
					//op->color = rand32();
					op->selected = 1;
				}

				
				if (e && f && (c || d)) {
					op->selected = 1;
				}

				if (g && h && (a || b)) {
					op->selected = 1;
				}
			}
		}



	}


	if (ImGui::IsMouseReleased(0)) {
		cursys->updateExecIDs();
		cursys->updatePrefabInputIDs();
		cursys->updatePrefabOutputIDs();
	}

	if (ImGui::IsMouseReleased(0) && scissor_active) {
		int hits = 0;
		for (auto ll : linklines) {
			ImVec2 intersection;
			int ret = get_line_intersection(
			
				ll->from->pos.x,
				ll->from->pos.y + 10.0f,
				ll->to->pos.x,
				ll->to->pos.y + 10.0f,
				scissor_from.x,
				scissor_from.y,
				scissor_to.x,
				scissor_to.y,
				&intersection.x,
				&intersection.y
			);
			if (ret) {
				hits++;
				ll->from->inputlinks->remove(ll->to);
				
				ll->to->owner->on_link_disconnect(ll->to->owner, ll->to->local_id);
				// currently this only cleans the shader opengl_id, so we can invalidate the program
				//ll->to->clean();
			}
		}
		scissor_active = 0;
		if (hits) {
			log("Deleted %d links, regenerate call graphs\n", hits);
			cursys->regenerateCallGraphs();
		}
	}


	// draw all lines
	for (auto ll : linklines) {
		if (ll->to) {


			if (ll->from->owner->system != ll->to->owner->system) {
				// this case is true when output is connected to op in a prefab subsys
				ImVec2 projected_pos = ll->from->owner->system->prefab->default_link_outputs[0].pos;
				DrawLine(projected_pos.x, projected_pos.y + 10.0f, ll->to->pos.x, ll->to->pos.y + 10.0f);
			} else {
				DrawLine(ll->from->pos.x, ll->from->pos.y + 10.0f, ll->to->pos.x, ll->to->pos.y + 10.0f);
			}

		}

	}
	// free all
	for (auto ll : linklines)
		free(ll);
	// and reset list
	linklines.clear();

	if (link_from && link_to && ImGui::IsMouseReleased(0)) {
		log("connect link_from->owner=%s (isprefab=%d) link_to->owner=%s (isprefab=%d)\n",
			link_from->owner->name,
			link_from->owner->isprefab,
			link_to->owner->name,
			link_to->owner->isprefab
		);
		//if (link_from->owner->isprefab) {
		//	OpPrefab *prefab = (OpPrefab *) link_from->owner;
		//	log("prefab[%d] maps to %s[%d]\n",
		//		link_from->local_id, 
		//		prefab->internal_outputs[0]->owner->name, 0
		//	);
		//
		//	// when connecting a prefab->linkoutput to op->link, we actually do two links
		//	// 1) The normal prefab link, but this one is just for the drawline and load/save
		//	// 2) the internal one: from the subsys to the upper system, which actually does the work
		//
		//	// make the internal link:
		//	prefab->internal_outputs[0]->inputlinks->push_back( link_to );
		//}

		link_from->inputlinks->push_back(link_to);
		link_from = NULL;
		link_to->changed += 1; // make op_did_inputs_change work on linking
		//link_to->owner->forcereload += 1;
		cursys->regenerateCallGraphs();
	}
	// link_to is set based on if(ishovered()), so we gotta reset it all time so hover resets it all time
	link_to = NULL;




	// DRAG DROP support for Explorer
	extern std::string *dragdrop_file;
	if (ImGui::IsMouseHoveringWindow() && ImGui::IsMouseReleased(0) && dragdrop_file) {
		
		log("OPS pphysfs has dragdrop_file: %s\n", dragdrop_file->c_str());

		char *filename = (char *) dragdrop_file->c_str();
		const char *extension = get_filename_ext(filename);
		if (strstr("xmodel", extension) != 0) {
			log("read xmodel: %s\n", filename);

			char buf[256] = {0};
			sprintf(buf, "filename %s", filename);
			OpXModel *tmp = op_xmodel_new();
			tmp->read_extra_data(tmp, buf);
			cursys->add((Op *)tmp);
			cursys->regenerateCallGraphs();
			//auto xmodel = Format::CallOfDuty2::XModel::getByFilename(op->filename);
			//xmodel->load_xmodel((char *)op->filename);
		} else {

			OpFile *file = op_file_new();
			cursys->add((Op *)file);
			cursys->regenerateCallGraphs();
			file->pos = ImGui::GetMousePos() - ImGui::GetWindowPos();
		
			op_file_read_extra_data(file, (char *)(std::string("filename ") + *dragdrop_file).c_str());
			// op_file_read_extra_data possible deletes OpFile *file, so do not touch it anymore here
		}
		dragdrop_file = NULL;
		delete dragdrop_file;
	}
}

char opseditor_filename[64] = "perm.txt";


std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
    std::vector<std::string> names;
    std::string search_path = std::string(folder + "/*.*");
    WIN32_FIND_DATA fd; 
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd); 
    if(hFind != INVALID_HANDLE_VALUE) { 
        do { 
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(folder + "/" + fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 
    } 
    return names;
}


std::vector<std::string> files;
extern int force_movement;

extern char *currentwindow;

char node_opsys_filename[256] = {"opsystem/default.opsys"};

void render_imgui_ops_editor() {

	if (cursys == NULL) {
		ImGui::Text("cursys == NULL");
		return;
	}

	//static bool show_editor;

	//ImGui::Begin("OPS editor", &show_editor);
	//ImGui::SetDockActive

	int showops = strcmp(currentwindow, "Hierarchy") != 0;

	//if (!showops) {
	//	if (selected_node) {
	//		ImGui::Text("Current Node: %s", selected_node->name.c_str());
	//
	//		ImGui::InputText("opsys", node_opsys_filename, sizeof node_opsys_filename);
	//		if (ImGui::Button("Load")) {
	//			//selected_node->opsystem = new OpSystem(node_opsys_filename, 1);
	//			//auto opsys = selected_node->opsystem;
	//			//opsys->loadFile();
	//		}
	//
	//	} else {
	//		ImGui::Text("No node selected");
	//
	//	}
	//	return;
	//}

	if (g_dock.m_current) {
		//ImGui::Text("Curret Dock: %s\n", g_dock.m_current->label); // this var only gives the Dock which is rendered atm
		ImGui::Text("Focused Window: %s", currentwindow);
		ImGui::Text("Renderables: %d", cursys->renderables.size());
		
		//ImGui::IsWindowFocused
	}

	if (ImGui::Button("Update files")) {
		files = get_all_files_names_within_folder("opsystem");
		
		
	}
	
	for (auto file : files) {
		if ( ImGui::Button(file.c_str()) ) {
			auto newsys = new OpSystem((char *)file.c_str(), 0);
			newsys->loadFile(newsys->name);
		}

		ImGui::SameLine(0,5);

		ImGui::PushID(file.c_str());
		if (ImGui::Button("As Prefab")) {
			OpPrefab *prefab = op_prefab_new();
			//strcpy(prefab->filename, file.c_str());
			op_prefab_read_extra_data(prefab, (char *)("filename " + file).c_str());
			//prefab->pos.x = 100;
			//prefab->pos.y = 100;
			//prefab->size.x = 100;
			//prefab->size.y = 100;
			//prefab->selected = 1;
			cursys->add((Op *)prefab);
		}
		ImGui::PopID();
	}

	ImGui::Checkbox("force_movement", (bool *)&force_movement);
	if (selected_node) {


		ImGui::InputText("node name", selected_node->name, sizeof selected_node->name);

		ImGui::InputText("filename", selected_node->opsystem.name, sizeof selected_node->opsystem.name);
		//selected_node->name.resize( strlen( selected_node->name.data() ) );
	} else {
		//ImGui::InputText("filename", opseditor_filename, sizeof opseditor_filename);
	}

	for (auto sys : systems) {
		int popstyle = 0;
		if (cursys == sys) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 1.0f, 1.0f));
			popstyle = 1;
		}

		ImGui::PushID(sys);
		if (ImGui::Button(sys->name))
			OpSystem::SetCurrent(sys);
		ImGui::PopID();

		if (popstyle)
			ImGui::PopStyleColor();
	}
	
	//if (ImGui::Button("Save to filename")) { cursys->saveFile(opseditor_filename); }
	//if (ImGui::Button("Load System from filename")) { cursys->loadFile(opseditor_filename); cursys->regenerateCallGraphs(); }
	ImGui::Text("cursys = %s", cursys->name);
	if (ImGui::Button("Save System")) { cursys->saveFile(cursys->name); }
	if (ImGui::Button("Load System")) { cursys->loadFile(cursys->name); cursys->regenerateCallGraphs(); }
	if (ImGui::Button("Clear System")) { cursys->clearScene(); }
	ImGui::SameLine(0, 5);
	if (ImGui::Button("Unselect all")) {
			cursys->unselectAll();
	}

	if (ImGui::Button("Regenerate Call Graphs")) {
		cursys->regenerateCallGraphs();
	}

	cursys->renderEditor();

	//ImGui::End();
}



int main_menu_GUI() {
    //////////////////////
    // Placeholder menu //
    //////////////////////
    int menu_height = 0;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::MenuItem("fish_hat.c");
                ImGui::MenuItem("fish_hat.inl");
                ImGui::MenuItem("fish_hat.h");
                if (ImGui::BeginMenu("More.."))
                {
                    ImGui::MenuItem("Hello");
                    ImGui::MenuItem("Sailor");
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Disabled", false)) // Disabled
            {
                IM_ASSERT(0);
            }
           // if (ImGui::MenuItem("Fullscreen", NULL, fullscreen)) {fullscreen = !fullscreen;}
            if (ImGui::MenuItem("Quit", "Alt+F4")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        menu_height = ImGui::GetWindowSize().y;

        ImGui::EndMainMenuBar();
    }
    
    return menu_height;
}



void imgui_op_list() {
	cursys->makeOpButtons();
}

char *currentwindow = "OPS";

#include "tab_physfs.h"


void render_op_system() {

	if (ImGui::GetIO().DisplaySize.y > 0) {
		////////////////////////////////////////////////////
		// Setup root docking window                      //
		// taking into account menu height and status bar //
		////////////////////////////////////////////////////
		float menu_height = 200;
		auto pos = ImVec2(0, main_menu_GUI());
		auto size = ImGui::GetIO().DisplaySize;
		size.y -= pos.y;

		int statusbar = 1;

		if (statusbar)
			ImGui::RootDock(pos, ImVec2(size.x, size.y - 25.0f));
		else
			ImGui::RootDock(pos, ImVec2(size.x, size.y));

		if (statusbar) {
			// Draw status bar (no docking)
			ImGui::SetNextWindowSize(ImVec2(size.x, 25.0f), ImGuiSetCond_Always);
			ImGui::SetNextWindowPos(ImVec2(0, size.y - 6.0f), ImGuiSetCond_Always);
			ImGui::Begin("statusbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize);
			ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
			ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
			ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
			ImGui::End();
		}
	}


	struct windows {
		std::string name;
		void (*renderimgui)();
		bool show;

	};

	struct windows wins[] = {
	
		{"OPList"        , imgui_op_list           , 1},
		{"Hierarchy"     , render_hierarchy        , 1},
		{"Console"       , imgui_op_console        , 1},
		{"REPL"          , imgui_op_repl2          , 1},
		{"OPS"           , render_imgui_ops        , 1},
		{"Editor"        , render_imgui_ops_editor , 1},
		{"Explorer"      , render_explorer         , 1},
		{"PHYSFS"        , tab_physfs              , 1},
		
		{"", NULL, 0}
	};
	

	struct windows *it = wins;
	for (it = wins; it->renderimgui; it++) {
		if (ImGui::BeginDock(it->name.c_str(), &it->show)) {   
			//ImGui::Print(); // print docking information
			it->renderimgui();
			if (ImGui::IsWindowFocused()) {
				currentwindow = (char *)it->name.c_str();
			}
		}
		ImGui::EndDock();
	}



	
	
	
}



#if 0
int main(int, char**)
{
	
	js_init_full();

    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
	glfwWindowHint(GLFW_MAXIMIZED, 1);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "tcp imgui operators", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Setup ImGui binding
    ImGui_ImplGlfw_Init(window, true);

    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    bool show_test_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);
	double frame_start = glfwGetTime();


	init_wsa();

	serverdata_t serverdata;
	strcpy(serverdata.ip, "127.0.0.1");
	strcpy(serverdata.port, "13000");
	strcpy(serverdata.ondata, "function OnData(buffer)\n{\tprintf(\"%\", buffer);\n}\n");
	serverdata.running = 0;
	serverdata.ListenSocket = INVALID_SOCKET;

	clientdata_t clientdata;
	strcpy(clientdata.ip, "127.0.0.1");
	strcpy(clientdata.port, "13000");
	strcpy(clientdata.ondata, "function OnData(buffer)\n{\tprintf(\"%\", buffer);\n}\n");
	clientdata.ConnectSocket = INVALID_SOCKET;


	generate_texture();





    // Main loop
    while (!glfwWindowShouldClose(window)) {
		double wait_time = 1.0 / (60.0);
		if(glfwGetTime() - frame_start < wait_time)
		{
		double dur = (wait_time - (glfwGetTime() - frame_start));
		Sleep((DWORD)(dur * 1000.0 + 0.5));
		}
		frame_start = glfwGetTime();

        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();

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


		static bool show_client;
		ImGui::Begin("Client", &show_client);
		ImGui::Button("Client");

		ImGui::InputText("IP:", clientdata.ip, 128);
		ImGui::InputText("Port:", clientdata.port, 128);
		ImGui::Text("Socket: %d", clientdata.ConnectSocket);

		ImGui::Text("Actions:");
		
		if (clientdata.ConnectSocket != INVALID_SOCKET) {
			tcp_client_recv(&clientdata);
		}
		

		if (ImGui::Button("Connect()")) {
			tcp_client_connect(&clientdata);
		}
		ImGui::Button("Disconnect()");
		if (ImGui::Button("Send(\"test\")")) {
			tcp_client_send(&clientdata);
			console_lines.push_back("test");
		}

		ImGui::Text("Reactions:");
			
		ImGui::InputTextMultiline("OnData:", clientdata.ondata, 128);


		ImGui::End();




		static bool show_server;
		ImGui::Begin("Server", &show_server);
		ImGui::Button("Server");
		
		
		ImGui::InputText("IP:", serverdata.ip, 128);
		ImGui::InputText("Port:", serverdata.port, 128);
		ImGui::Text("Socket: %d", serverdata.ListenSocket);


		ImGui::Text("Actions:");
		
		if (ImGui::Button("Listen()")) {
			tcp_listen(&serverdata);
			log("listen\n");
		}
		// try to accept a new client every frama
		if (serverdata.running) {
			tcp_accept(&serverdata);
			tcp_server_read_all(&serverdata);
		}

		if (ImGui::Button("Disconnect()")) {
			log("disconnect\n");
		}
		ImGui::Button("Disconnect()");
		if (ImGui::Button("Send(\"test\")")) {
			console_lines.push_back("test");


		}

		ImGui::Text("Reactions:");
			
		
		ImGui::InputTextMultiline("OnData:", serverdata.ondata, 128);

		ImGui::Text("Clients:");
		server_clientdata_t *shutdown_scd = NULL;
		for (auto scd : serverdata.clientsockets) {

			ImGui::PushID(scd);

			ImGui::Text("Client: %d", scd->socket);

			ImGui::SameLine(0.0f, 5.0f);
			if (ImGui::Button("Shutdown()"))
			{
				// don't touch this stuff when iterating over it
				shutdown_scd = scd;
			}
			ImGui::SameLine(0.0f, 5.0f);
			if (ImGui::Button("Send()"))
			{
				log("send from #%ld to #%ld\n", serverdata.ListenSocket, scd->socket);
				tcp_send(scd->socket, scd->to_send, sizeof scd->to_send);
			}
			ImGui::InputText(scd->label, scd->to_send, sizeof scd->to_send);

			ImGui::PopID();
		}

		if (shutdown_scd) {
			// shutdown the connection since no more data will be sent
			int ret = shutdown(shutdown_scd->socket, SD_SEND);
			if (ret == SOCKET_ERROR) {
				log("shutdown failed with error: %d\n", WSAGetLastError());
				//closesocket(clientdata->ConnectSocket);
				//WSACleanup();
				//return 1;
			}
			serverdata.clientsockets.remove(shutdown_scd);
			free(shutdown_scd);
		}


		ImGui::End();



		

		
		ImGui::Image((ImTextureID)checkerboardid, ImVec2(100,100));


        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();

		#if 0
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
			glLoadIdentity();									// Reset The View
			glTranslatef(0.0f,0.0f,-5.0f);

			//glRotatef(xrot,1.0f,0.0f,0.0f);
			//glRotatef(yrot,0.0f,1.0f,0.0f);
			//glRotatef(zrot,0.0f,0.0f,1.0f);

			//glBindTexture(GL_TEXTURE_2D, texture[0]);
			glBindTexture(GL_TEXTURE_2D, checkerboardid);

			glBegin(GL_QUADS);
				// Front Face
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
				// Back Face
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
				// Top Face
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
				// Bottom Face
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
				// Right face
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
				// Left Face
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
			glEnd();
		#endif

        glfwSwapBuffers(window);
    }
	
	ImGuiContext& g = *GImGui;
	SaveIniSettingsToDisk(g.IO.IniFilename);


    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();


    return 0;
}

#endif
