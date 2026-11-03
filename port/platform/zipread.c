/* Reading the files in a zip, for mods. A zip ends with a list of its files, the central directory, each pointing back at a local header
 * with the file's bytes after it. Only what's needed is read: the list when a mod's found, and a file when it's wanted. Every number is
 * checked against the zip's size before it's used, and what a file unpacks to against its stated size, a limit and its CRC, as a zip can
 * come from anyone. Deflate is zlib's. */
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <zlib.h>
#include "zipread.h"

#define END_SIGNATURE 0x06054B50u
#define CENTRAL_SIGNATURE 0x02014B50u
#define LOCAL_SIGNATURE 0x04034B50u
#define END_BYTES 22
#define CENTRAL_BYTES 46
#define LOCAL_BYTES 30
#define MAX_COMMENT 0xFFFF
#define MAX_LIST_BYTES (16 * 1024 * 1024)
#define MAX_NAME 1024
#define ZIP64 0xFFFFFFFFu

static uint32_t get16(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8;
}

static uint32_t get32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

static int read_at(SDL_IOStream *io, Sint64 at, void *buf, size_t n) {
    return SDL_SeekIO(io, at, SDL_IO_SEEK_SET) == at && SDL_ReadIO(io, buf, n) == n;
}

void zip_close(ZipFile *zip) {
    int i;

    if (zip == NULL) {
        return;
    }
    for (i = 0; i < zip->count; i++) {
        free(zip->entries[i].name);
    }
    free(zip->entries);
    free(zip->path);
    free(zip);
}

/* The end record is the last thing in the file but for a comment after it, as long as it says. */
static long find_end(const uint8_t *tail, size_t bytes) {
    long i;

    for (i = (long)bytes - END_BYTES; i >= 0; i--) {
        if (get32(tail + i) == END_SIGNATURE && (size_t)i + END_BYTES + get16(tail + i + 20) == bytes) {
            return i;
        }
    }
    return -1;
}

/* An entry from the list's record e, or 0 if it's a folder or a name it won't take. */
static int take_entry(ZipEntry *z, const uint8_t *e, uint32_t name_bytes) {
    uint32_t i;

    if (name_bytes == 0 || name_bytes >= MAX_NAME || e[CENTRAL_BYTES + name_bytes - 1] == '/' || e[CENTRAL_BYTES + name_bytes - 1] == '\\') {
        return 0;
    }
    z->name = malloc(name_bytes + 1);
    if (z->name == NULL) {
        return 0;
    }
    for (i = 0; i < name_bytes; i++) {
        char c = (char)e[CENTRAL_BYTES + i];

        if (c == 0) {
            free(z->name);
            z->name = NULL;
            return 0;
        }
        z->name[i] = c == '\\' ? '/' : c;
    }
    z->name[name_bytes] = 0;
    z->method = get16(e + 10);
    z->crc = get32(e + 16);
    z->packed = get32(e + 20);
    z->size = get32(e + 24);
    z->local = get32(e + 42);
    z->locked = (get16(e + 8) & 1) != 0;
    /* a file past 4 GB, which no mod has: it's left unread */
    if (z->packed == ZIP64 || z->size == ZIP64 || z->local == ZIP64) {
        z->method = 0xFFFF;
    }
    return 1;
}

