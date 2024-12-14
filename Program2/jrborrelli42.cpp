#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int returnMaxElement(std::vector<int>);

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
                    arrayOfValues.push_back(std::stoi(input));
                }catch(const std::invalid_argument& error){
                    std::cout << "Cannot convert text file element \"" << input << "\"" << " to integer: " << error.what() << std::endl;
                    return(-1); // exit program
                }
            }
            if(!arrayOfValues.empty()){
                std::cout << returnMaxElement(arrayOfValues); // print out the max element in my array of values
            }else{
                std::cout << "Please enter a text file with number values.";
            }
        }
    }
}

/*
Purpose: This algorithm returns the biggest integer values in the array given to it
INPUT: an array of integers
OUTPUT: the largest integer in the inputted array.
*/
int returnMaxElement(std::vector<int> array){
    int maxElement = array[0];
    for(std::vector<int>::size_type i = 1; i < array.size(); i++){ // loop through the array, i is size_type instead of int to prevent compiler warning
        if(array[i] > maxElement){ // if the input is bigger than previous maxElement
            maxElement = array[i]; // then set maxElement to the new bigger input.
        }
    }
    return(maxElement);
}