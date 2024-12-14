/*
 Joey Borrelli
 9/16/A.D.2024
 Purpose: a program for my CSC 2400 class. finds the GCD using Euclid's Algorithm
*/

#include <iostream>
#include <sstream>
#include <cmath>

/* A sub-method of calculate_gcd, used to do the recursive comparison part of euclid's algorithm
 * INPUT: two ints
 * OUTPUT: Prints the GCD*/
void euclidRec(int m, int n){
	if((m % n) == 0){ // check if n evenly divides into m
		std::cout << "= " << abs(n);		
	}else{
		euclidRec(n, (m % n));
	}
}

/* An algorithm used to calculate the greatest common divisor of two given numbers, calls a sub method to do the recursive math part. Makes n always the smallest number, and checks for the undefined case.
 * INPUT: Two Integers
 * OUTPUT: output statement about the GCD */ 
void calculate_gcd(int m, int n){
	std::cout << "gcd(" << m << "," << n << ") ";
	if((n == 0) && (m == 0)){ // if both are zero, undef.
		std::cout << "is undefined";
	}else if((n == 0)){ // if n is zero, m is gcd
		std::cout << "= " << abs(m);
	}else if(m == 0){ // if m is zero, n is gcd
		std::cout << "= " << abs(n);
	}else if(abs(n) <= abs(m)){ // check if n is smaller than m
		euclidRec(m, n);
	}else{ // if not, switch.
		int temp = m;
		m = n;
		n = temp;
		euclidRec(m,n);
	}
}

void usage(char *name) {
	std::cerr << "Calculates the greatest common divisor of two numbers." << std::endl;
	std::cerr << "usage:" << name << " [m] [n]" << std::endl;
	std::cerr << "m,n integers, not both zero" << std::endl;
}

int main(int argc, char *argv[]) {
	int m = 0;
	int n = 0;

	if(argc != 3) {
		std::cerr << "Invalid number of arguments." << std::endl << std::endl;
		usage(argv[0]);

		return 0;
	}

	std::istringstream iss(argv[1]);
	if((iss>>m).fail() || !iss.eof()) {
		std::cerr << argv[1] << " is not a valid integer." << std::endl << std::endl;
		usage(argv[0]);

		return 0;
	}

	iss.str(argv[2]);
	iss.clear();
	if((iss>>n).fail() || !iss.eof()) {
		std::cerr << argv[2] << " is not a valid integer." << std::endl << std::endl;
		usage(argv[0]);

		return 0;
	}

	calculate_gcd(m, n);

	return 0;
}
