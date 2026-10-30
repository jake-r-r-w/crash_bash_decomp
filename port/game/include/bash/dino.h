#ifndef BASH_DINO_H
#define BASH_DINO_H

/* Dino (overlay dino).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

enum DINO_AST_types {
    DINO_AST_zero = 0x1900,
    AST_DinoGem,
    AST_DinoScratch,
    AST_DinoSmoke
};

#endif /* BASH_DINO_H */
