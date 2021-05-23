#include "sha.h"

int main(int argc, char* argv[]){
	if(strcmp(argv[1], "-r") == 0)
		get_hash_from_random();
	else
		get_hash(argv[1]);
	return 0;
}