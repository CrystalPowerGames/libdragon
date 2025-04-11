#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include "binout.h"

#define STBDS_NO_SHORT_NAMES
#define STB_DS_IMPLEMENTATION //Hack to get tools to compile
#include "stb_ds.h"

struct placeholder_data {
<<<<<<< HEAD
	int64_t offset;
	uint64_t *pending_offsets_64;
	uint32_t *pending_offsets_32;
	uint16_t *pending_offsets_16;
	uint8_t *pending_offsets_8;
=======
	int offset;
	int *pending_offsets;
>>>>>>> 24926336e643b93c6390d7ec57b62e1f5044e9cf
};

struct {
	char *key;
	struct placeholder_data value;
} *placeholder_hash = NULL;

void _w8(FILE *f, uint8_t v) 
{
    fputc(v, f);
}

void _w16(FILE *f, uint16_t v)
{
    w8(f, v >> 8);
    w8(f, v & 0xff);
}

void _w32(FILE *f, uint32_t v)
{
    w16(f, v >> 16);
    w16(f, v & 0xffff);
}

void _w64(FILE *f, uint64_t v)
{
    w32(f, v >> 32);
    w32(f, v & 0xffffffff);
}

int w32_placeholder(FILE *f)
{
    int pos = ftell(f);
    w32(f, 0);
    return pos;
}

void _w64_at(FILE *f, int pos, uint64_t v)
{
	int cur = ftell(f);
	assert(cur >= 0);  // fail on pipes
	fseek(f, pos, SEEK_SET);
	w64(f, v);
	fseek(f, cur, SEEK_SET);
}

void _w32_at(FILE *f, int pos, uint32_t v)
{
    int cur = ftell(f);
    assert(cur >= 0);  // fail on pipes
    fseek(f, pos, SEEK_SET);
    w32(f, v);
    fseek(f, cur, SEEK_SET);
}

void _w16_at(FILE *f, int pos, uint16_t v)
{
	int cur = ftell(f);
	assert(cur >= 0);  // fail on pipes
	fseek(f, pos, SEEK_SET);
	w16(f, v);
	fseek(f, cur, SEEK_SET);
}

void _w8_at(FILE *f, int pos, uint8_t v)
{
	int cur = ftell(f);
	assert(cur >= 0);  // fail on pipes
	fseek(f, pos, SEEK_SET);
	w8(f, v);
	fseek(f, cur, SEEK_SET);
}

void walign(FILE *f, int align)
{ 
    int pos = ftell(f);
    assert(pos >= 0);  // fail on pipes
    while (pos++ % align) w8(f, 0);
}

void wpad(FILE *f, int size)
{
    while (size--) {
        w8(f, 0);
    }
}

struct placeholder_data *__placeholder_get_data(const char *name)
{
<<<<<<< HEAD
	if(placeholder_hash == NULL) {
		stbds_sh_new_arena(placeholder_hash);
	}
=======
>>>>>>> 24926336e643b93c6390d7ec57b62e1f5044e9cf
	ptrdiff_t index = stbds_shgeti(placeholder_hash, name);
	if(index == -1) {
		struct placeholder_data default_value = {-1, NULL};
		index = stbds_shlen(placeholder_hash);
		stbds_shput(placeholder_hash, name, default_value);
	}
	return &placeholder_hash[index].value;
}

