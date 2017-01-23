char *shortname = SHORTNAME;
char name[64];
ImVec2 pos;
ImVec2 size;
int selected = 0;
OpType type;
unsigned int color;
int imgui_was_dragged = 0;
LinkOutput default_link_outputs[16];
Link default_link_inputs[16];
int number_of_inputs = 16;
int number_of_outputs = 16;
int is_permanent = 0;
int showtitle = 1;
std::list<STRUCTNAME *> update_ops_this_order;
OpSystem *system;
int cache_ingoing_connections;
int cache_outgoing_connections;
int isprefab = 0;
int forcereload = 0;

void (*render_editor       )(STRUCTNAME *op);
void (*render              )(STRUCTNAME *op);
void (*update              )(STRUCTNAME *op);
void (*changed             )(STRUCTNAME *op);
void (*dump_file           )(STRUCTNAME *op, PHYSFS_File *f);
void (*dump_links          )(STRUCTNAME *op, PHYSFS_File *f);
void (*resize_minimal      )(STRUCTNAME *op);
void (*destroy             )(STRUCTNAME *op);
void (*request             )(STRUCTNAME *op);
void (*write_extra_data    )(STRUCTNAME *op, PHYSFS_File *f);
void (*read_extra_data     )(STRUCTNAME *op, char *line);
void (*on_link_connect     )(STRUCTNAME *op, int id);
void (*on_link_disconnect  )(STRUCTNAME *op, int id);
void (*on_link_update      )(STRUCTNAME *op, int id);
void (*draw_in_scene       )(STRUCTNAME *op);