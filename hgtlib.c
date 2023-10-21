#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    const char *filename;
    FILE *fd;
    uint16_t *buffer;
    uint32_t bufferSize;
} hgt_file_t;

static const uint32_t lines = 1201, samples = 1201; // SRTM3

hgt_file_t *hgtCreateContext(const char *filename)
{
    hgt_file_t *hgt = (hgt_file_t *)malloc(sizeof(hgt_file_t));

    if (!(hgt->fd = fopen(filename, "rb"))) // open file as binary
    {
        printf("Failed to load file %s\n", filename);
        return NULL;
    }

    fseek(hgt->fd, 0, SEEK_END);      // seek at the end
    hgt->bufferSize = ftell(hgt->fd); // read the file pointer
    fseek(hgt->fd, 0, SEEK_SET);      // seek at the start

    hgt->buffer = malloc(hgt->bufferSize);           // allocate a buffer to hold the buffer
    fread(hgt->buffer, hgt->bufferSize, 1, hgt->fd); // read the whole file

    return hgt;
}

void hgtDeleteContext(hgt_file_t *hgt)
{
    // close the file
    fclose(hgt->fd);

    // delete the used pointers
    free(hgt->buffer);
    free(hgt);
}

int16_t hgtReadElevationRaw(hgt_file_t *hgt, size_t offset)
{
    uint16_t be, le;

    be = hgt->buffer[offset];            // read the elevation value at the offset
    le = ((be & 0xFF) << 8) | (be >> 8); // swap endianness (x86_64 is little-endian)
                                         // fixme: don't assume this is x86_64!

    return (int16_t)le;
}

// fixme: move this in a test.c file
int main()
{
    hgt_file_t *hgt = hgtCreateContext("N48E029.hgt"); // fixme: don't hard-code

    for (int i = 0; i < 1201; i++)
        printf("%d ", hgtReadElevationRaw(hgt, i));

    return 0;
}