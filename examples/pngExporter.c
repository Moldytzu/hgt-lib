// adapted from https://github.com/cnlr-web-dev/igeomap/blob/main/datasetRenderer/pngExporter.c

#include <stdio.h>
#include <stdlib.h>
#include <hgt-lib/hgtlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// from arduino libc
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main()
{
    hgt_file_t *hgt = hgtCreateContext("N48E029.hgt");
    size_t hgtArea = hgt->sideLength * hgt->sideLength; // the area in points covered by the HGT

    // determine altitude minimum, maximum and average in meters
    int16_t max = HGT_DATA_VOID, min = INT16_MAX;
    uint64_t avg = 0;
    for (size_t i = 0; i < hgtArea; i++)
    {
        int16_t elevation = hgtReadElevationRaw(hgt, i);
        if (elevation > max)
            max = elevation;
        if (elevation < min)
            min = elevation;

        avg += abs(elevation);
    }
    avg /= hgtArea; // do the arithmetic mean
    printf("%s: min: %dm max: %dm avg: %ldm\n", hgt->filename, min, max, avg);

    // prepare image data
    uint8_t *buffer = malloc(hgtArea); // this holds the 8 bit grayscale data
    for (size_t i = 0; i < hgtArea; i++)
    {
        int16_t elevation = hgtReadElevationRaw(hgt, i);
        buffer[i] = (uint8_t)map(elevation, min, max, 0, 255);
    }

    // write using stbi_write
    stbi_write_png_compression_level = 0;                                                                       // disable compression
    stbi_write_png("test.png", hgt->sideLength, hgt->sideLength, 1, buffer, hgt->sideLength * sizeof(uint8_t)); // generate .png image

    // deallocate
    hgtDeleteContext(hgt);
    free(buffer);

    return 0;
}