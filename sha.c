#include "sha.h"

// ALL LENGTHS IN BYTES
#define RATE_LEN 136
#define CAPACITY_LEN 64
#define OUTPUT_LEN 32
#define STATE_SIZE 200
#define DEPTH 64
#define BLANK 0

uint8_t state[STATE_SIZE] = {0};
uint8_t input[16000] = {0};

// RC table for iota function
static const uint8_t RC_TABLE[24][8] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}, 
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x82}, 
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x8a},
        {0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00}, 
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x8b}, 
        {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01},
        {0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x81}, 
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x09}, 
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8a},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88}, 
        {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x09}, 
        {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0a},
        {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x8b}, 
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8b}, 
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x89},
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03}, 
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02}, 
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0a}, 
        {0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0a}, 
        {0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x81},
        {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80}, 
        {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01}, 
        {0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x08}
    };

uint8_t get_bit_pos(int pos, uint8_t *arr)
{
	int arr_val = arr[pos / 8];
	int bit_pos = pos % 8;
	return (uint8_t)(((arr_val << bit_pos) & 0x80) >> 7);
}

void set_bit_pos(int val, int pos, uint8_t *arr)
{
	int arr_val = arr[pos / 8];
	int bit_pos = pos % 8;
	if (val == 1)
		arr[pos / 8] |= 1 << (7 - bit_pos);
	else
		arr[pos / 8] &= ~(1 << (7 - bit_pos));
}

void reverse_endian(uint8_t *lane)
{
	for (int i = 0; i < 4; i++)
	{
		uint8_t temp = lane[i];
		lane[i] = lane[7 - i];
		lane[7 - i] = temp;
	}
}

/*
	Function to get the state cube coordinates
	@param (int x) x-coordinates
	@param (int y) y-coordinates
	@param (int z) z-coordinates
	@return(uint8_t) the value in the state array.
*/
uint8_t get_cube_pt(int x, int y, int z,  uint8_t* arr)
{
	// z = DEPTH - z;
	static int w = (STATE_SIZE * 8) / 25;
	int arr_index = (w * ((5 * y) + x)) + z;

	return get_bit_pos(arr_index, arr);
}

void set_cube_pt(int val, int x, int y, int z, uint8_t* arr)
{
	// z = DEPTH - z;
	static int w = (STATE_SIZE * 8) / 25;
	int arr_index = (w * ((5 * y) + x)) + z;

	set_bit_pos(val, arr_index, arr);
}

/*
	Function to perform the initial XOR to rate section
	@param (uint8_t* current_block) current block of the plaintext
	@param (uint8_t* state) current state array
*/
void xor_rate(uint8_t *current_block)
{
	for (int i = 0; i < RATE_LEN; i++)
	{
		state[i] ^= current_block[i];
	}
}

/*
	Function to perform the f-scramble

*/
void theta()
{

	int C[5][DEPTH];
	int D[5][DEPTH];

	// for(int x = 0; x < 5; x++)
	// 	for (int z = 0; z < DEPTH; z++){

	// 	}

	for (int x = 0; x < 5; x++)
		for (int z = 0; z < DEPTH; z++)
		{
			C[x][z] = BLANK;
			D[x][z] = BLANK;
		}

	// populate C
	for (int x = 0; x < 5; x++)
		for (int z = 0; z < DEPTH; z++)
		{
			C[x][z] = get_cube_pt(x, 0, z, state);
			for (int y = 1; y < 5; y++)
				C[x][z] ^= get_cube_pt(x, y, z, state);
		}

	for (int x = 0; x < 5; x++)
		for (int z = 0; z < DEPTH; z++)
		{
			printf("%01x", C[x][z]);
		}

	printf("\n\n");

	// populate D
	for (int x = 0; x < 5; x++)
		for (int z = 0; z < DEPTH; z++)
		{
			D[x][z] = C[(x - 1) % 5][z] ^ C[(x + 1) % 5][(z + 1) % DEPTH];
			for (int y = 0; y < 5; y++)
			{
				int current_val = get_cube_pt(x, y, z, state);
				set_cube_pt(current_val ^ D[x][z], x, y, z, state);
			}
		}

	for (int x = 0; x < 5; x++)
		for (int z = 0; z < DEPTH; z++)
		{
			printf("%01x", D[x][z]);
		}
	printf("\n\n");
}

void rho()
{
	int x = 1;
	int y = 0;

	uint8_t temp_arr[STATE_SIZE] = {0};

	for(int z = 0; z < DEPTH; z++){
		int current_val = get_cube_pt(0, 0, z, state);
		set_cube_pt(current_val, 0, 0, z, temp_arr);
	}

	for (int t = 0; t < 24; t++)
	{
		for (int z = 0; z < DEPTH; z++)
		{
			int current_val = get_cube_pt(x, y, (z + (t + 1) * (t + 2) / 2) % DEPTH, state);
			set_cube_pt(current_val, x, y, z, temp_arr);
		}
		int temp = y;
		y = (3 * y + 2 * x) % 5;
		x = temp;
	}

	for(int i = 0 ;  i < STATE_SIZE; i++){
		state[i] = temp_arr[i];
	}
}

void pi()
{

	uint8_t temp_arr[STATE_SIZE] = {0};

	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 5; y++)
			for (int z = 0; z < DEPTH; z++)
			{
				int current_val = get_cube_pt((x + 3 * y) % 5, x, z, state);
				set_cube_pt(current_val, x, y, z, temp_arr);
			}
	
	for(int i = 0 ;  i < STATE_SIZE; i++){
		state[i] = temp_arr[i];
	}
}

void chi() {
	uint8_t temp_arr[STATE_SIZE] = {0};

	for(int x = 0; x < 5; x++)
		for(int y = 0; y < 5; y++)
			for(int z = 0; z < DEPTH; z++){
				int op_1 = get_cube_pt(x, y, z, state);
				int op_2 = get_cube_pt((x + 1) % 5, y, z, state) ^ 1;
				int op_3 = get_cube_pt((x + 2) % 5, y, z, state);
				set_cube_pt(op_1 ^ (op_2 & op_3),x, y, z, temp_arr);
			}

	for(int i = 0 ;  i < STATE_SIZE; i++){
		state[i] = temp_arr[i];
	}
}

void iota(int round) {
	
	for(int i = 0 ; i < 8; i++){
		state[i] ^= RC_TABLE[round][i];
	}

}

void init(char *ptext)
{
	int ptext_len = strlen(ptext);

	strcpy((char *)input, ptext);

	int pad_len = RATE_LEN - (ptext_len % RATE_LEN);

	// pad appropriately
	if (pad_len == 1)
	{
		input[ptext_len] = 0x86;
	}
	else if (pad_len > 1)
	{
		input[ptext_len] = 0x06;
		input[ptext_len + pad_len - 1] = 0x80;
	}
}

void get_hash(char *ptext)
{
	init(ptext);

	uint8_t *current_block = input;
	uint8_t *current_block_2 = input;

	for (int i = 0; i < RATE_LEN / 8; i++)
	{
		current_block_2 = input + 8 * i;
		reverse_endian(current_block_2);
	}

	xor_rate(current_block);

	theta();
	rho();
	pi();
	chi();
	iota(0);

	for (int i = 0; i < STATE_SIZE; i++)
		printf("%02x ", state[i]);
}
