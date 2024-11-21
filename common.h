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
            if (strcmp(str, "white") == 0)    return (rgb_t) { 0x0,  0x0,  0x0  };
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

#endif
