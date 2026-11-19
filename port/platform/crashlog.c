/* The program's log, and what it says when the game crashes or sticks (docs/PORT.md, The log).
 *
 * A player's run writes its output to a file of its own in logs/ beside the program (crashbash-DATE-TIME.txt; in the settings folder's
 * logs/ where the program's folder can't be written), and logs not written to in 14 days are deleted, so there's always something to
 * send with a report. It starts with the version, the build and the machine. A crash says what it was (reading, writing
 * or running an address), what's at that address (which module, which section, the nearest symbol: a write to read-only data names
 * the data), every frame of the stack by module and function, the registers and the game's state, and on a player's run a message
 * says where the log is. The release keeps its symbol table for this, and the program reads its own file for it as it crashes: it
 * costs nothing until then. */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* the registers' names in a signal's context, and dladdr */
#endif
#if defined(_WIN32) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0A00 /* Windows 10's calls: a thread's stack limits */
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include <SDL3/SDL.h>
#include "crashlog.h"
#include "platform.h"

void game_log_state(void); /* game/main/main.c: what the game's doing, for a crash */
int platform_vblanks(void);

static char LogPath[1100];
static int PlayerRun;
static Uint64 StartTicks;

const char *crashlog_path(void) {
    return LogPath[0] != 0 ? LogPath : NULL;
}

void crashlog_time(char *out, int size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (t == NULL || strftime(out, size, "%Y-%m-%d %H:%M:%S", t) == 0) {
        SDL_snprintf(out, size, "?");
    }
}

/* ---------------------------------------------------------------------------------------------------------------------------- */
/* The log file */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>

/* Whether the output's already been sent somewhere: a file or a pipe */
static int output_redirected(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD type = h == NULL || h == INVALID_HANDLE_VALUE ? FILE_TYPE_UNKNOWN : GetFileType(h);

    return type == FILE_TYPE_DISK || type == FILE_TYPE_PIPE;
}
#else
#include <unistd.h>
#include <sys/stat.h>

static int output_redirected(void) {
    struct stat st;

    return fstat(STDOUT_FILENO, &st) == 0 && (S_ISREG(st.st_mode) || S_ISFIFO(st.st_mode));
}
#endif

/* Old logs: a run's file is crashbash-DATE-TIME.txt, and one not written to for this long goes as the next run starts */
#define LOG_KEEP_DAYS 14
#define LOG_PREFIX "crashbash-"
#define MAX_OLD_LOGS 256

typedef struct {
    char names[MAX_OLD_LOGS][96];
    int count;
} LogList;

static SDL_EnumerationResult list_log(void *data, const char *dir, const char *name) {
    LogList *l = data;
    size_t n = strlen(name);

    (void)dir;
    if (strncmp(name, LOG_PREFIX, strlen(LOG_PREFIX)) == 0 && n > 4 && strcmp(name + n - 4, ".txt") == 0 && n < sizeof(l->names[0])) {
        SDL_strlcpy(l->names[l->count++], name, sizeof(l->names[0]));
    }
    return l->count < MAX_OLD_LOGS ? SDL_ENUM_CONTINUE : SDL_ENUM_SUCCESS;
}

static void clear_old_logs(const char *folder) {
    static LogList l;
    SDL_Time now;
    int i;

    l.count = 0;
    if (!SDL_GetCurrentTime(&now) || !SDL_EnumerateDirectory(folder, list_log, &l)) {
        return;
    }
    for (i = 0; i < l.count; i++) {
        char path[1200];
        SDL_PathInfo info;

        SDL_snprintf(path, sizeof(path), "%s%s", folder, l.names[i]);
        if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE
            && now - info.modify_time > (SDL_Time)LOG_KEEP_DAYS * 24 * 3600 * SDL_NS_PER_SECOND) {
            SDL_RemovePath(path);
        }
    }
}

/* This run's log, crashbash-DATE-TIME.txt in a folder's logs/, made empty: 0 if it can't be written there */
static int new_log(const char *folder) {
    char dir[1100], stamp[32];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    FILE *f;
    int n;

    /* in the folder's own separator, as the player's shown the path */
    SDL_snprintf(dir, sizeof(dir), "%slogs%c", folder, strchr(folder, '\\') != NULL ? '\\' : '/');
    SDL_CreateDirectory(dir);
    if (t == NULL || strftime(stamp, sizeof(stamp), "%Y-%m-%d-%H%M%S", t) == 0) {
        SDL_strlcpy(stamp, "now", sizeof(stamp));
    }
    /* two copies started in the same second (online play on one PC) each have their own */
    for (n = 1; n < 100; n++) {
        if (n == 1) {
            SDL_snprintf(LogPath, sizeof(LogPath), "%s" LOG_PREFIX "%s.txt", dir, stamp);
        } else {
            SDL_snprintf(LogPath, sizeof(LogPath), "%s" LOG_PREFIX "%s-%d.txt", dir, stamp, n);
        }
        if (!SDL_GetPathInfo(LogPath, NULL)) {
            break;
        }
    }
    f = fopen(LogPath, "w");
    if (f == NULL) {
        LogPath[0] = 0;
        return 0;
    }
    fclose(f);
    clear_old_logs(dir);
    return 1;
}

