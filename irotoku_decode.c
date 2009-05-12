/*
The irotoku steganographic decoder. Created by JP Hastings-Spital, kedakai.
More details can be found at http://projects.kedakai.co.uk/irotoku

Distributed under the Creative Commons Attribution-Non-Commercial-Share Alike 2.0 license
Details can be found here: http://creativecommons.org/licenses/by-nc-sa/2.0/uk/
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gd.h"

int bits_per_colour, dont_stop_at_0;

void addToBuffer(int *buffer,int *bufferindex,int bits);

int main(int argc, char **argv) {
	gdImagePtr im;
	FILE *in;
	int x,y,rgb, opt, modulo;
	char *filename;
	
	dont_stop_at_0 = 0;
	bits_per_colour = 1;
	modulo = 2;
	
	while ((opt = getopt(argc, argv, "h0b:")) != -1) {
		switch (opt) {
			case 'h':
				fprintf(stdout,"For more info visit projects.kedakai.co.uk/irotoku\nUsage: %s [options] file.png ...\nWhere the options are one of:\n\t-0     Don't stop decoding at the first \\0 ASCII character\n\t-b N   Assume there are N bits per colour value. [1],2,4\nN.B. Output is sent to stdout.\n",argv[0]);
				exit(0);
			case 'b':
				switch ((int)optarg[0]) {
					case '4':
						bits_per_colour = 4;
						modulo = 16;
						break;
					case '2':
						bits_per_colour = 2;
						modulo = 4;
						break;
				}
				break;
			case '0':
				dont_stop_at_0 = 1;
				break;
		}
	}
	if (argv[optind] == NULL) { fprintf(stderr,"You haven't specified an input file.\nTry %s -h for help.\n",argv[0]); exit(-1); }
	filename = argv[optind];
	
	int buffer = 0, bufferindex = 8;
	
	in =  fopen(filename, "r");
	if (in == NULL) { fprintf(stderr,"The file '%s' doesn't exist\n",filename); exit(-1); }
	im = gdImageCreateFromPng(in);
	if (im == NULL) { fprintf(stderr,"The file '%s' is not a valid PNG image\n",filename); exit(-1); }
	fclose(in);
	
	for (x=0;x<gdImageSX(im);++x) {
		for (y=0;y<gdImageSY(im);++y) {
			rgb = gdImageGetPixel(im,x,y);
			addToBuffer(&buffer,&bufferindex,gdImageRed(im,rgb) % modulo);
			addToBuffer(&buffer,&bufferindex,gdImageGreen(im,rgb) % modulo);
			addToBuffer(&buffer,&bufferindex,gdImageBlue(im,rgb) % modulo);
		}
	}
	
	gdImageDestroy(im);
}

void addToBuffer(int *buffer,int *bufferindex,int bits) {
	extern dont_stop_at_0,bits_per_colour;
	
	*bufferindex -= bits_per_colour;
	*buffer += bits << *bufferindex;
	if (*bufferindex == 0) {
		if (dont_stop_at_0 != 1 && *buffer == 0)
			exit(0);
		fprintf(stdout,"%c",*buffer);
		*buffer = 0;
		*bufferindex = 8;
	}
}
