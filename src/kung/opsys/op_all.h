#ifndef OP_ALL_H
#define OP_ALL_H

#include "op.h"
#include "opsystem.h"

void InitPermanentOps();
extern std::vector<Op *> permanent_ops;

// these ops are directly called from code and always in Ops View
#include "op_matrix.h"
extern OpMatrix            *craft_matrix_persp;
extern OpMatrix            *craft_matrix_ortho;

typedef struct oplist_s {
	char *name;
	Op *(*create_op)();
} oplist_t;
extern oplist_t available_ops[];

#endif