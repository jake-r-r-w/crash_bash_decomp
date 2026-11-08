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
#elif defined(_WIN32)
OVL_SECTION(".ov" OVL_STR(OVL_NAME) "$b")
#elif defined(__APPLE__)
OVL_SECTION("__DATA,__ov" OVL_STR(OVL_NAME))
#else
OVL_SECTION("ovl_" OVL_STR(OVL_NAME))
#endif