void crashlog_open(const char *folder, int testing) {
    const char *base = platform_base_path();

    StartTicks = SDL_GetTicks();
    PlayerRun = !testing;
    if (testing || output_redirected()) {
        return;
    }
    /* beside the program, where a player looks first (the user's word, 2026-11-19), or where it can't be written, in the settings
       folder */
    if (!(base != NULL && new_log(base)) && !(folder != NULL && new_log(folder))) {
        return;
    }
    /* both streams append to the one file, so neither writes over the other */
    if (freopen(LogPath, "a", stdout) == NULL || freopen(LogPath, "a", stderr) == NULL) {
        LogPath[0] = 0;
        return;
    }
    /* unbuffered: what's there when the program dies, however it dies, is everything it said. It says little (a line a level). */
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

/* ---------------------------------------------------------------------------------------------------------------------------- */
/* The machine */

#ifdef _WIN32
static void reg_text(HKEY root, const char *key, const char *name, char *out, DWORD size) {
    out[0] = 0;
    if (RegGetValueA(root, key, name, RRF_RT_REG_SZ, NULL, out, &size) != ERROR_SUCCESS) {
        out[0] = 0;
    }
}

static DWORD reg_number(HKEY root, const char *key, const char *name) {
    DWORD value = 0, size = sizeof(value);

    return RegGetValueA(root, key, name, RRF_RT_REG_DWORD, NULL, &value, &size) == ERROR_SUCCESS ? value : 0;
}

static void system_text(char *out, int size) {
    static const char *key = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
    char product[128], release[64], build[32];
    char *ten;
    DWORD ubr = reg_number(HKEY_LOCAL_MACHINE, key, "UBR");

    reg_text(HKEY_LOCAL_MACHINE, key, "ProductName", product, sizeof(product));
    reg_text(HKEY_LOCAL_MACHINE, key, "DisplayVersion", release, sizeof(release));
    reg_text(HKEY_LOCAL_MACHINE, key, "CurrentBuildNumber", build, sizeof(build));
    /* Windows 11 still calls itself Windows 10 there: its builds are from 22000 */
    if (atoi(build) >= 22000 && (ten = strstr(product, "Windows 10")) != NULL) {
        ten[9] = '1';
    }
    SDL_snprintf(out, size, "%s %s, build %s.%lu", product[0] ? product : "Windows", release, build, (unsigned long)ubr);
}

static void cpu_text(char *out, int size) {
    char name[128];

    reg_text(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", name, sizeof(name));
    SDL_snprintf(out, size, "%s", name[0] ? name : "unknown");
}
#else
#include <sys/utsname.h>
#ifdef SDL_PLATFORM_ANDROID
#include <sys/system_properties.h>
#endif

/* The value of a "KEY<sep>VALUE" line of a text file, trimmed of quotes and spaces */
static int file_value(const char *path, const char *key, char sep, char *out, int size) {
    FILE *f = fopen(path, "r");
    char line[512];
    size_t n = strlen(key);

    out[0] = 0;
    if (f == NULL) {
        return 0;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        char *v = line + n, *end;

        if (strncmp(line, key, n) != 0) {
            continue;
        }
        while (*v == ' ' || *v == '\t') {
            v++;
        }
        if (*v != sep) {
            continue;
        }
        v++;
        while (*v == ' ' || *v == '"') {
            v++;
        }
        end = v + strlen(v);
        while (end > v && (end[-1] == '\n' || end[-1] == '\r' || end[-1] == '"' || end[-1] == ' ')) {
            *--end = 0;
        }
        SDL_strlcpy(out, v, size);
        break;
    }
    fclose(f);
    return out[0] != 0;
}

static void system_text(char *out, int size) {
    struct utsname u;
    char name[256] = "";

    if (uname(&u) != 0) {
        SDL_strlcpy(u.sysname, "?", sizeof(u.sysname));
        u.release[0] = u.machine[0] = 0;
    }
#ifdef SDL_PLATFORM_ANDROID
    {
        char release[PROP_VALUE_MAX] = "", sdk[PROP_VALUE_MAX] = "", maker[PROP_VALUE_MAX] = "", model[PROP_VALUE_MAX] = "";

        __system_property_get("ro.build.version.release", release);
        __system_property_get("ro.build.version.sdk", sdk);
        __system_property_get("ro.product.manufacturer", maker);
        __system_property_get("ro.product.model", model);
        SDL_snprintf(name, sizeof(name), "Android %s (API %s), %s %s", release, sdk, maker, model);
    }
#else
    file_value("/etc/os-release", "PRETTY_NAME", '=', name, sizeof(name));
#endif
    SDL_snprintf(out, size, "%s, %s %s %s", name[0] ? name : u.sysname, u.sysname, u.release, u.machine);
}

static void cpu_text(char *out, int size) {
    if (!file_value("/proc/cpuinfo", "model name", ':', out, size) && !file_value("/proc/cpuinfo", "Hardware", ':', out, size)) {
        SDL_snprintf(out, size, "unknown");
    }
}
#endif

void crashlog_system(void) {
    char when[32], os[400], cpu[160];

    crashlog_time(when, sizeof(when));
    system_text(os, sizeof(os));
    cpu_text(cpu, sizeof(cpu));
    printf("Crash Bash PC port %s (commit %s, built %s), started %s\n", BASH_VERSION, BASH_COMMIT, __DATE__, when);
    printf("system: %s\n", os);
    printf("cpu: %s, %d threads; memory: %d MB\n", cpu, SDL_GetNumLogicalCPUCores(), SDL_GetSystemRAM());
    if (LogPath[0] != 0) {
        printf("log: %s\n", LogPath);
    }
}

/* ---------------------------------------------------------------------------------------------------------------------------- */
/* A crash */

/* How long the program's been going, and how far the game is, for a crash's first lines */
static void print_when(void) {
    char when[32];
    Uint64 ms = SDL_GetTicks() - StartTicks;

    crashlog_time(when, sizeof(when));
    printf("crash: at %s, %llu:%02llu after the start, vblank %d of this start\n", when, (unsigned long long)(ms / 60000),
           (unsigned long long)(ms / 1000 % 60), platform_vblanks());
}

/* The end of a crash's report: the game's own state last, as reading it is what's likeliest to crash again */
static void print_game(void) {
    printf("crash: what the game was doing:\n");
    fflush(stdout);
    game_log_state();
    printf("crash: end of the report\n");
    fflush(stdout);
    fflush(stderr);
}

#ifdef _WIN32
#include <psapi.h>

/* The program's own symbol table, read from its file when it's first needed (a release links it in: CMakeLists.txt) */
static unsigned char *Syms;
static DWORD NumSyms;
static const char *Strings;
static DWORD StringsSize;
static int SymsTried;
static DWORD MainThread;

static void load_symbols(void) {
    WCHAR path[2048];
    HANDLE f;
    DWORD got, lfanew = 0, at_syms, num, strings = 0;
    IMAGE_FILE_HEADER fh;
    LARGE_INTEGER to;

    if (SymsTried) {
        return;
    }
    SymsTried = 1;
    if (GetModuleFileNameW(NULL, path, 2048) == 0) {
        return;
    }
    f = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (f == INVALID_HANDLE_VALUE) {
        return;
    }
    to.QuadPart = 0x3C;
    if (!SetFilePointerEx(f, to, NULL, FILE_BEGIN) || !ReadFile(f, &lfanew, 4, &got, NULL) || got != 4) {
        goto done;
    }
    to.QuadPart = lfanew + 4;
    if (!SetFilePointerEx(f, to, NULL, FILE_BEGIN) || !ReadFile(f, &fh, sizeof(fh), &got, NULL) || got != sizeof(fh)) {
        goto done;
    }
    at_syms = fh.PointerToSymbolTable;
    num = fh.NumberOfSymbols;
    if (at_syms == 0 || num == 0 || num > 0x1000000) {
        goto done;
    }
    to.QuadPart = at_syms + (LONGLONG)num * IMAGE_SIZEOF_SYMBOL;
    if (!SetFilePointerEx(f, to, NULL, FILE_BEGIN) || !ReadFile(f, &strings, 4, &got, NULL) || got != 4 || strings > 0x10000000) {
        goto done;
    }
    /* the heap may be what's broken, so not from it */
    Syms = VirtualAlloc(NULL, (SIZE_T)num * IMAGE_SIZEOF_SYMBOL + strings + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (Syms == NULL) {
        goto done;
    }
    to.QuadPart = at_syms;
    if (!SetFilePointerEx(f, to, NULL, FILE_BEGIN) || !ReadFile(f, Syms, num * IMAGE_SIZEOF_SYMBOL + strings, &got, NULL)
        || got != num * IMAGE_SIZEOF_SYMBOL + strings) {
        Syms = NULL;
        goto done;
    }
    NumSyms = num;
    Strings = (const char *)Syms + num * IMAGE_SIZEOF_SYMBOL;
    StringsSize = strings;
done:
    CloseHandle(f);
}

static IMAGE_SECTION_HEADER *section_of(HMODULE module, uintptr_t addr, int *index) {
    IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)module;
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)module + dos->e_lfanew);
    IMAGE_SECTION_HEADER *s = IMAGE_FIRST_SECTION(nt);
    uintptr_t rva = addr - (uintptr_t)module;
    int i;

    for (i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        DWORD size = s[i].Misc.VirtualSize > s[i].SizeOfRawData ? s[i].Misc.VirtualSize : s[i].SizeOfRawData;

        if (rva >= s[i].VirtualAddress && rva < s[i].VirtualAddress + size) {
            *index = i;
            return &s[i];
        }
    }
    return NULL;
}

