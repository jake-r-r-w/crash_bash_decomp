#ifndef BASH_SPIN_H
#define BASH_SPIN_H

/* Spin (overlay spin).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

enum SPIN_AST_types {
    SPIN_AST_zero = 0x1800,
    AST_HazardType,
    AST_Balloon
};

#endif /* BASH_SPIN_H */
