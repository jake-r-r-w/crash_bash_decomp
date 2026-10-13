#include "common.h"
#include "bash/8003e0ec.h"

extern u8 D_8006DBCC[];

/* Identical to libmcrd's McrdGetGlobalStructure. */
void *func_8003E0EC(void) {
    return D_8006DBCC;
}