/* The program's nearest symbol at or before an address in one of its sections */
static const char *own_symbol(uintptr_t addr, int section, uintptr_t start, uintptr_t *offset) {
    static char name[9];
    const unsigned char *best = NULL;
    DWORD best_value = 0, want = (DWORD)(addr - start), i;

    load_symbols();
    for (i = 0; i < NumSyms; i++) {
        const IMAGE_SYMBOL *s = (const IMAGE_SYMBOL *)(Syms + i * IMAGE_SIZEOF_SYMBOL);

        if (s->SectionNumber == section + 1 && (s->StorageClass == IMAGE_SYM_CLASS_EXTERNAL || s->StorageClass == IMAGE_SYM_CLASS_STATIC)
            && s->Value <= want && (best == NULL || s->Value >= best_value)) {
            const char *n = s->N.Name.Short != 0 ? (const char *)s->N.ShortName : s->N.Name.Long < StringsSize ? Strings + s->N.Name.Long : ".";

            if (n[0] != '.') {
                best = (const unsigned char *)s;
                best_value = s->Value;
            }
        }
        i += s->NumberOfAuxSymbols;
    }
    if (best == NULL) {
        return NULL;
    }
    *offset = want - best_value;
    {
        const IMAGE_SYMBOL *s = (const IMAGE_SYMBOL *)best;

        if (s->N.Name.Short == 0) {
            return Strings + s->N.Name.Long;
        }
        memcpy(name, s->N.ShortName, 8);
        name[8] = 0;
        return name;
    }
}

