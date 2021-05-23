#ifndef SHA_H
#define SHA_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

extern void get_hash(char* ptext);
extern void get_hash_from_random();

#endif // SHA_H