ZipFile *zip_open(const char *path, const char **problem) {
    SDL_IOStream *io = SDL_IOFromFile(path, "rb");
    uint8_t *tail = NULL, *list = NULL;
    ZipFile *zip = NULL;
    Sint64 size;
    size_t tail_bytes, at;
    uint32_t count, list_bytes, list_at, i;
    long end;

    *problem = "NOT A ZIP IT CAN READ";
    if (io == NULL) {
        *problem = "CAN'T BE OPENED";
        return NULL;
    }
    size = SDL_GetIOSize(io);
    if (size < END_BYTES || size >= (Sint64)ZIP64) {
        goto fail;
    }
    tail_bytes = (size_t)(size < END_BYTES + MAX_COMMENT ? size : END_BYTES + MAX_COMMENT);
    tail = malloc(tail_bytes);
    if (tail == NULL || !read_at(io, size - (Sint64)tail_bytes, tail, tail_bytes) || (end = find_end(tail, tail_bytes)) < 0) {
        goto fail;
    }
    count = get16(tail + end + 10);
    list_bytes = get32(tail + end + 12);
    list_at = get32(tail + end + 16);
    if (get16(tail + end + 4) != 0 || get16(tail + end + 6) != 0 || get16(tail + end + 8) != count) {
        *problem = "A ZIP SPLIT IN PARTS";
        goto fail;
    }
    if (count == 0xFFFF || list_bytes == ZIP64 || list_at == ZIP64) {
        *problem = "A ZIP64 ZIP, WHICH IT CAN'T READ";
        goto fail;
    }
    /* the list ends where the end record starts */
    if (list_bytes > MAX_LIST_BYTES || (Sint64)list_at + list_bytes > size - (Sint64)tail_bytes + end) {
        goto fail;
    }
    list = malloc(list_bytes != 0 ? list_bytes : 1);
    zip = calloc(1, sizeof(*zip));
    if (list == NULL || zip == NULL || !read_at(io, list_at, list, list_bytes)) {
        goto fail;
    }
    zip->entries = calloc(count != 0 ? count : 1, sizeof(ZipEntry));
    zip->path = malloc(strlen(path) + 1);
    if (zip->entries == NULL || zip->path == NULL) {
        goto fail;
    }
    strcpy(zip->path, path);
    for (at = 0, i = 0; i < count; i++) {
        const uint8_t *e = list + at;
        uint32_t name_bytes, more;

        if (at + CENTRAL_BYTES > list_bytes || get32(e) != CENTRAL_SIGNATURE) {
            goto fail;
        }
        name_bytes = get16(e + 28);
        more = name_bytes + get16(e + 30) + get16(e + 32);
        if (at + CENTRAL_BYTES + more > list_bytes) {
            goto fail;
        }
        if (take_entry(&zip->entries[zip->count], e, name_bytes)) {
            zip->count++;
        }
        at += CENTRAL_BYTES + more;
    }
    free(tail);
    free(list);
    SDL_CloseIO(io);
    *problem = NULL;
    return zip;

fail:
    free(tail);
    free(list);
    zip_close(zip);
    SDL_CloseIO(io);
    return NULL;
}

uint8_t *zip_read(const ZipFile *zip, int entry, size_t limit, size_t *size, const char **problem) {
    const ZipEntry *z;
    SDL_IOStream *io;
    uint8_t head[LOCAL_BYTES], *packed = NULL, *out = NULL;
    Sint64 file_size, data_at;
    int ok = 0;

    *size = 0;
    *problem = "BROKEN";
    if (zip == NULL || entry < 0 || entry >= zip->count) {
        return NULL;
    }
    z = &zip->entries[entry];
    if (z->locked) {
        *problem = "ENCRYPTED";
        return NULL;
    }
    if (z->method != 0 && z->method != 8) {
        *problem = "PACKED IN A WAY IT CAN'T READ";
        return NULL;
    }
    if (z->size > limit) {
        *problem = "TOO BIG";
        return NULL;
    }
    /* deflate makes nothing much bigger than it was */
    if (z->packed > z->size + z->size / 64 + 1024) {
        return NULL;
    }
    io = SDL_IOFromFile(zip->path, "rb");
    if (io == NULL) {
        *problem = "THE ZIP CAN'T BE OPENED";
        return NULL;
    }
    file_size = SDL_GetIOSize(io);
    if (!read_at(io, z->local, head, LOCAL_BYTES) || get32(head) != LOCAL_SIGNATURE) {
        goto done;
    }
    data_at = (Sint64)z->local + LOCAL_BYTES + get16(head + 26) + get16(head + 28);
    if (data_at + z->packed > file_size || (z->method == 0 && z->packed != z->size)) {
        goto done;
    }
    packed = malloc(z->packed != 0 ? z->packed : 1);
    /* a byte more than it says, so a file that unpacks to more is caught */
    out = malloc((size_t)z->size + 1);
    if (packed == NULL || out == NULL || (z->packed != 0 && !read_at(io, data_at, packed, z->packed))) {
        goto done;
    }
    if (z->method == 0) {
        memcpy(out, packed, z->size);
        ok = 1;
    } else {
        z_stream s;

        memset(&s, 0, sizeof(s));
        if (inflateInit2(&s, -MAX_WBITS) == Z_OK) {
            s.next_in = packed;
            s.avail_in = z->packed;
            s.next_out = out;
            s.avail_out = z->size + 1;
            ok = inflate(&s, Z_FINISH) == Z_STREAM_END && s.total_out == z->size;
            inflateEnd(&s);
        }
    }
    if (ok && (uint32_t)crc32(0L, out, z->size) != z->crc) {
        *problem = "DAMAGED";
        ok = 0;
    }

done:
    free(packed);
    SDL_CloseIO(io);
    if (!ok) {
        free(out);
        return NULL;
    }
    *size = z->size;
    *problem = NULL;
    return out;
}
