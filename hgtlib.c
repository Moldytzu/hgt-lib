#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const char *file = "N48E029.hgt"; // fixme: don't hard-code
FILE *fd;

uint16_t readShort(size_t offset)
{
    uint16_t be, le;
    fseek(fd, offset * sizeof(uint16_t), SEEK_SET);
    fread(&be, sizeof(uint16_t), 1, fd); // read a short in our requested variable

    le = ((be & 0xFF) << 8) | (be >> 8); // swap endianness (x86_64 is little-endian)
                                         // fixme: don't assume this is x86_64!

    return le;
}

int main()
{
    fd = fopen(file, "rb"); // open file as binary

    if (!fd)
    {
        printf("Failed to load file %s\n", file);
        return 0;
    }

    for (int i = 0; i < 16; i++)
        printf("%x ", readShort(i));

    fclose(fd); // close our file

    return 0;
}