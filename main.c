#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_MAX_LENGTH 128
#define ALPHABET_SIZE 32





void print_char_array(char [], unsigned long );
int* parse_to_dec(char* , unsigned long );
int isValidChar(char , char* );
int char_to_dem(char , char* , int* );
void print_dec_array(int* , unsigned long );
void getBin(int , char *);
int bin_to_dem(char* );
char* last_char(char* );
void print_n_bit_group(char * , int );
char decoce(char* );

int code_array[ALPHABET_SIZE];
char char_array[ALPHABET_SIZE];


int main(int argc, const char * argv[])
{

	char* user_in = NULL;
    int* msg_dec = NULL;
    char* num_5bit = NULL;
    char* num_8bit = NULL;
    char* dec_t_bin = NULL;
    char* dec_t_bin2 = NULL;
    char* digit = NULL;
    int octlets = 0;
    int* msg_dec_2 = NULL;
    char* last = NULL;
    int msg_len =0;
    char m;


  //generate character array from ascii values
  int i;
	int ascii_num =64 ; //65 is A , 66 is B

	for (i=0; i<ALPHABET_SIZE; i++) {
		code_array[i]=i;
	}

	for (i=1; i<27; i++) {
		char_array[i]= ascii_num + i; //value of next character in ascii (8bit)
	}
    //manually declare coding for special symbols
	char_array[0] = ' ';
	char_array[27] = '.';
	char_array[28] = ',';
	char_array[29] = ':';
	char_array[30] = '?';
	char_array[31] = '!';


    //print character set for coding
	printf("Valid charecter for messages are:\n" );
	print_char_array(char_array, ALPHABET_SIZE);

    //malloc enough space for user message to encrypt
	user_in = (char*) malloc(sizeof(char) * 512);
	if(user_in == NULL) { // check for malloc errors
		fprintf(stderr, "Can't malloc memory for user input!\n");
		exit(-1);
	}

    //accept user input below
    printf("Enter message to crypt: ");
    fgets(user_in, MSG_MAX_LENGTH, stdin);

    //print user input to console
    printf("\nMessage is: %s\n",user_in);

    //sanity checks for user input eg min max message lenght
    if(strlen(user_in) > MSG_MAX_LENGTH) {
		printf("Toooo big input aboarting!\n");
	}
	if(strlen(user_in) <= 0) {
		printf("Toooo small input aboarting!\n");
	}


    //check every character in user input and decide if it can be decoded given tha character set we have
	for(i=0; i<strlen(user_in)-1; i++) {
		if(isValidChar(user_in[i], char_array) == 1) {
			continue;
		}
		else { //a character can't be decoded so we are aborting , sorry folks
			printf("illegal character detected: aboarting...\n");

			free(user_in);
			user_in=NULL;
			exit(-1);
		}
	}

	msg_len= strlen(user_in)-1;
	printf("Message length to encrypt: %d\n", msg_len);


    //first stage encryption transform characters to demical digits

	msg_dec = parse_to_dec(user_in,msg_len);
		printf("Decical represantetion\n" );
    //print the demical represantetion of the message
    print_dec_array(msg_dec,msg_len );

    num_5bit = (char*) malloc(sizeof(char)*6); //5 bis plus the null termination = 6bits
    if(num_5bit == NULL) {
        fprintf(stderr, "Can't malloc memory for demical to binary conversation\n");
    }



    dec_t_bin = (char*) malloc(sizeof(char)*msg_len*5+1);
    if(dec_t_bin == NULL) {
    	fprintf(stderr, "Can't malloc memory for decimal to binary conversation\n");
    	free(num_5bit);
    	free(msg_dec);
    	free(user_in);
    	exit(-1);
    }
    memset(dec_t_bin,0,msg_len*5+1);

    //binary repsesentasion of inputed message
    for(i=0; i< msg_len; i++) {
        getBin(msg_dec[i], num_5bit);
        strncat(dec_t_bin,num_5bit,strlen(num_5bit));

    }
    dec_t_bin[msg_len*5+1]='\0';
	//print the binary message in 5 bit groups
	print_n_bit_group(dec_t_bin , 5 );



	//find the number of 8bits segments for binary string
	octlets = strlen(dec_t_bin)/8;
	if(strlen(dec_t_bin)%8 > 0) {
		octlets++;
	}
	//printf("Number of octels %d\n", octlets);
	num_8bit = (char*) malloc(sizeof(char)*8+1);
	if(num_8bit == NULL) {
		exit(-1);
	}

	//set digit as index to traverse the binary string
	digit = dec_t_bin;
	msg_dec_2 = (int*) malloc(sizeof(int)*octlets);
	if(msg_dec_2 == NULL) {
		exit(-1);
	}

	dec_t_bin2 = (char*) malloc(sizeof(char)*8*msg_len+1);
	if(dec_t_bin2 == NULL) {
		exit(-1);
	}

	//make the 8 bit groups of the message
	for(i=0; i<octlets-1; i++) {
		strncpy(num_8bit,digit,8);
		num_8bit[8]='\0';
		digit = digit+8;
		//make the new decimal representation from binary 8bit segments
		strncat(dec_t_bin2, num_8bit, strlen(num_8bit));
		msg_dec_2[i] = bin_to_dem(num_8bit);
	}

	last = last_char(dec_t_bin);

	memset(num_8bit,0,9); //hardcoded memset
	//decode the last segment of last bits it might be less than 8 bits so we process every bit until '\0' character
	while(digit<=last) {
		strncat(num_8bit,digit,1);
		digit++;
		if(digit>last) { // digit points to null termnitor address break please
			break;
		}
	}

	*digit='\0';
	//zero pading last segment to rich lenth of 8 bits
	while(strlen(num_8bit) !=8) {
		num_8bit[strlen(num_8bit)]='0';
		num_8bit[strlen(num_8bit)+1]='\0';
		#ifdef DEBUG_MSG
		printf("last oct len %d\n",strlen(num_8bit));
		#endif
	}
	msg_dec_2[octlets-1] = bin_to_dem(num_8bit);
	strncat(dec_t_bin2, num_8bit, strlen(num_8bit));
	//print in 8 bits groups
	print_n_bit_group(dec_t_bin2,8);
	//print new decimal representation
	print_dec_array(msg_dec_2, octlets);


	printf("Decoding message: \n" );
	//decoding
	i=0;
	while(i<msg_len) {
	memset(num_5bit,'\0',6);
	strncpy(num_5bit,dec_t_bin2, 5);
	m=decoce(num_5bit);
	printf("%c",m);
	dec_t_bin2 =dec_t_bin2+5;
	i++;
	}

    //clean up the memory allocations
    free(num_5bit);
    free(user_in);
    free(msg_dec);
    free(dec_t_bin);
    free(num_8bit);
    //free(dec_t_bin2);
    free(msg_dec_2);

    //this is the eof
    puts("\npress enter to exit!\n");
    getchar();
	return 0;
}

