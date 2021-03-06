/*
ProbaGenerator.c
Demo program creating sample file with controlled probabilies
Copyright (C) Yann Collet 2012-2013
GPL v2 License

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


//******************************
// Compiler options
//******************************
#define _CRT_SECURE_NO_WARNINGS   // Visual warning


//******************************
// Include
//******************************
#include <stdlib.h>   // malloc
#include <stdio.h>    // printf
#include <string.h>   // memset
#include <math.h>     // log


//******************************
// Constants
//******************************
#define MB *(1<<20)
#define BUFFERSIZE 1 MB
#define PROBATABLESIZE 4096


//**************************************
// Macros
//**************************************
#define DISPLAY(...)         fprintf(stderr, __VA_ARGS__)
#define DISPLAYLEVEL(l, ...) if (displayLevel>=l) { DISPLAY(__VA_ARGS__); }


//***************************************************
// Local variables
//***************************************************
static char* programName;
static int   displayLevel = 2;   // 0 : no display  // 1: errors  // 2 : + result + interaction + warnings ;  // 3 : + progression;  // 4 : + information


//******************************
// local functions
//******************************
static int usage()
{
    DISPLAY("Usage :\n");
    DISPLAY("%s P%%\n", programName);
    DISPLAY("Exemple :\n");
    DISPLAY("%s 70%%\n", programName);
    return 0;
}

static int badusage()
{
    DISPLAYLEVEL(1, "Incorrect parameters\n");
    if (displayLevel >= 1) usage();
    DISPLAY("Press enter to exit \n");
    getchar();
    exit(1);
}

static void generate(void* buffer, size_t buffSize, double p)
{
    char table[PROBATABLESIZE];
    int remaining = PROBATABLESIZE;
    int pos = 0;
    int s = 0;
    char* op = (char*) buffer;
    char* oend = op + buffSize;

    DISPLAY("\nGenerating %i MB with P=%.2f%%\n", (int)(buffSize >> 20), p*100);

    // Build Table
    while (remaining)
    {
        int n = (int)(remaining * p);
        int end;
        if (!n) n=1;
        end = pos + n;
        while (pos<end) table[pos++]=(char)s;
        s++;
        remaining -= n;
    }

    // Fill buffer
    while (op<oend)
    {
        const int r = rand() & (PROBATABLESIZE-1);
        *op++ = table[r];
    }
}


void createSampleFile(char* filename, double p)
{
    FILE* foutput = fopen( filename, "wb" );
    void* buffer = malloc(BUFFERSIZE);
    generate(buffer, BUFFERSIZE, p);
    fwrite(buffer, 1, BUFFERSIZE, foutput);
    fclose(foutput);
    DISPLAY("File %s generated\n", filename);
}


int main(int argc, char** argv) 
{
    char* n;
    double proba = 0.;
    char  filename[] = "proba.bin";

    programName = argv[0];
    printf("Binary file generator\n");
    if (argc<2) badusage();

    n = argv[1];
    if ((*n>='0') && (*n<='9')) { proba += *n-'0'; n++; }
    if ((*n>='0') && (*n<='9')) { proba*=10; proba += *n-'0'; n++; }
    proba /= 100;

    createSampleFile(filename, proba);

    DISPLAY("Press enter to exit \n");
    getchar();
    return 0;
}