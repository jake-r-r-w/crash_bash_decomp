/* Force-included in each slot overlay's C (CMakeLists.txt, with OVL_NAME set): its data goes in a section of its own, whose ends
 * overlays.c knows. -fno-zero-initialized-in-bss keeps its zeroed data there too.
 *   Windows (PE): .ovNAME$b, which the linker puts between overlays.c's marks in .ovNAME$a and .ovNAME$c, as it sorts a section's
 *     $ parts by name.
 *   Linux (ELF): ovl_NAME, a name that's a C identifier, so the linker marks its ends with __start_ovl_NAME and __stop_ovl_NAME (ELF
 *     linkers don't sort $ parts: there the marks ended up side by side after everything, around nothing).
 *   The Mac (Mach-O): __DATA,__ovNAME, whose ends its linker gives as section$start$__DATA$__ovNAME and section$end$... (not yet built). */
#define OVL_STR2(x) #x
#define OVL_STR(x) OVL_STR2(x)
#define OVL_PRAGMA(x) _Pragma(#x)
#define OVL_SECTION(name) OVL_PRAGMA(clang section data = name)
#if !defined(__clang__)
/* GCC has no pragma for it (overlays.c) */
#define OVL_HERE
#define OVL_KEEP
#elif defined(_WIN32)
#define OVL_HERE OVL_SECTION(".ov" OVL_STR(OVL_NAME) "$b")
#elif defined(__APPLE__)
#define OVL_HERE OVL_SECTION("__DATA,__ov" OVL_STR(OVL_NAME))
#else
#define OVL_HERE OVL_SECTION("ovl_" OVL_STR(OVL_NAME))
#endif
#ifndef OVL_KEEP
/* Data between OVL_KEEP and OVL_HERE is the program's ordinary data, which a restart doesn't put back: what's made of the player's
 * own files (the hero packs' models, bonusheroes.c), and not the game's state, whose memory is freed and made again as they change */
#define OVL_KEEP OVL_SECTION("")
#endif
OVL_HERE
