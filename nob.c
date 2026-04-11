#include <string.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

typedef struct
{
    const char** items;
    size_t count;
    size_t capacity;
} Array;

void print_usage(const char* program) { nob_log(NOB_INFO, "Usage: %s [run]", program); }

int main(int argc, char** argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    const char* program = nob_shift_args(&argc, &argv);

    bool run = false;

    int arg_count = argc;
    for (int a = 0; a < arg_count; a++)
    {
        const char* subcmd = nob_shift_args(&argc, &argv);
        if (strcmp(subcmd, "run") == 0) run = true;
        else
        {
            nob_log(NOB_ERROR, "Unknown command '%s'", subcmd);
            print_usage(program);
            return 1;
        }
    }

    Array srcs = {0};
    Array include_dirs = {0};
    Array lib_dirs = {0};
    Array libs = {0};

    nob_da_append(&srcs, "src/main.c");
    nob_da_append(&srcs, "src/player.c");

    nob_da_append(&include_dirs, ".");

    nob_da_append(&libs, ":libSDL3.a");

#ifdef _WIN32
    const char* app_output = "./bin/mage.exe";
    nob_da_append(&include_dirs, "D:/Haythem/Libraries/SDL3/include");
    nob_da_append(&lib_dirs, "D:/Haythem/Libraries/SDL3/lib");
    nob_da_append(&libs, "gdi32");
    nob_da_append(&libs, "winmm");
    nob_da_append(&libs, "imm32");
    nob_da_append(&libs, "ole32");
    nob_da_append(&libs, "oleaut32");
    nob_da_append(&libs, "version");
    nob_da_append(&libs, "uuid");
    nob_da_append(&libs, "setupapi");
    nob_da_append(&libs, "hid");
#else
    const char* app_output = "./bin/mage";
    nob_da_append(&include_dirs, "$HOME/opt/dev/SDL3/include");
    nob_da_append(&lib_dirs, "$HOME/opt/dev/SDL3/lib");
#endif
    if (!nob_mkdir_if_not_exists("bin")) return 1;

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "gcc", "-o", "./bin/mage");

    for (int i = 0; i < srcs.count; i++) nob_cmd_append(&cmd, srcs.items[i]);
    for (int i = 0; i < include_dirs.count; i++) nob_cmd_append(&cmd, "-I", include_dirs.items[i]);
    for (int i = 0; i < lib_dirs.count; i++) nob_cmd_append(&cmd, "-L", lib_dirs.items[i]);
    for (int i = 0; i < libs.count; i++) nob_cmd_append(&cmd, "-l", libs.items[i]);

    if (!nob_cmd_run_sync(cmd)) return 1;

    if (run)
    {
        cmd.count = 0;
        nob_cmd_append(&cmd, "./bin/mage");
        nob_cmd_run_sync(cmd);
    }

    return 0;
}
