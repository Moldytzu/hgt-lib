#include <stdio.h>
#include "hgtlib.h"

int main()
{
    hgt_file_t *hgt = hgtCreateContext("N48E029.hgt"); // open the hgt

    // iterate over 1 degree of latitude
    // NOTE: the HGT covers only 1 DEGREE in each direction
    for (double i = 0; i < 1; i += 0.01)
        printf("%d ", hgtReadElevation(hgt, 0, i));

    hgtDeleteContext(hgt); // clean up

    return 0;
}