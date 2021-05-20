#include "sha.h"

// ALL LENGTHS IN BYTES
#define RATE_LEN 136		
#define CAPACITY_LEN 64
#define OUTPUT_LEN 32
#define STATE_SIZE 200

uint8_t state[STATE_SIZE];
uint8_t input[16000] = {0};

uint8_t get_bit_pos(int pos, uint8_t* arr){
	int arr_val = arr[pos / 8];
	int bit_pos = pos % 8;
	return (uint8_t)(((arr_val << bit_pos) & 0x80) >> 7);
}

/*
	Function to get the state cube coordinates
	@param (int x) x-coordinates
	@param (int y) y-coordinates
	@param (int z) z-coordinates
	@return(uint8_t) the value in the state array.
*/
uint8_t get_cube_pt(int x, int y, int z){
	return 0;
}

/*
	Function to perform the initial XOR to rate section
	@param (uint8_t* current_block) current block of the plaintext
	@param (uint8_t* state) current state array
*/
void xor_rate(uint8_t* current_block, uint8_t* state){
	for(int i = 0; i < RATE_LEN; i++){
		state[i] ^= current_block[i];
	}
}

/*
	Function to perform the f-scramble

*/

void get_hash(char* ptext){
	// copy the string
	strcpy(input, ptext);

	// set padded length;
	int str_length = strlen(ptext);
	input[str_length - 1] = 0;				// remove the \0
	int pad_length =  str_length + (RATE_LEN -  (str_length % RATE_LEN));

	printf("%d", get_bit_pos(2, input));

}