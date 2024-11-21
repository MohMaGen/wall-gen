#ifndef COMMON_h_INCLUDED
#define COMMON_h_INCLUDED

#include <stdint.h>

#define defer(n) do { ret = n; goto defer; } while (0)


typedef struct { char *ptr; size_t len; } slice_i;
void* _shift(void *_self, size_t elem_size);
#define shift(slice) ((typeof((slice)->ptr)) _shift((slice), sizeof(*(slice)->ptr)))

typedef struct { char **ptr; size_t len; } args_t;
#define mk_args(ptr, len) ((args_t){ ptr, len })


typedef union { uint8_t rgb[4]; uint32_t hex; } color_t;
typedef struct { uint8_t r, g, b; } rgb_t;

rgb_t str2rgb(const char *str);

int read_size_t_arg(args_t *args, size_t *value);
int read_str_arg(args_t *args, char **str);


typedef struct {
    char *bg_color_str, *line_color_str, *output;
    size_t width, height;
} common_flags_t;

common_flags_t default_flags(void);


int read_common_flags(char *flag, args_t *args, common_flags_t *flags);

const char*get_help_msg();
void set_help_msg(char *msg);


#endif // COMMON_h_INCLUDED


#ifdef COMMON_IMPLEMENTATION

    void* _shift(void *_self, size_t elem_size)
    {
        slice_i *self = (slice_i*)_self;

        if (self->len == 0) return NULL;

        void *ret = (void*)self->ptr;

        self->ptr += elem_size;
        self->len--;

        return ret;
    }


    int read_size_t_arg(args_t *args, size_t *value)
    {
        if (args->len == 0) {
            fprintf(stderr, "\x1b[31;mexpect value of arg.\x1b[0;m\n"); 
            return 1;
        }

        char *flag = *shift(args);

        char *endptr;
        *value = strtoul(flag, &endptr, 10);
        if (endptr == flag) {
            fprintf(stderr, "\x1b[31;mexpect value of arg, but get: `%s`.\x1b[0;m\n", flag);
            return 1;
        }

        return 0;
    }

    int read_str_arg(args_t *args, char **str)
    {
        if (args->len == 0) {
            fprintf(stderr, "\x1b[31;mexpect value of arg.\x1b[0;m\n"); 
            return 1;
        }

        *str = *shift(args);

        return 0;
    }

    rgb_t str2rgb(const char *str)
    {
        if (str == NULL) return (rgb_t) { 0, 0, 0 };

        if (str[0] != '#') {
            if (strcmp(str, "red") == 0)      return (rgb_t) { 0xFF, 0x0,  0x0  };
            if (strcmp(str, "cyan") == 0)     return (rgb_t) { 0x0,  0xFF, 0xFF };
            if (strcmp(str, "blue") == 0)     return (rgb_t) { 0x0,  0x0,  0xFF };
            if (strcmp(str, "green") == 0)    return (rgb_t) { 0x0,  0xFF, 0x0  };
            if (strcmp(str, "black") == 0)    return (rgb_t) { 0x0,  0x0,  0x0  };
            if (strcmp(str, "white") == 0)    return (rgb_t) { 0xFF, 0xFF, 0xFF };
            if (strcmp(str, "yellow") == 0)   return (rgb_t) { 0xFF, 0xFF, 0x0  };
            if (strcmp(str, "magenta") == 0)  return (rgb_t) { 0xFF, 0x0,  0xFF };
            return (rgb_t) { 0, 0, 0 };

        } else {
            size_t value = 0;
            char *endptr = NULL;
            value = strtoul(str+1, &endptr, 16);
            if (endptr == str+1) return (rgb_t) { 0, 0, 0 };

            return (rgb_t) {
                value / 0x10000 % 0x1000000,
                value / 0x100 % 0x10000,
                value / 0x1 % 0x100,
            };
        }
    }

    int read_common_flags(char *flag, args_t *args, common_flags_t *flags)
    {
        size_t ret = 0;

        if (strcmp(flag, "--help") == 0) {
            printf("%s\n", get_help_msg());
            exit(0);
        }

        if (strcmp(flag, "-w") == 0 && read_size_t_arg(args, &flags->width) != 0) defer(1);

        if (strcmp(flag, "-h") == 0 && read_size_t_arg(args, &flags->height) != 0) defer(1);

        if (strcmp(flag, "-o") == 0 && read_str_arg(args, &flags->output) != 0) defer(1);

        if (strcmp(flag, "--bg-color") == 0 && read_str_arg(args, &flags->bg_color_str) != 0) defer(1);

        if (strcmp(flag, "--line-color") == 0 && read_str_arg(args, &flags->line_color_str) != 0) defer(1);

    defer:
        return ret;
    }

    common_flags_t default_flags(void)
    {
        return (common_flags_t) {
            .bg_color_str = "#272e33",
            .line_color_str = "#d3c6aa", 
            .output = "output.png",
            .width = 1920,
            .height = 1080,
        };
    }

    char *help_message;
    const char *get_help_msg() {
        return help_message;
    }

    void set_help_msg(char *msg)
    {
        help_message = msg;
    }


#endif
