#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define BUFFER_SIZE 1024

int main( int argc, const char* argv[] )
{
	if(argc < 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 0;
	}
	const char *fileName = argv[1];
	FILE *fp = fopen(fileName, "rb");
	if(NULL == fp) {
		printf("Error(%d): %s %s\n", errno, strerror(errno), fileName);
		exit(errno);
	}

	bool firstRead = true;
	unsigned char last = 127;
	unsigned char buffer[BUFFER_SIZE];
	
	while(!feof(fp)) {
		size_t result = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, fp);
		for(int i = 0; i < result; i++) {
			unsigned char sample = buffer[i];
			unsigned char factor = (unsigned char)((((float)sample / (float)last) + 0.005f) * 100.0f);
			fwrite(&factor,sizeof(factor),1,stdout);
			last = sample;
		}
	}

	fclose(fp);
	

}
