/* BPS patches, as beat specifies them and Floating IPS and port/mods/bps.mjs make them: "BPS1", the source's size, the target's and
 * the metadata's, the metadata, then actions until the last 12 bytes, which are the CRCs of the source, the target and the patch. An
 * action is a number whose low 2 bits say what and the rest how long, less one: read the source where the target is (0), read bytes from
 * the patch (1), copy from the source (2) or from what's made so far (3) from an offset moved by a signed number. A number is 7 bits a
 * byte, the last with its top bit set, each byte but the first counting one more than its bits say. Every count and offset is checked
 * before it's used, as a patch can come from anyone. */
#include <stdlib.h>
#include <string.h>

#include <zlib.h>
#include "bps.h"

static uint32_t get32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

static int number(const uint8_t **at, const uint8_t *end, uint64_t *out) {
    uint64_t data = 0, shift = 1;

    while (*at < end) {
        uint8_t x = *(*at)++;

        data += (x & 0x7F) * shift;
        if (x & 0x80) {
            *out = data;
            return 1;
        }
        if (shift > (uint64_t)1 << 49) {
            return 0;
        }
        shift <<= 7;
        data += shift;
    }
    return 0;
}

uint8_t *bps_apply(const uint8_t *patch, size_t patch_size, const uint8_t *source, size_t source_size, size_t limit, size_t *size,
                   const char **problem) {
    const uint8_t *at = patch + 4, *end;
    uint64_t source_bytes, target_bytes, meta, made = 0;
    int64_t from_source = 0, from_target = 0;
    uint8_t *target;

    *size = 0;
    *problem = "NOT A PATCH IT CAN READ";
    if (patch_size < 4 + 3 + 12 || memcmp(patch, "BPS1", 4) != 0) {
        return NULL;
    }
    *problem = "DAMAGED";
    end = patch + patch_size - 12;
    if ((uint32_t)crc32(0L, patch, (uInt)(patch_size - 4)) != get32(patch + patch_size - 4) || !number(&at, end, &source_bytes) ||
        !number(&at, end, &target_bytes) || !number(&at, end, &meta) || meta > (uint64_t)(end - at)) {
        return NULL;
    }
    at += meta;
    if (source_bytes != source_size || (uint32_t)crc32(0L, source, (uInt)source_size) != get32(end)) {
        *problem = "FOR ANOTHER FILE";
        return NULL;
    }
    if (target_bytes > limit) {
        *problem = "TOO BIG";
        return NULL;
    }
    target = malloc((size_t)target_bytes + 1);
    if (target == NULL) {
        *problem = "TOO BIG";
        return NULL;
    }
    while (at < end) {
        uint64_t data, length, move;
        int64_t step;

        if (!number(&at, end, &data) || (length = (data >> 2) + 1) > target_bytes - made) {
            goto fail;
        }
        switch (data & 3) {
        case 0: /* the source's bytes where the target's are */
            if (made + length > source_size) {
                goto fail;
            }
            memcpy(target + made, source + made, (size_t)length);
            break;
        case 1: /* the patch's own */
            if (length > (uint64_t)(end - at)) {
                goto fail;
            }
            memcpy(target + made, at, (size_t)length);
            at += length;
            break;
        default: /* from somewhere in the source, or in what's made so far, which may run on into what it's making */
            if (!number(&at, end, &move) || (move >> 1) > (uint64_t)INT64_MAX / 2) {
                goto fail;
            }
            step = (int64_t)(move >> 1);
            step = move & 1 ? -step : step;
            if ((data & 3) == 2) {
                from_source += step;
                if (from_source < 0 || (uint64_t)from_source + length > source_size) {
                    goto fail;
                }
                memcpy(target + made, source + from_source, (size_t)length);
                from_source += (int64_t)length;
            } else {
                uint64_t i;

                from_target += step;
                if (from_target < 0 || (uint64_t)from_target >= made) {
                    goto fail;
                }
                for (i = 0; i < length; i++) {
                    target[made + i] = target[from_target++];
                }
            }
            break;
        }
        made += length;
    }
    if (made != target_bytes || (uint32_t)crc32(0L, target, (uInt)target_bytes) != get32(end + 4)) {
        goto fail;
    }
    *size = (size_t)target_bytes;
    *problem = NULL;
    return target;

fail:
    free(target);
    return NULL;
}