/* Another module's nearest export at or before an address: a system DLL's or a driver's function, near enough */
static const char *export_near(HMODULE module, uintptr_t addr, uintptr_t *offset) {
    IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)module;
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)module + dos->e_lfanew);
    IMAGE_DATA_DIRECTORY *dir = &nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    IMAGE_EXPORT_DIRECTORY *ex;
    DWORD *names, *funcs, best = 0, rva = (DWORD)(addr - (uintptr_t)module), i;
    WORD *ords;
    const char *found = NULL;

    if (dir->VirtualAddress == 0 || dir->Size == 0) {
        return NULL;
    }
    ex = (IMAGE_EXPORT_DIRECTORY *)((char *)module + dir->VirtualAddress);
    names = (DWORD *)((char *)module + ex->AddressOfNames);
    funcs = (DWORD *)((char *)module + ex->AddressOfFunctions);
    ords = (WORD *)((char *)module + ex->AddressOfNameOrdinals);
    for (i = 0; i < ex->NumberOfNames; i++) {
        DWORD f = funcs[ords[i]];

        if (f <= rva && f >= best && !(f >= dir->VirtualAddress && f < dir->VirtualAddress + dir->Size)) {
            best = f;
            found = (const char *)module + names[i];
        }
    }
    *offset = rva - best;
    return found;
}

static const char *section_kind(const char *name) {
    return strcmp(name, ".text") == 0 ? "code" : strcmp(name, ".rdata") == 0 ? "read-only data" : strcmp(name, ".pdata") == 0 ? "unwind data"
         : strncmp(name, ".ov", 3) == 0 ? "the game's data" : "data";
}