char* last_char(char* str)
{
	int i = 0;
	char* last_c;
	if(str == NULL) {
		return NULL;
	}
	last_c=str;
	while(*last_c != '\0') {
		if(*(last_c+1) == '\0') {
			return last_c;
		}
		last_c++;
	}
	return last_c;
}

int bin_to_dem(char* bin)
{
	int i=0;
	int len=0;
	int r =0;

	if(bin == NULL) {
		return -1;
	}
	len =strlen(bin);
	for(i = 0; i < len; i++)
    {
        r = r * 2 + (bin[i] == '1' ? 1 : 0);
    }
    return r;
}

char decoce(char* char_5bit)
{
	int index =-1;
	if(char_5bit == NULL) {
		return '\0';
	}
	if(strlen(char_5bit) > 5) {
		return '\0';
	}
	index = bin_to_dem(char_5bit);
	return char_array[index];
}

void print_n_bit_group(char * str, int gsize)
{
	int i=0;
	if(str == NULL) {
		return;
	}
	if(gsize <=0) {
		gsize=1;
	}
	printf("[");
	for(i=0;i<strlen(str);i++) {
		if(i%gsize==0) {
			if(i>0) {
				printf("][");
			}
		}
		printf("%c",str[i]);
	}
	printf("]\n");
}

int isValidChar(char c, char* char_set)
{
	int len=0;
	int valid=0;

	for(len=0; len<strlen(char_set); len++) {
		if(char_set[len] == c || char_set[len] =='\0') {
			valid=1;
			break;
		}
	}
	return valid;
}

int char_to_dem(char c, char* char_set, int* dec_set)
{
	int len=0;
	int dem=-1;
	if(char_set == NULL) {
		printf("Empty characher set!\n");
		return -1;
	}
    if(dec_set == NULL) {
		printf("Empty demical number set!\n");
		return -1;
	}
	for(len=0; len<strlen(char_set); len++) {
		if(char_set[len] == c) {
            dem=dec_set[len];
			break;
		}
	}
	return dem;
}


void print_char_array(char array[], unsigned long length)
{
    if(length <=0) {
        return;
    }

    int i=0;
	for(i=0; i<length; i++) {
		if(array[i] == ' ') {
			printf("<space> ");
		}
		else {
			printf("%c ",array[i]);
		}
	}
	printf("\n");
}

void print_dec_array(int* array, unsigned long length)
{
    if(length <=0 || array == NULL) {
        return;
    }
    int i=0;

	for(i=0; i<length; i++) {
        printf("[%d] ", array[i]);
	}
	printf("\n");
}

void getBin(int num, char *str)
{
    *(str+5) = '\0';
    int mask = 0x10 << 1;
    while(mask >>= 1)
        *str++ = !!(mask & num) + '0';
}

int* parse_to_dec(char* msg, unsigned long length)
{
    int i=0;
    int j=0;
    int* msg_dec = NULL;

    if(length <= 0) {
        return NULL;
    }
    if(msg == NULL) {
        printf("Message is NULL\n");
        return NULL;
    }
    msg_dec = (int*) malloc(sizeof(int)*length);
    if(msg_dec == NULL) {
        fprintf(stderr, "Can't malloc message demical array!\n");
        exit(-1);
    }
    for(i=0; i<length; i++) {
        msg_dec[i]= char_to_dem(msg[i], char_array, code_array);
    }
    return msg_dec;
}