<<<<<<< HEAD
void __placeholder_make(FILE *file, int64_t offset, const char *name)
{
	struct placeholder_data *data = __placeholder_get_data(name);
	data->offset = offset;
	for (int i=0; i<stbds_arrlen(data->pending_offsets_64); i++) {
		w32_at(file, data->pending_offsets_64[i], data->offset);
	}
	for(int i=0; i<stbds_arrlen(data->pending_offsets_32); i++) {
		w32_at(file, data->pending_offsets_32[i], data->offset);
	}
	for(int i=0; i<stbds_arrlen(data->pending_offsets_16); i++) {
		w16_at(file, data->pending_offsets_16[i], data->offset);
	}
	for(int i=0; i<stbds_arrlen(data->pending_offsets_8); i++) {
		w8_at(file, data->pending_offsets_8[i], data->offset);
	}
	stbds_arrfree(data->pending_offsets_64);
	stbds_arrfree(data->pending_offsets_32);
	stbds_arrfree(data->pending_offsets_16);
	stbds_arrfree(data->pending_offsets_8);
=======
void __placeholder_make(FILE *file, int offset, const char *name)
{
	if(placeholder_hash == NULL) {
		stbds_sh_new_arena(placeholder_hash);
	}
	struct placeholder_data *data = __placeholder_get_data(name);
	data->offset = offset;
	for(size_t i=0; i<stbds_arrlenu(data->pending_offsets); i++) {
		w32_at(file, data->pending_offsets[i], data->offset);
	}
	stbds_arrfree(data->pending_offsets);
>>>>>>> 24926336e643b93c6390d7ec57b62e1f5044e9cf
}

void placeholder_setv(FILE *file, const char *format, va_list arg)
{
	char *name = NULL;
	vasprintf(&name, format, arg);
	__placeholder_make(file, ftell(file), name);
	free(name);
}

void placeholder_set(FILE *file, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	placeholder_setv(file, format, args);
	va_end(args);
}

<<<<<<< HEAD
void placeholder_setv_offset(FILE *file, int64_t offset, const char *format, va_list arg)
=======
void placeholder_setv_offset(FILE *file, int offset, const char *format, va_list arg)
>>>>>>> 24926336e643b93c6390d7ec57b62e1f5044e9cf
{
	char *name = NULL;
	vasprintf(&name, format, arg);
	__placeholder_make(file, offset, name);
	free(name);
}

<<<<<<< HEAD
void placeholder_set_offset(FILE *file, int64_t offset, const char *format, ...)
=======
void placeholder_set_offset(FILE *file, int offset, const char *format, ...)
>>>>>>> 24926336e643b93c6390d7ec57b62e1f5044e9cf
{
	va_list args;
	va_start(args, format);
	placeholder_setv_offset(file, offset, format, args);
	va_end(args);
}

<<<<<<< HEAD
void __w64_placeholder_named(FILE *file, const char *name)
{
	struct placeholder_data *data = __placeholder_get_data(name);
	if(data->offset == -1) {
		stbds_arrpush(data->pending_offsets_64, ftell(file));
		w64(file, 0);
	} else {
		w64(file, data->offset);
	}
}

void __w32_placeholder_named(FILE *file, const char *name)
{
	struct placeholder_data *data = __placeholder_get_data(name);
	if(data->offset == -1) {
		stbds_arrpush(data->pending_offsets_32, ftell(file));
=======

void __w32_placeholder_named(FILE *file, const char *name)
{
	if(placeholder_hash == NULL) {
		stbds_sh_new_arena(placeholder_hash);
	}
	struct placeholder_data *data = __placeholder_get_data(name);
	if(data->offset == -1) {
		stbds_arrpush(data->pending_offsets, ftell(file));
>>>>>>> 24926336e643b93c6390d7ec57b62e1f5044e9cf
		w32(file, 0);
	} else {
		w32(file, data->offset);
	}
}

<<<<<<< HEAD
void __w16_placeholder_named(FILE *file, const char *name)
{
	struct placeholder_data *data = __placeholder_get_data(name);
	if(data->offset == -1) {
		stbds_arrpush(data->pending_offsets_16, ftell(file));
		w16(file, 0);
	} else {
		w16(file, data->offset);
	}
}

void __w8_placeholder_named(FILE *file, const char *name)
{
	struct placeholder_data *data = __placeholder_get_data(name);
	if(data->offset == -1) {
		stbds_arrpush(data->pending_offsets_8, ftell(file));
		w8(file, 0);
	} else {
		w8(file, data->offset);
	}
}

void w64_placeholdervf(FILE *file, const char *format, va_list arg)
{
	char *name = NULL;
	vasprintf(&name, format, arg);
	__w64_placeholder_named(file, name);
	free(name);
}

void w64_placeholderf(FILE *file, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	w64_placeholdervf(file, format, args);
	va_end(args);
}

=======
>>>>>>> 24926336e643b93c6390d7ec57b62e1f5044e9cf
void w32_placeholdervf(FILE *file, const char *format, va_list arg)
{
	char *name = NULL;
	vasprintf(&name, format, arg);
	__w32_placeholder_named(file, name);
	free(name);
}

void w32_placeholderf(FILE *file, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	w32_placeholdervf(file, format, args);
	va_end(args);
}

<<<<<<< HEAD
void w16_placeholdervf(FILE *file, const char *format, va_list arg)
{
	char *name = NULL;
	vasprintf(&name, format, arg);
	__w16_placeholder_named(file, name);
	free(name);
}

void w16_placeholderf(FILE *file, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	w16_placeholdervf(file, format, args);
	va_end(args);
}

void w8_placeholdervf(FILE *file, const char *format, va_list arg)
{
	char *name = NULL;
	vasprintf(&name, format, arg);
	__w8_placeholder_named(file, name);
	free(name);
}

void w8_placeholderf(FILE *file, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	w8_placeholdervf(file, format, args);
	va_end(args);
}

void placeholder_clear()
{
	for(int i=0; i<stbds_shlen(placeholder_hash); i++) {
		stbds_arrfree(placeholder_hash[i].value.pending_offsets_64);
		stbds_arrfree(placeholder_hash[i].value.pending_offsets_32);
		stbds_arrfree(placeholder_hash[i].value.pending_offsets_16);
		stbds_arrfree(placeholder_hash[i].value.pending_offsets_8);
=======
void placeholder_clear()
{
	for(size_t i=0; i<stbds_shlenu(placeholder_hash); i++) {
		stbds_arrfree(placeholder_hash[i].value.pending_offsets);
>>>>>>> 24926336e643b93c6390d7ec57b62e1f5044e9cf
	}
	stbds_shfree(placeholder_hash);
}
