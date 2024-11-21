#define NOB_IMPLEMENTATION
#include "nob.h"

#define COMMON_IMPLEMENTATION
#include "common.h"

int main(int argc, char **argv) {
    if (!nob_mkdir_if_not_exists("build")) return 1;

    NOB_GO_REBUILD_URSELF(argc, argv);
    int ret = 0;

    Nob_Cmd cmd = {0};

    args_t args = mk_args(argv, argc);

    (char **)shift(&args);

    char **command = shift(&args);
    if (command == NULL) {
        printf("Expect command\n");
        defer(1);
    }

    nob_cmd_append(&cmd, "clang", "-o", 
                    nob_temp_sprintf("build/%s", *command), 
                    nob_temp_sprintf("%s.c", *command), "-lm", "-std=c2x");

    if (!nob_cmd_run_sync_and_reset(&cmd)) defer(1);

    nob_cmd_append(&cmd, nob_temp_sprintf("build/%s", *command), "-o", "res.png");
    while (args.len != 0) {
        nob_cmd_append(&cmd, *shift(&args));
    }
    if (!nob_cmd_run_sync_and_reset(&cmd)) defer(1);

    nob_cmd_append(&cmd, "feh", "res.png");
    if (!nob_cmd_run_sync_and_reset(&cmd)) defer(1);

defer:
    return ret;
}
