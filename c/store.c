#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define BUFFER_SIZE 1024

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
	closeInputStream(state);
	closeStoreStream(state);
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
	state->fpStore = fopen(storeFileName, "ab+");
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
	unsigned char buffer[256];
	memset(buffer, 0, 256);
	fwrite(&buffer, sizeof(buffer[0]), 256, state->fpStore); 
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
	} else if(256 > state.storeSize) {
		fprintf(stderr, "Invalid store size: %ld\n", state.storeSize);
		closeState(&state);
		exit(1);
	}

	unsigned char buffer[BUFFER_SIZE];
	
	while(!feof(state.fpInput)) {
		size_t result = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, state.fpInput);
		for(int i = 0; i < result; i++) {
			unsigned char sample = buffer[i];
			// fwrite(&factor,sizeof(factor),1,stdout);
			fprintf(stderr, "%d\n", sample);
		}
	}

	closeState(&state);
}

