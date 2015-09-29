
/**************************
 * bignum_math.c -- an outline for CLab1
 *
 * orginially written by Andy Exley
 * modified by Emery Mizero
 **************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum_math.h"


/*
 * Returns true if the given char is a digit from 0 to 9
 */
bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

/*
 * Returns true if lower alphabetic character
 */
bool is_lower_alphabetic(char c) {
	return c >= 'a' && c <= 'z';
}

/*
 * Returns true if upper alphabetic character
 */
bool is_upper_alphabetic(char c) {
	return c >= 'A' && c <= 'Z';
}

/*
 * Convert a string to an integer
 * returns 0 if it cannot be converted.
 */
int string_to_integer(char* input) {
	int result = 0;
	int length = strlen(input);
    int num_digits = length;
	int sign = 1;
	
	int i = 0;
    int factor = 1;

    if (input[0] == '-') {
		num_digits--;
		sign = -1;
    }

	for (i = 0; i < num_digits; i++, length--) {
		if (!is_digit(input[length-1])) {
			return 0;
		}
		if (i > 0) factor*=10;
		result += (input[length-1] - '0') * factor;
	}

    return sign * result;
}

/*
 * Returns true if the given base is valid.
 * that is: integers between 2 and 36
 */
bool valid_base(int base) {
	if(!(base >= 2 && base <= 36)) { 
		return false; 
	}
	return true;
}

/*
 * TODO
 * Returns true if the given string (char array) is a valid input,
 * that is: digits 0-9, letters A-Z, a-z
 * and it should not violate the given base and should not handle negative numbers
 */
bool valid_input(char* input, int base) {
	/*
	 * check for valid base and if negative
	 */
	if (!valid_base(base) || input[0]=='-') {
		return false;
	}
	else {
		int len = strlen(input);
		int i;
		for (i =0; i< len; i++){
			/*
			 * check if the input string is a digit/letter
			 */
			if (!(is_digit(input[i]) || is_lower_alphabetic(input[i]) || is_upper_alphabetic(input[i]))){
				return false;
			}
			/*
			 * if the int excesses the base?
			 */
			else if (is_digit(input[i])){
				if (input[i]-'0'>=base){     //convert char to int and compare with the base
					return false;
				}

			}
			/*
			 *or if the letter excesses the base?
			 */

			else if (is_lower_alphabetic(input[i])){
				if (input[i]-'a'+10 >=base){
					return false;
				}
			}
			else if (is_upper_alphabetic(input[i])){
				if (input[i] - 'A' + 10 >=base) {
					return false;
				}
			}
		}
		return true;

	}
}


/*
 * converts from an array of characters (string) to an array of integers
 */
int* string_to_integer_array(char* str) {
	int* result;
	int i, str_offset = 0;
		result = malloc((strlen(str) + 1) * sizeof(int));
		result[strlen(str)] = -1;
	for(i = str_offset; str[i] != '\0'; i++) {
		if(is_digit(str[i])) { 
			result[i - str_offset] = str[i] - '0';
		} else if (is_lower_alphabetic(str[i])) {
			result[i - str_offset] = str[i] - 'a' + 10;
		} else if (is_upper_alphabetic(str[i])) {
			result[i - str_offset] = str[i] - 'A' + 10;
		} else {
			printf("I don't know how got to this point!\n");
		}
	}
	return result;
}

/*
 * finds the length of a bignum... 
 * simply traverses the bignum until a negative number is found.
 */
int bignum_length(int* num) {
	int len = 0;
	while(num[len] >= 0) { len++; }
	return len;
}

/*
 * TODO
 * Prints out a bignum using digits and upper-case characters
 * Current behavior: prints integers
 * Expected behavior: prints characters
 */
void bignum_print(int* num) {
	int i;
	if(num == NULL) { return; }

	/* Handle negative numbers as you want
	 * let the last digit be -2 if negative
	 * */
	i = bignum_length(num);
	if (num[i]==-2){
		printf("-");
	}

	/* Then, print each digit */
	for(i = 0; num[i] >= 0; i++) {
		if (num[i]<=9){
			printf("%d", num[i]);
		}
		else if (num[i]>9){
			char digit =  num[i]+'A'-10;
			printf("%c", digit);
		}

	}
	printf("\n");
}

/*
 *	Helper for reversing the result that we built backward.
 *  see add(...) below
 */
void reverse(int* num) {
	int i, len = bignum_length(num);
	for(i = 0; i < len/2; i++) {
		int temp = num[i];
		num[i] = num[len-i-1];
		num[len-i-1] = temp;
	}
}


/*
 * used to add two numbers with the same sign
 * GIVEN FOR GUIDANCE
 */
int* add(int* input1, int* input2, int base) {
	int len1 = bignum_length(input1);
	int len2 = bignum_length(input2);
	int resultlength = ((len1 > len2)? len1 : len2) + 2;
	int* result = (int*) malloc (sizeof(int) * resultlength);
	int r = 0;
	int carry = 0;
	int sign = input1[len1];

	len1--;
	len2--;

	while(len1 >= 0 || len2 >= 0) {
		int num1 = (len1 >= 0)? input1[len1] : 0;
		int num2 = (len2 >= 0)? input2[len2] : 0;
		result[r] = (num1 + num2 + carry) % base;
		carry = (num1 + num2 + carry) / base;
		len1--;
		len2--;
		r++;
	}
	if(carry > 0) { result[r] = carry; r++; }
	result[r] = sign;
	reverse(result);
	return result;
}

