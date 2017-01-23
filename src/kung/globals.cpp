#include <kung/include_glfw.h>
#include <kung/include_glfw.h>
#include <vector>

struct GLFWwindow *window = NULL;

int force_movement;
int autocomplete;
int repl_execute;
float player_matrix[16];
float global_view_matrix[16];
float global_projection_matrix[16];

// make op_renderer.cpp happy
std::vector<float> points;