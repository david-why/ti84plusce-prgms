#include <tice.h>

int getinput(char *buffer,
             unsigned int maxsize,
             int x,
             int y,
             sk_key_t okkey = sk_Enter,
             sk_key_t quitkey = sk_Mode,
             const char *var_n = NULL,
             const char *var_s = NULL,
             const char *var_A = NULL,
             const char *var_a = NULL);