/* What's at an address: its module and where in it, its section and nearest symbol, or the stack or the heap */
static void describe(uintptr_t addr, char *out, int size) {
    HMODULE module = NULL;
    WCHAR wide[1024];
    char file[512];
    const char *base;
    int index;

    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)addr, &module)
        || module == NULL) {
        ULONG_PTR low = 0, high = 0;
        MEMORY_BASIC_INFORMATION mbi;

        GetCurrentThreadStackLimits(&low, &high);
        if (addr >= low && addr < high) {
            SDL_snprintf(out, size, "on this thread's stack");
        } else if (addr < 0x10000) {
            SDL_snprintf(out, size, "near address 0: a null pointer");
        } else if (VirtualQuery((void *)addr, &mbi, sizeof(mbi)) == sizeof(mbi) && mbi.State == MEM_COMMIT) {
            SDL_snprintf(out, size, "in allocated memory (%s)", mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE) ? "writable" : "not writable");
        } else {
            SDL_snprintf(out, size, "in no memory the program has");
        }
        return;
    }
    if (GetModuleFileNameW(module, wide, 1024) == 0 || WideCharToMultiByte(CP_UTF8, 0, wide, -1, file, sizeof(file), NULL, NULL) == 0) {
        SDL_strlcpy(file, "?", sizeof(file));
    }
    base = strrchr(file, '\\') != NULL ? strrchr(file, '\\') + 1 : file;
    if (module == GetModuleHandleW(NULL)) {
        IMAGE_SECTION_HEADER *s = section_of(module, addr, &index);
        char name[9] = "?";
        uintptr_t offset = 0;
        const char *sym;

        if (s != NULL) {
            memcpy(name, s->Name, 8);
            name[8] = 0;
        }
        sym = s != NULL ? own_symbol(addr, index, (uintptr_t)module + s->VirtualAddress, &offset) : NULL;
        if (sym != NULL) {
            SDL_snprintf(out, size, "%s+0x%llX, %s+0x%llX (%s %s)", base, (unsigned long long)(addr - (uintptr_t)module), sym,
                         (unsigned long long)offset, name, section_kind(name));
        } else {
            SDL_snprintf(out, size, "%s+0x%llX (%s %s)", base, (unsigned long long)(addr - (uintptr_t)module), name, section_kind(name));
        }
    } else {
        uintptr_t offset = 0;
        const char *ex = export_near(module, addr, &offset);

        if (ex != NULL) {
            SDL_snprintf(out, size, "%s+0x%llX, near %s+0x%llX", base, (unsigned long long)(addr - (uintptr_t)module), ex, (unsigned long long)offset);
        } else {
            SDL_snprintf(out, size, "%s+0x%llX", base, (unsigned long long)(addr - (uintptr_t)module));
        }
    }
}

/* The return addresses up the stack: on one line, for llvm-symbolizer (playtest.mjs reads it), then one a line with what each is */
static void print_stack(CONTEXT c) {
    DWORD64 at[48];
    int n = 0, i;

    at[n++] = c.Rip;
    while (n < 48) {
        DWORD64 base = 0, frame = 0;
        PVOID handler = NULL;
        PRUNTIME_FUNCTION fn = RtlLookupFunctionEntry(c.Rip, &base, NULL);

        if (fn != NULL) {
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, base, c.Rip, fn, &c, &handler, &frame, NULL);
        } else if (c.Rsp != 0 && !IsBadReadPtr((void *)c.Rsp, 8)) {
            c.Rip = *(DWORD64 *)c.Rsp; /* a leaf function */
            c.Rsp += 8;
        } else {
            break;
        }
        if (c.Rip == 0) {
            break;
        }
        at[n++] = c.Rip;
    }
    printf("stack:");
    for (i = 0; i < n; i++) {
        printf(" %llX", (unsigned long long)at[i]);
    }
    printf("\n");
    for (i = 0; i < n; i++) {
        char what[700];

        /* a return address is the instruction after the call: what's before it is the call's own function */
        describe(i == 0 ? at[i] : at[i] - 1, what, sizeof(what));
        printf("  %2d %s\n", i, what);
    }
    fflush(stdout);
}

static void print_registers(const CONTEXT *c) {
    printf("registers: rip %llX rsp %llX rbp %llX\n  rax %llX rbx %llX rcx %llX rdx %llX rsi %llX rdi %llX\n"
           "  r8 %llX r9 %llX r10 %llX r11 %llX r12 %llX r13 %llX r14 %llX r15 %llX\n",
           c->Rip, c->Rsp, c->Rbp, c->Rax, c->Rbx, c->Rcx, c->Rdx, c->Rsi, c->Rdi, c->R8, c->R9, c->R10, c->R11, c->R12, c->R13, c->R14, c->R15);
}

void crashlog_thread_stack(void *thread) {
    CONTEXT c;

    c.ContextFlags = CONTEXT_FULL;
    if (GetThreadContext((HANDLE)thread, &c)) {
        print_stack(c);
    }
}

