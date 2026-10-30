#ifndef BASH_N2O_H
#define BASH_N2O_H

/* N2O and NOX, the N. Gin and Oxide boss games (overlay n2o).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

enum N2O_AST_types {
    N2O_AST_zero = 0x1B00,
    AST_N2OBall,
    AST_N2Omissile,
    AST_N2Ofirstmissile,
    AST_N2OmissileExplode,
    AST_N2ODeadWall,
    NOXfill
};

enum NOX_AST_types {
    NOX_AST_zero = 0x1C00,
    AST_AsteroidRoll,
    AST_NitroBoss,
    AST_DummyCamera,
    AST_NitroMissile,
    AST_NitroMine,
    AST_SpiralSat,
    AST_LaserShoot,
    AST_MineSFX,
    AST_NitroSmokeBoost
};

#endif /* BASH_N2O_H */
