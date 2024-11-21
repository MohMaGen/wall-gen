#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

#define COMMON_IMPLEMENTATION
#include "./common.h"

const char help_msg[] = 
    "WALL-GEN               grid              WALL-GEN\n"
    "\n"
    "                     HELP MSG\n"
    "\n"
    "USAGE:\n"
    "grid [option]\n"
    "\n"
    "OPTIONS:\n"
    " -o           -- set output file. Default: `out.png`.\n"
    " -w           -- set width of output image.\n"
    " -h           -- set height of output image.\n"
    " --offset-x   -- set offset by x axis.\n"
    " --offset-y   -- set offset by y axis.\n"
    " --grid_size  -- set grid size.\n"
	" --bg-color   -- color of the background\n"
	" --line-color -- color of the lightest lines\n"
    ;

int main(int argc, char **argv) {
    args_t args = mk_args(argv, argc);
    char *prog_name = *shift(&args);

    printf("run as: %s\n", prog_name);

    size_t width, height, channels, offset_x, offset_y, grid_size;
    width     = 1920;
    height    = 1080;

    offset_x  = 60;
    offset_y  = 40;
    channels  = 3;
    grid_size = 100;

    unsigned char *image;
    image = NULL;

    char *output = "output.png";

    int ret = 0;

    char *bg_color_str = "#272e33";
    char *line_color_str = "#d3c6aa";

    while (args.len != 0) {
        char *flag = *shift(&args);
        if (strcmp(flag, "--help") == 0) {
            printf("%s\n", help_msg);
            defer(0);
        }

        if (strcmp(flag, "-w") == 0 && read_size_t_arg(&args, &width) != 0) defer(1);

        if (strcmp(flag, "-h") == 0 && read_size_t_arg(&args, &height) != 0) defer(1);

        if (strcmp(flag, "--offset-y") == 0 && read_size_t_arg(&args, &offset_y) != 0) defer(1);
        if (strcmp(flag, "--offset-x") == 0 && read_size_t_arg(&args, &offset_x) != 0) defer(1);

        if (strcmp(flag, "--grid-size") == 0 && read_size_t_arg(&args, &grid_size) != 0) defer(1);

        if (strcmp(flag, "-o") == 0 && read_str_arg(&args, &output) != 0) defer(1);

        if (strcmp(flag, "--bg-color") == 0 && read_str_arg(&args, &bg_color_str) != 0) defer(1);

        if (strcmp(flag, "--line-color") == 0 && read_str_arg(&args, &line_color_str) != 0) defer(1);
    }

    printf("    ---==< Image properties >==---\n"); 
    printf("      %10s: %10ld\n", "gridysize", grid_size);
    printf("      %10s: %10ld\n", "channels", channels);
    printf("      %10s: %10ld\n", "offset-y", offset_y);
    printf("      %10s: %10ld\n", "offset-x", offset_x);
    printf("      %10s: %10ld\n", "height", height);
    printf("      %10s: %10ld\n", "width", width);

    image = calloc(width * height * channels, sizeof(unsigned char));

    rgb_t bg_color = str2rgb(bg_color_str);
    rgb_t line_color = str2rgb(line_color_str);

    rgb_t *rgb_img = (rgb_t *)image;

	for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            size_t idx = (x + width * y);
            rgb_t color = bg_color;

                if ((x + grid_size - offset_x) % (grid_size / 4) == 0 ||
                    (y + grid_size - offset_y) % (grid_size / 4) == 0)
                {
                    color = (rgb_t) {
                        (3 * (int)bg_color.r + (int)line_color.r) / 4,
                        (3 * (int)bg_color.g + (int)line_color.g) / 4,
                        (3 * (int)bg_color.b + (int)line_color.b) / 4,
                    };
                }

                if ((x + grid_size - offset_x) % (grid_size / 2) == 0 ||
                    (y + grid_size - offset_y) % (grid_size / 2) == 0)
                {
                    color = (rgb_t) {
                        ((int)bg_color.r + (int)line_color.r) / 2,
                        ((int)bg_color.g + (int)line_color.g) / 2,
                        ((int)bg_color.b + (int)line_color.b) / 2,
                    };
                }

                if ((x + grid_size - offset_x) % grid_size == 0 ||
                    (y + grid_size - offset_y) % grid_size == 0)
                {
                    color = line_color;
                }
            rgb_img[idx] = color;
        }
    }

    stbi_write_png(output, width, height, channels, image, width * channels);


defer:
    if (image != NULL) free(image);

    return ret;
}
