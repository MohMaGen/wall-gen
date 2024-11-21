#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define COMMON_IMPLEMENTATION
#include "common.h"


int main(int argc, char **argv)
{
    int ret = 0;

    args_t args = mk_args(argv, argc);
    (void)shift(&args);

    set_help_msg(
        "\n\n"
        "WALL-GEN               square               WALL-GEN\n"
        "\n"
        "                     HELP MSG\n"
        "\n"
        "USAGE:\n"
        "square [option]\n"
        "\n"
        "OPTIONS:\n"
        " -o           -- set output file. Default: `out.png`.\n"
        " -w           -- set width of output image.\n"
        " -h           -- set height of output image.\n"
        " --bg-color   -- color of the background.\n"
        " --line-color -- color of the lightest lines.\n"
        " --center_x   -- x coord of the squares center.\n"
        " --center_y   -- y coord of the squares center.\n"
        " --sw         -- square half width.\n"
        " --sh         -- square half height.\n"
    );

    common_flags_t common_flags = default_flags();
    unsigned char *image = NULL;

    size_t center_x  = common_flags.width / 2;
    size_t center_y  = common_flags.height / 2;
    size_t sw = common_flags.width / 8;
    size_t sh = common_flags.height / 8;

    while (args.len != 0) {
        char *flag = *shift(&args);
        if (read_common_flags(flag, &args, &common_flags) == 1) defer(1);

        if (strcmp(flag, "--center_x") == 0 && read_size_t_arg(&args, &center_x) != 0) defer(1);
        if (strcmp(flag, "--center_y") == 0 && read_size_t_arg(&args, &center_y) != 0) defer(1);
        if (strcmp(flag, "--sw") == 0 && read_size_t_arg(&args, &sw) != 0) defer(1);
        if (strcmp(flag, "--sh") == 0 && read_size_t_arg(&args, &sh) != 0) defer(1);
    }

    image = calloc(common_flags.width * common_flags.height * 3, sizeof(unsigned char));

    rgb_t *image_rgb = (rgb_t*)image;
    rgb_t bg_color = str2rgb(common_flags.bg_color_str);
    rgb_t line_color = str2rgb(common_flags.line_color_str);

    for (int y = 0; y < common_flags.height; y++) {
        for (int x = 0; x < common_flags.width; x++) {
            rgb_t color = bg_color;

            if (
                x >= (int)center_x - (int)sw && x <= (int)center_x + sw &&
                y >= (int)center_y - (int)sw && y <= (int)center_y + sw
            ) 
            {
                color = line_color;
            }


            image_rgb[x + y * common_flags.width] = color;
        }
    }

    stbi_write_png(common_flags.output, common_flags.width, common_flags.height, 3, image,
                   common_flags.width * 3);

defer:
    if (image != NULL) free(image);
    return ret;
}
