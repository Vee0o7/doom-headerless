#include "include.h"
#include "doomgeneric.h"

#include "w_file.h"
#include "z_zone.h"

typedef struct
{
    wad_file_t wad;
} stdc_wad_file_t;

extern wad_file_class_t stdc_wad_file;

static wad_file_t *W_StdC_OpenFile(char *path)
{
    stdc_wad_file_t *result;

    // Create a new stdc_wad_file_t to hold the file handle.

    result = Z_Malloc(sizeof(stdc_wad_file_t), PU_STATIC, 0);
    result->wad.file_class = &stdc_wad_file;
    result->wad.mapped = NULL;
    result->wad.length = 0;

    return &result->wad;
}

extern void DH_read_wad(unsigned int offset, void* buffer, size_t buffer_len);

// Read data from the specified position in the file into the 
// provided buffer.  Returns the number of bytes read.

size_t W_StdC_Read(wad_file_t *wad, unsigned int offset,
                   void *buffer, size_t buffer_len)
{
    stdc_wad_file_t *stdc_wad;
    size_t result;

    stdc_wad = (stdc_wad_file_t *) wad;

		DH_read_wad(offset, buffer, buffer_len);

    return buffer_len;
}

wad_file_class_t stdc_wad_file = 
{
    W_StdC_OpenFile,
    W_StdC_Read,
};
