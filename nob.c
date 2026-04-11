#include <string.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

void print_usage(const char* program)
{
    nob_log(NOB_INFO, "Usage: %s [run]", program);
    return;
}

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

    const char* srcs[] = {"src/main.c", "src/player.c"};
    const char* include_dirs[] = {"."};

    const char* app_name = "mage";

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "test", "-d", "bin");
    if (!nob_cmd_run_sync(cmd))
    {
        cmd.count = 0;
        nob_cmd_append(&cmd, "mkdir", "bin");
        if (!nob_cmd_run_sync(cmd)) return 1;
    }

    cmd.count = 0;
    nob_cmd_append(&cmd, "gcc", "-o", "./bin/mage");

    for (int i = 0; i < NOB_ARRAY_LEN(srcs); i++)
    {
        nob_cmd_append(&cmd, srcs[i]);
    }

    for (int i = 0; i < NOB_ARRAY_LEN(include_dirs); i++)
    {
        nob_cmd_append(&cmd, "-I", include_dirs[i]);
    }

    nob_cmd_append(&cmd, "-l", "SDL3");
    nob_cmd_append(&cmd, "-L", "$HOME/dev/opt/SDL3/lib");

    if (!nob_cmd_run_sync(cmd)) return 1;

    if (run)
    {
        cmd.count = 0;
        nob_cmd_append(&cmd, "./bin/mage");
        nob_cmd_run_sync(cmd);
    }

    return 0;
}
