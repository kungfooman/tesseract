#ifndef OP_SYSTEM_H
#define OP_SYSTEM_H

#include "op_all.h"
#include "op_value.h"
#include "op_matrix.h"
#include "op_prefab.h"
#include "op_renderer.h"
#include "op_cube.h"
#include "op_astar.h"

#include <list>

extern std::list<OpSystem *> systems;

extern OpSystem *cursys;
//extern OpSystem GLHacker;
extern OpSystem perm;
//extern OpSystem sys_ortho;

class OpSystem {
public:
	int was_already_in_system = 0;
	int isPrefab = 0;
	OpPrefab *prefab = NULL;
	int run_on_start;
	OpSystem(char *filename_);
	OpSystem(char *name, int run_on_start);
	OpSystem(char *name, OpPrefab *prefab_); // overload for prefabs
	static void OpSystem::SetCurrent(OpSystem *sys);
	char name[64];
	std::vector<Op *> all;
	std::list<Op *> requesters;
	std::list<Op *> changed_ops; // Ops which called op_changed() will first be stored in here, so the calls are dispatched at end of System::Update
	void render();
	void update();
	void updateRequesterList();
	void generateBacklinks();
	void regenerateCallGraphs();
	void renderEditor();
	void add(Op *op);
	void makeOpButtons();
	void clearScene();
	void unselectAll();
	Op * op_from_string(char * line);
	void loadFile(char *filename);
	void fixNameCollisions();
	void saveFile(char *filename);
	void deleteOp(Op * op);
	void deleteSelectedOps();
	OpValue  *createValue (char *name, int is_permanent);
	OpMatrix *createMatrix(char *name, int is_permanent);

	// sort these by id from top to bottom
	std::list<Op *> expressions;
	void updateExecIDs();
	std::list<Op *> prefabinputs;
	void updatePrefabInputIDs();
	std::list<Op *> prefaboutputs;
	void updatePrefabOutputIDs();

	
	void updateRenderers();
	std::list<OpRenderer *> renderers;
	void updateRenderables();
	std::list<Op *> renderables; // they contain op->draw_in_scene, normal ops are NULL
	void updateRenderablesLegacy();
	std::list<Op *> renderablesLegacy;
	void updateAStars();
	std::list<OpAStar *> astars;
};

#endif