/*
AUTHOR: Joey R. Borrelli
Date:   11/26/A.D.2024
Course: CSC 2400-001
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int returnMaxElementRecursive(std::vector<int>);

int main(int argc, char* argv[]){
    if(argc < 2){ // checks if there wasn't an argument with the execution.
        std::cout << "Please insert a text file\n";
    }else{
        std::ifstream inputText(argv[1]); // create an input file stream object

        if(!inputText){ //  checks if the file opened properly
            std::cout << "File: " << argv[1] << " didn't open properly\n";
        }else{
            std::vector<int> arrayOfValues;
            std::string input; // creates a string to hold our input from the file
            while(std::getline(inputText, input)){ // iterates as long as our text file isn't empty, reading in all the values.
                try{
                    arrayOfValues.push_back(std::stoi(input)); // add element to array
                }catch(const std::invalid_argument& error){
                    std::cout << "Cannot convert text file element \"" << input << "\"" << " to integer: " << error.what() << std::endl;
                    return(-1); // exit program
                }
            }
            if(!arrayOfValues.empty()){ // checks if array isn't empty
                std::cout << returnMaxElementRecursive(arrayOfValues); // print out the max element in my array of values
            }else{
                std::cout << "Please enter a text file with number values.";
            }
        }
    }
}

/*
Purpose: This algorithm returns the biggest integer values in the array given to it in a decrease and conquer manner
INPUT: an array of integers
OUTPUT: the largest integer in the inputted array.
*/
int returnMaxElementRecursive(std::vector<int> array){
    if(array.size() == 1){ 
        return(array[0]); // base case (if there is only one element in the array)
    }else{ // recursive case (if there is more than 1 element in the array)
        
        // remove either the 1st or 2nd element
        if(array[0] > array[1]){
            array.erase(array.begin() + 1); // remove second element
        }else{
            array.erase(array.begin()); // remove first element
        }
        
        // recursive case: pass through array decreased by a constant (1 element)
        return(returnMaxElementRecursive(array));
    }
}