static const char *code_name(DWORD code) {
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION:
        return "an access violation";
    case EXCEPTION_STACK_OVERFLOW:
        return "a stack overflow";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        return "a division by zero";
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        return "an illegal instruction";
    case EXCEPTION_PRIV_INSTRUCTION:
        return "a privileged instruction";
    case EXCEPTION_IN_PAGE_ERROR:
        return "a page that couldn't be read in";
    case EXCEPTION_BREAKPOINT:
        return "a breakpoint";
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        return "a misaligned access";
    case 0xC0000374:
        return "a broken heap";
    case 0xC0000409:
        return "a stack buffer overrun";
    default:
        return "an exception";
    }
}

/* The player's told, and where the log is */
static void tell_player(const char *what) {
    char text[1600];
    WCHAR wide[1600];

    if (!PlayerRun) {
        return;
    }
    if (LogPath[0] != 0) {
        SDL_snprintf(text, sizeof(text), "Crash Bash has crashed (%s).\n\nWhat happened is in its log:\n%s\n\nPlease send that file with a report "
                     "of what you were doing.", what, LogPath);
    } else {
        SDL_snprintf(text, sizeof(text), "Crash Bash has crashed (%s).\n\nWhat happened is in the program's output.", what);
    }
    if (MultiByteToWideChar(CP_UTF8, 0, text, -1, wide, 1600) != 0) {
        MessageBoxW(NULL, wide, L"Crash Bash", MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
    }
}

static LONG WINAPI crash_report(EXCEPTION_POINTERS *e) {
    static volatile LONG inside;
    EXCEPTION_RECORD *r = e->ExceptionRecord;
    uintptr_t image = (uintptr_t)GetModuleHandleW(NULL);
    unsigned long long addr = r->NumberParameters >= 2 ? (unsigned long long)r->ExceptionInformation[1] : 0;
    const char *how = "at";
    char what[700], said[200];
    PROCESS_MEMORY_COUNTERS mem;

    /* a crash in the report: there's nothing more to say */
    if (InterlockedExchange(&inside, 1) != 0) {
        return EXCEPTION_EXECUTE_HANDLER;
    }
    fflush(stdout); /* the log up to here */
    if (r->ExceptionCode == EXCEPTION_ACCESS_VIOLATION || r->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) {
        how = r->ExceptionInformation[0] == 1 ? "writing" : r->ExceptionInformation[0] == 8 ? "running" : "reading";
    }
    /* the first line as it always was (playtest.mjs reads it) */
    printf("crash: code %08lX, %s %llX, image %llX\n", r->ExceptionCode, how, addr, (unsigned long long)image);
    print_when();
    describe((uintptr_t)r->ExceptionAddress, what, sizeof(what));
    printf("crash: %s, in %s\n", code_name(r->ExceptionCode), what);
    if (r->ExceptionCode == EXCEPTION_ACCESS_VIOLATION || r->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) {
        describe((uintptr_t)addr, what, sizeof(what));
        printf("crash: %s %llX, which is %s\n", how, addr, what);
    }
    printf("crash: thread %lu, %s\n", GetCurrentThreadId(),
           GetCurrentThreadId() == MainThread ? "the game's" : "not the game's (the sound's, the network's or the system's)");
    mem.cb = sizeof(mem);
    if (K32GetProcessMemoryInfo(GetCurrentProcess(), &mem, sizeof(mem))) {
        printf("crash: memory in use %llu MB (at most %llu MB)\n", (unsigned long long)(mem.WorkingSetSize >> 20),
               (unsigned long long)(mem.PeakWorkingSetSize >> 20));
    }
    print_stack(*e->ContextRecord);
    print_registers(e->ContextRecord);
    print_game();
    SDL_snprintf(said, sizeof(said), "%s %s %llX", code_name(r->ExceptionCode), how, addr);
    tell_player(said);
    return EXCEPTION_EXECUTE_HANDLER;
}

/* abort(): an assert, or the C library giving up */
static void abort_report(int sig) {
    CONTEXT c;

    (void)sig;
    fflush(stdout);
    printf("crash: code ABORT, the program stopped itself, image %llX\n", (unsigned long long)(uintptr_t)GetModuleHandleW(NULL));
    print_when();
    RtlCaptureContext(&c);
    print_stack(c);
    print_game();
    tell_player("it stopped itself");
    _exit(3);
}

void crashlog_install(void) {
    ULONG guarantee = 64 * 1024;

    MainThread = GetCurrentThreadId();
    SetThreadStackGuarantee(&guarantee); /* a stack overflow leaves this much for the report */
    SetUnhandledExceptionFilter(crash_report);
    signal(SIGABRT, abort_report);
}

#else /* Linux and Android */
#include <dlfcn.h>
#include <elf.h>
#include <fcntl.h>
#include <link.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unwind.h>

/* The program's own symbol table, from its file (Linux's /proc/self/exe, or the library Android loaded, when it can be read) */
static const ElfW(Sym) *Syms;
static size_t NumSyms;
static const char *Strings;
static uintptr_t SymsBase, OwnBase;
static int SymsTried;

static void load_symbols(void) {
    Dl_info info;
    const char *path;
    int fd, i;
    struct stat st;
    const unsigned char *file;
    const ElfW(Ehdr) *eh;
    const ElfW(Shdr) *sh;

    if (SymsTried) {
        return;
    }
    SymsTried = 1;
    if (dladdr((void *)load_symbols, &info) == 0) {
        return;
    }
    SymsBase = OwnBase = (uintptr_t)info.dli_fbase;
#ifdef SDL_PLATFORM_ANDROID
    path = info.dli_fname;
#else
    path = "/proc/self/exe";
#endif
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        return;
    }
    if (fstat(fd, &st) != 0 || (file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
        close(fd);
        return;
    }
    close(fd);
    eh = (const ElfW(Ehdr) *)file;
    if (memcmp(eh->e_ident, ELFMAG, SELFMAG) != 0 || eh->e_shoff == 0) {
        return;
    }
    sh = (const ElfW(Shdr) *)(file + eh->e_shoff);
    for (i = 0; i < eh->e_shnum; i++) {
        if (sh[i].sh_type == SHT_SYMTAB && sh[i].sh_link < eh->e_shnum) {
            Syms = (const ElfW(Sym) *)(file + sh[i].sh_offset);
            NumSyms = sh[i].sh_size / sizeof(ElfW(Sym));
            Strings = (const char *)file + sh[sh[i].sh_link].sh_offset;
            break;
        }
    }
    /* a program that isn't position-independent has its symbols at their own addresses */
    if (eh->e_type == ET_EXEC) {
        SymsBase = 0;
    }
}

static const char *own_symbol(uintptr_t addr, uintptr_t *offset) {
    const ElfW(Sym) *best = NULL;
    size_t i;

    load_symbols();
    for (i = 0; i < NumSyms; i++) {
        const ElfW(Sym) *s = &Syms[i];
        int type = ELF64_ST_TYPE(s->st_info);
        uintptr_t at = SymsBase + s->st_value;

        if ((type == STT_FUNC || type == STT_OBJECT) && s->st_shndx != SHN_UNDEF && at <= addr && (best == NULL || s->st_value > best->st_value)
            && (s->st_size == 0 || addr < at + s->st_size + 4096)) {
            best = s;
        }
    }
    if (best == NULL) {
        return NULL;
    }
    *offset = addr - (SymsBase + best->st_value);
    return Strings + best->st_name;
}

static void describe(uintptr_t addr, char *out, int size) {
    Dl_info info;
    uintptr_t offset = 0;
    const char *sym, *file;

    if (addr < 0x10000) {
        SDL_snprintf(out, size, "near address 0: a null pointer");
        return;
    }
    if (dladdr((void *)addr, &info) == 0 || info.dli_fname == NULL) {
        SDL_snprintf(out, size, "in no module");
        return;
    }
    file = strrchr(info.dli_fname, '/') != NULL ? strrchr(info.dli_fname, '/') + 1 : info.dli_fname;
    load_symbols();
    sym = (uintptr_t)info.dli_fbase == OwnBase ? own_symbol(addr, &offset) : NULL;
    if (sym == NULL && info.dli_sname != NULL) {
        sym = info.dli_sname;
        offset = addr - (uintptr_t)info.dli_saddr;
    }
    if (sym != NULL) {
        SDL_snprintf(out, size, "%s+0x%llX, %s+0x%llX", file, (unsigned long long)(addr - (uintptr_t)info.dli_fbase), sym, (unsigned long long)offset);
    } else {
        SDL_snprintf(out, size, "%s+0x%llX", file, (unsigned long long)(addr - (uintptr_t)info.dli_fbase));
    }
}

typedef struct {
    uintptr_t at[48];
    int n, started;
    uintptr_t from;
} Walk;

static _Unwind_Reason_Code walk_one(struct _Unwind_Context *ctx, void *arg) {
    Walk *w = arg;
    uintptr_t ip = _Unwind_GetIP(ctx);

    /* the handler's own frames come first: from the crashed instruction on */
    if (!w->started) {
        if (ip != w->from && ip != w->from + 1) {
            return _URC_NO_REASON;
        }
        w->started = 1;
        return _URC_NO_REASON;
    }
    if (ip == 0 || w->n >= 48) {
        return _URC_END_OF_STACK;
    }
    w->at[w->n++] = ip;
    return _URC_NO_REASON;
}

static void print_stack(uintptr_t pc) {
    Walk w = { .n = 0, .started = pc == 0, .from = pc };
    char what[600];
    int i;

    if (pc != 0) {
        w.at[w.n++] = pc;
    }
    _Unwind_Backtrace(walk_one, &w);
    if (pc != 0 && w.n == 1 && !w.started) {
        /* the walk never reached the crashed frame: what it saw is all there is */
        w.started = 1;
        w.from = 0;
        _Unwind_Backtrace(walk_one, &w);
    }
    printf("stack:");
    for (i = 0; i < w.n; i++) {
        printf(" %llX", (unsigned long long)w.at[i]);
    }
    printf("\n");
    for (i = 0; i < w.n; i++) {
        describe(i == 0 ? w.at[i] : w.at[i] - 1, what, sizeof(what));
        printf("  %2d %s\n", i, what);
    }
}

static const char *signal_name(int sig) {
    switch (sig) {
    case SIGSEGV:
        return "a segmentation fault";
    case SIGBUS:
        return "a bus error";
    case SIGFPE:
        return "an arithmetic error";
    case SIGILL:
        return "an illegal instruction";
    case SIGABRT:
        return "an abort: the program stopped itself";
    default:
        return "a signal";
    }
}

static void crash_signal(int sig, siginfo_t *si, void *uc_) {
    ucontext_t *uc = uc_;
    uintptr_t pc = 0, image = 0;
    const char *how = "at";
    char what[600];
    Dl_info info;

    fflush(stdout);
#if defined(__x86_64__)
    pc = (uintptr_t)uc->uc_mcontext.gregs[REG_RIP];
    if (sig == SIGSEGV || sig == SIGBUS) {
        how = (uc->uc_mcontext.gregs[REG_ERR] & 16) ? "running" : (uc->uc_mcontext.gregs[REG_ERR] & 2) ? "writing" : "reading";
    }
#elif defined(__aarch64__)
    pc = (uintptr_t)uc->uc_mcontext.pc;
#endif
    if (dladdr((void *)crash_signal, &info) != 0) {
        image = (uintptr_t)info.dli_fbase;
    }
    printf("crash: code %s (signal %d), %s %llX, image %llX\n", sig == SIGSEGV ? "SIGSEGV" : sig == SIGBUS ? "SIGBUS" : sig == SIGFPE ? "SIGFPE"
           : sig == SIGILL ? "SIGILL" : "SIGABRT", sig, how, (unsigned long long)(uintptr_t)si->si_addr, (unsigned long long)image);
    print_when();
    if (pc != 0) {
        describe(pc, what, sizeof(what));
        printf("crash: %s, in %s\n", signal_name(sig), what);
    }
    if (sig == SIGSEGV || sig == SIGBUS) {
        describe((uintptr_t)si->si_addr, what, sizeof(what));
        printf("crash: %s %llX, which is %s\n", how, (unsigned long long)(uintptr_t)si->si_addr, what);
    }
    print_stack(sig == SIGABRT ? 0 : pc);
#if defined(__x86_64__)
    {
        greg_t *g = uc->uc_mcontext.gregs;

        printf("registers: rip %llX rsp %llX rbp %llX\n  rax %llX rbx %llX rcx %llX rdx %llX rsi %llX rdi %llX\n"
               "  r8 %llX r9 %llX r10 %llX r11 %llX r12 %llX r13 %llX r14 %llX r15 %llX\n",
               g[REG_RIP], g[REG_RSP], g[REG_RBP], g[REG_RAX], g[REG_RBX], g[REG_RCX], g[REG_RDX], g[REG_RSI], g[REG_RDI], g[REG_R8], g[REG_R9],
               g[REG_R10], g[REG_R11], g[REG_R12], g[REG_R13], g[REG_R14], g[REG_R15]);
    }
#endif
    print_game();
    /* the default's next: the system's own report (Android's tombstone), and the exit code a crash has */
    signal(sig, SIG_DFL);
    raise(sig);
}

void crashlog_install(void) {
    static char alt[64 * 1024];
    stack_t ss = { .ss_sp = alt, .ss_size = sizeof(alt), .ss_flags = 0 };
    struct sigaction sa;
    const int sigs[] = { SIGSEGV, SIGBUS, SIGFPE, SIGILL, SIGABRT };
    size_t i;

    sigaltstack(&ss, NULL); /* a stack overflow leaves this for the report */
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = crash_signal;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK | SA_RESETHAND;
    sigemptyset(&sa.sa_mask);
    for (i = 0; i < sizeof(sigs) / sizeof(sigs[0]); i++) {
        sigaction(sigs[i], &sa, NULL);
    }
}
#endif
