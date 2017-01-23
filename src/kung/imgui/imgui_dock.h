#ifndef IMGUI_DOCK_H
#define IMGUI_DOCK_H

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace ImGui {
	enum Slot_ {
		Slot_Left,
		Slot_Right,
		Slot_Top,
		Slot_Bottom,
		Slot_Tab,

		Slot_Float,
		Slot_None
	};

	enum EndAction_ {
		EndAction_None,
		EndAction_Panel,
		EndAction_End,
		EndAction_EndChild
	};

	enum Status_ {
		Status_Docked,
		Status_Float,
		Status_Dragged
	};

	class Dock {
	public:
		ImU32 id;
		char* label;
		Dock* next_tab;
		Dock* prev_tab;
		Dock* parent;
		ImVec2 pos;
		ImVec2 size;
		bool active;
		Status_ status;
		bool opened;
		Dock* children[2];
		char location[16];
		int last_frame;
		int invalid_frames;
		bool first;

		Dock();
		~Dock() ;
		ImVec2 getMinSize() const;
		bool isHorizontal() const ;
		void setParent(Dock* dock);
		Dock& getSibling();
		Dock& getFirstTab();
		void setActive();
		bool isContainer() const ;
		void setChildrenPosSize(const ImVec2& _pos, const ImVec2& _size);
		void setPosSize(const ImVec2& _pos, const ImVec2& _size);
	};

	class DockContext {
	public:
		ImVector<Dock*> m_docks;
		ImVec2 m_drag_offset;
		Dock* m_current = nullptr;
		int m_last_frame = 0;
		EndAction_ m_end_action;

		~DockContext();
		Dock& getDock(const char* label, bool opened);
		void putInBackground();
		void splits();
		void checkNonexistent();
		void beginPanel();
		void endPanel();
		Dock* getDockAt(const ImVec2& /*pos*/) const; // Doesn't use input??
		ImRect getDockedRect(const ImRect& rect, Slot_ dock_slot);
		ImRect getSlotRect(ImRect parent_rect, Slot_ dock_slot);
		ImRect getSlotRectOnBorder(ImRect parent_rect, Slot_ dock_slot);
		Dock* getRootDock();
		bool dockSlots(Dock& dock, Dock* dest_dock, const ImRect& rect, bool on_border);
		void handleDrag(Dock& dock);
		void fillLocation(Dock& dock);
		void doUndock(Dock& dock);
		void drawTabbarListButton(Dock& dock);
		bool tabbar(Dock& dock, bool close_button);
		void setDockPosSize(Dock& dest, Dock& dock, Slot_ dock_slot, Dock& container);
		void doDock(Dock& dock, Dock* dest, Slot_ dock_slot);
		void rootDock(const ImVec2& pos, const ImVec2& size);
		void setDockActive();
		Slot_ getSlotFromLocationCode(char code);
		char getLocationCode(Dock* dock);
		void tryDockToStoredLocation(Dock& dock);
		bool begin(const char* label, bool* opened, ImGuiWindowFlags extra_flags);
		void end();
		int getDockIndex(Dock* dock);
		void save();
		Dock* getDockByIndex(int idx);
		void load();
	};

	IMGUI_API void ShutdownDock();
	IMGUI_API void RootDock(const ImVec2& pos, const ImVec2& size);
	IMGUI_API bool BeginDock(const char* label, bool* opened = nullptr, ImGuiWindowFlags extra_flags = 0);
	IMGUI_API void EndDock();
	IMGUI_API void SetDockActive();
	IMGUI_API void LoadDock();
	IMGUI_API void SaveDock();
	IMGUI_API void Print();

	

} // namespace ImGui

extern ImGui::DockContext g_dock;

#endif