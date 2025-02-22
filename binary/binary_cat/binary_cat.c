#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define CHUNK_SIZE 16 

int main(int argc, char *argv[]) {
	//arg check
	if(argc < 2) {
		printf("Error : No file provided.\n");
	}	

	//open file
	FILE *file = fopen(argv[1],"rb");
	if(file == NULL) {
		printf("Error : Opening file failed.\n");
		return 1;
	}

	//get and check file size
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	if(ftell(file) > LONG_MAX) {
		printf("Error : Too large file size.");
		fclose(file);
		return 1;
	}
	rewind(file);
	
	//define buffer
	unsigned char *buf = (unsigned char *)malloc(CHUNK_SIZE);
	if(buf == NULL) {
		printf("Error : Memory allocation failed.\n");
		fclose(file);
		return 1;
	}
	size_t bytesRead;
	size_t totalBytesRead = 0;

	//read file
	long addr = 0;
	while((bytesRead = fread(buf, 1, CHUNK_SIZE, file)) > 0) {
		printf("ADDRESS:%08lx ", addr);
		for (size_t i = 0; i < bytesRead; i++) {
			printf("%02x ", buf[i]);
		}
		printf("\n");
		totalBytesRead += bytesRead;
		addr += bytesRead;
	}

	if(totalBytesRead != fileSize) {
		printf("Error : Reading file faild.");
		free(buf);
		fclose(file);
		return 1;
	} else {
		printf("File read successfully, %zu bytes processed.\n", totalBytesRead);
	}
	
	free(buf);
	fclose(file);
	
	return 0;
}
