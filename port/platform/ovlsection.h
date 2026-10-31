/* Force-included in each slot overlay's C (CMakeLists.txt, with OVL_NAME set): its data goes in a section of its own, between the
 * marks in overlays.c. -fno-zero-initialized-in-bss keeps its zeroed data there too. */
#define OVL_STR2(x) #x
#define OVL_STR(x) OVL_STR2(x)
#define OVL_PRAGMA(x) _Pragma(#x)
#define OVL_SECTION(name) OVL_PRAGMA(clang section data = name)
OVL_SECTION(".ov" OVL_STR(OVL_NAME) "$b")
