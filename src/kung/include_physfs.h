// https://icculus.org/physfs/physfstut.txt

#include "G:/libs/physfs-2.0.3/physfs.h"


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "G:\\Craft-master\\src\\glfw\\lib-vc2010-32\\glfw3.lib")
//#pragma comment (lib, "duktapestuff.lib")


#pragma comment (lib, "G:/libs/physfs-2.0.3/build/Debug/physfs.lib")
#pragma comment (lib, "G:/libs/Simple OpenGL Image Library/projects/VC9/Debug/SOIL.lib")

#pragma comment (lib, "G:/libs/DevIL-master/DevIL/build/lib/x86/Debug/DevIL.lib")
#pragma comment (lib, "G:/libs/DevIL-master/DevIL/build/lib/x86/Debug/ILU.lib")
#pragma comment (lib, "G:/libs/DevIL-master/DevIL/build/lib/x86/Debug/ILUT.lib")

//#include "G:/libs/physfs-2.0.3/physfs_internal.h"

// https://github.com/Warzone2100/warzone2100/blob/master/lib/framework/physfs_ext.h
char *PHYSFS_fgets(char *s, int size, PHYSFS_file *stream);
int PHYSFS_fprintf(PHYSFS_file *f, char *format, ...);