/*
 * helper function for subtract
 * determine which number is larger of two positive numbers
 */
bool larger(int* input1, int* input2){
	int len1 = bignum_length(input1);
	int len2 = bignum_length(input2);
	if (len1<=len2){
		if (len1<len2){  //if input1 has less digit than input2
			return false;
		}
		int i;
		for (i =0; i < len1; i++ ){//they have the same length
			if (input1[i]<input2[i]){    //if the same digit in input1 is smaller than that in input2
				return false;
			}
		}
	}
	return true;  //else input1 is indeed larger than/equal input2 (longer or every digit is no less than that in input2
}


/*
 * helper function for subtract
 * subtract from the larger
 */
int* subtractLarger(int* input1, int* input2, int base){  //input1 is larger or equal than/to input2 and both positive

	int len1 = bignum_length(input1);
	int len2 = bignum_length(input2);
	int resultlength = ((len1 > len2) ? len1 : len2) + 2;
	int *result = (int *) malloc(sizeof(int) * resultlength);
	int r = 0;
	int carry = 0;
	int sign = -1;

	len1--;
	len2--;
	while(len1 >= 0 ) {
		int num1 = (len1 >= 0)? input1[len1]-carry : 0;
		int num2 = (len2 >= 0)? input2[len2] : 0;
		if (num1>=num2){
			result[r] = (num1-num2);
			carry = 0;
		}
		else {
			result[r]= num1+base-num2;
			carry = 1;

		}

		len1--;
		len2--;
		r++;
	}
	if (result[r-1]==0){
		result[r-1] = sign;
	}
	else {
		result[r] = sign;
	}
	reverse(result);
	return result;
}



/*
 * used to subtract two numbers with the same sign
 */
int* subtract (int* input1, int* input2, int base) {
	if (larger(input1,input2)){
		return subtractLarger(input1, input2, base);
	}
	else {
		int* res = subtractLarger(input2, input1, base);  //exchange input1 and input2, note the result is negative
		int sign = -2;									  //negative result
		res[bignum_length(res)] = sign;
		return res;
	}
}
/*
 * TODO
 * This function is where you will write the code that performs the heavy lifting, 
 * actually performing the calculations on input1 and input2.
 * Return your result as an array of integers.
 * HINT: For better code structure, use helper functions.
 */
int* perform_math(int* input1, int* input2, char op, int base) {

	/* 
	 * this code initializes result to be large enough to hold all necessary digits.
	 * if you don't use all of its digits, just put a -1 at the end of the number.
	 * you may omit this result array and create your own.
     */

	int resultlength = bignum_length(input1) + bignum_length(input2) + 2;
	int* result = (int*) malloc (sizeof(int) * resultlength);
 
	if(op == '+') {
		return add(input1, input2, base);
	}
	else if (op == '-'){
		return subtract(input1, input2, base);

	}

}


/*
 * Print to "stderr" and exit program
 */
void print_usage(char* name) {
	fprintf(stderr, "----------------------------------------------------\n");
	fprintf(stderr, "Usage: %s base input1 operation input2\n", name);
	fprintf(stderr, "base must be number between 2 and 36, inclusive\n");
	fprintf(stderr, "input1 and input2 are arbitrary-length integers\n");
	fprintf(stderr, "Two operations are allowed '+' and '-'\n");
	fprintf(stderr, "----------------------------------------------------\n");
	exit(1);
}


/*
 * MAIN: Run the program and tests your functions.
 * sample command: ./bignum 4 12 + 13
 * Result: 31
 */
int main(int argc, char** argv) {

	int input_base;

    int* input1;
    int* input2;
    int* result;

	if(argc != 5) { 
		print_usage(argv[0]); 
	}

	input_base = string_to_integer(argv[1]);

	if(!valid_base(input_base)) { 
		fprintf(stderr, "Invalid base: %s\n", argv[1]);
		print_usage(argv[0]);
	}
	

	if(!valid_input(argv[2], input_base)) { 
		fprintf(stderr, "Invalid input1: %s\n", argv[2]);
		print_usage(argv[0]);
	}

	if(!valid_input(argv[4], input_base)) { 
		fprintf(stderr, "Invalid input2: %s\n", argv[4]);
		print_usage(argv[0]);
	}

	if(argv[3][0] != '-' && argv[3][0] != '+') {
		fprintf(stderr, "Invalid operation: %s\n", argv[3]);
		print_usage(argv[0]);
	}

	input1 = string_to_integer_array(argv[2]);
	input2 = string_to_integer_array(argv[4]);

	result = perform_math(input1, input2, argv[3][0], input_base);

	printf("Result: ");
	bignum_print(result);

	printf("\n");
	exit(0);
}
