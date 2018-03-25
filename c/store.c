#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define VALUE_SPACE 256
#define VALUE_SPACE_SIZE VALUE_SPACE * sizeof(address_t)

/*
store unique chains in separate files

for each sample open samples file and get backref address, go to that back ref address and see if it matches,
if not move onto next chain,
retain largest matched chain

add file pool so that we can configure max number of simultaneous open files.
*/

typedef unsigned char sample_t;
typedef unsigned long address_t;

typedef struct state_t {
	int argc;
	const char **argv;
	FILE *fpStore;
	long storeSize;
	FILE *fpInput;
} state_t;

void closeInputStream(state_t *state) {
	if(NULL == state->fpInput) {
		return;
	}

	fclose(state->fpInput);
	state->fpInput = NULL;
}

void closeStoreStream(state_t *state) {
	if(NULL == state->fpStore) {
		return;
	}

	fclose(state->fpStore);
	state->fpStore = NULL;
}

void closeState(state_t *state) {
	fprintf(stderr, "Closing state\n");
	closeInputStream(state);
	closeStoreStream(state);
	fprintf(stderr, "Closed state\n");
}

void openInputStream(state_t *state) {
	state->fpInput = stdin;
	if(state->argc > 2) {
		const char *inputFileName = state->argv[2]; 
		state->fpInput = fopen(inputFileName, "rb");
		if(NULL == state->fpInput) {
			fprintf(stderr, "Error(%d): %s %s\n", errno, strerror(errno), inputFileName);
			closeState(state);
			exit(errno);
		}
		fprintf(stderr, "Reading from %s\n", inputFileName);
	} else {
		fprintf(stderr, "Reading from stdin\n");
	}
}

void openStoreStream(state_t *state) {
	const char *storeFileName = state->argv[1];
	state->fpStore = fopen(storeFileName, "wb+");
	if(NULL == state->fpStore) {
		printf("Error(%d): %s %s\n", errno, strerror(errno), storeFileName);
		closeState(state);
		exit(errno);
	}
	state->storeSize = ftell(state->fpStore);
	fprintf(stderr, "Store size: %ld\n", state->storeSize);
}

void initState(state_t *state, int argc, const char* argv[]) {
	state->argc = argc;
	state->argv = argv;
	state->fpStore = NULL;
	state->storeSize = 0;
	state->fpInput = NULL;

	openStoreStream(state);
	openInputStream(state);
}

void initStore(state_t *state) {
	address_t buffer[VALUE_SPACE];
	memset(buffer, 0, VALUE_SPACE_SIZE);
	fwrite(&buffer, sizeof(address_t), VALUE_SPACE, state->fpStore); 
	fflush(state->fpStore);
}

long getAddress(state_t *state, sample_t sample) {
	fseek(state->fpStore, sample*sizeof(address_t), SEEK_SET);
	address_t addr = ftell(state->fpStore);

	address_t sampleAddress = 0;
	fread(&sampleAddress, sizeof(address_t), 1, state->fpStore);
	fprintf(stderr, "Read at %ld of address %ld\n", addr, sampleAddress);
	return sampleAddress;
}

void appendSample(state_t *state, sample_t sample) {
	fseek(state->fpStore, 0, SEEK_END);
	address_t sampleAddress = ftell(state->fpStore);
	fprintf(stderr, "FFW to the end of the store at %ld\n", sampleAddress);

	address_t sampleValue = sample;
	address_t sampleValueAddress = sampleValue * sizeof(address_t);

	fwrite(&sampleValue, sizeof(address_t), 1, state->fpStore);
	fflush(state->fpStore);
	fprintf(stderr, "Wrote sample value %ld at address %ld\n", sampleValue, sampleAddress);

	fseek(state->fpStore, sampleValueAddress, SEEK_SET);
	fprintf(stderr, "Rewinding to sample value address at %ld\n", sampleValueAddress);

	fwrite(&sampleAddress, sizeof(address_t), 1, state->fpStore);
	fflush(state->fpStore);
	fprintf(stderr, "Wrote sample %ld at address %ld\n", sampleValue, sampleValueAddress);
}

int main(int argc, const char* argv[]) {
	if(argc < 2) {
		fprintf(stderr, "Usage: %s <store> [input]\n", argv[0]);
		fprintf(stderr, "Usage: cat <input> | %s <store>\n", argv[0]);
		fprintf(stderr, "If input is not specified will read from stdin\n");
		return EINVAL;
	}

	state_t state;

	initState(&state, argc, argv);
	if(0 == state.storeSize) {
		initStore(&state);
	} else if(VALUE_SPACE_SIZE > state.storeSize) {
		fprintf(stderr, "Invalid store size: %ld\n", state.storeSize);
		closeState(&state);
		exit(1);
	}

	sample_t buffer[BUFFER_SIZE];
	
	while(!feof(state.fpInput)) {
		size_t result = fread(buffer, sizeof(sample_t), BUFFER_SIZE, state.fpInput);
		for(int i = 0; i < result; i++) {
			sample_t sample = buffer[i];

			address_t sampleAddress = getAddress(&state, sample);
			if(sampleAddress < VALUE_SPACE_SIZE) {
				fprintf(stderr, "Sample %d is in value space\n", sample);
				appendSample(&state, sample);
			} else {
				fprintf(stderr, "Sample %d is in ref space\n", sample);
			}
			//long refAddr = refAddr(&state, sample);
			fprintf(stderr, "Sample: %d refAddr: %ld \n", sample, sampleAddress);
			// fwrite(&factor,sizeof(factor),1,stdout);
		}
	}

	closeState(&state);
}

