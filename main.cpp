#include <iostream>
#include <string.h>
#include <stack>
#include <sstream>
#include <math.h>
enum paramstype{
	NO_PARAMS,
	HELP,
	MALFORMED_PARAMS,
	VALID_PARAMS
};
paramstype getParamsType(int,char**);
bool isNumber(char*);
double toNumber(char*);
bool isOperator(char*);
bool handleOperator(std::stack <double>&, char*);

int main(int argc, char* argv[]){
	switch (getParamsType(argc,argv)){
	case NO_PARAMS:
		std::cerr << argv[0] << " requires at least one parameter.\n" <<
			"Try " << argv[0] << " -h for more information.\n";
		return 1;
		break;
	case HELP:
		std::cout << "TODO: help file here.\n";
		return 0;
		break;
	case MALFORMED_PARAMS:
		std::cerr << "Bad parameter list.\n" <<
			"Try " << argv[0] << " -h for more information.\n";
		return 1;
		break;
	}
	
	std::stack <double> numberStack;
	for (int i = 1; i < argc; i++){
		if (isNumber(argv[i])){
			numberStack.push(toNumber(argv[i]));
		}else if (isOperator(argv[i])){
			if (!handleOperator(numberStack,argv[i])){
				std::cerr << "Bad parameter list.\n" <<
					"Try " << argv[0] << " -h for more information.\n";
				return 1;
			}
		}else{
			std::cerr << "Unrecognized parameter " << argv[i] << ".\n" <<
				"Try " << argv[0] << " -h for more information.\n";
			return 1;
		}
	}
	std::cout << numberStack.top() << std::endl;
}

paramstype getParamsType(int argc, char* argv[]){
	if (argc == 1){
		return NO_PARAMS;
	}
	if (argc == 2 && strcmp(argv[1],"-h") == 0){
		return HELP;
	}
	return VALID_PARAMS;
}

bool isNumber(char* input){
	int i = 0;
	if (input[i] == 0){
		return false;
	}
	int decimalCount = 0;
	int numbersCount = 0;
	int exponentialCount = 0;
	while (input[i] != 0){
		if (input[i] == '-' && (i != 0)){
			if (input[i-1] != 'e'){
				return false;
			}
		}
		if (input[i] == '.'){
			++decimalCount;
		}
		if (input[i] == 'e'){
			++exponentialCount;
		}
		if (input[i] >= '0' && input[i] <= '9'){
			++numbersCount;
		}else{
			if (input[i] != '.' && input[i] != '-' && input[i] != '+' && input[i] != 'e'){
				return false;
			}
		}
		++i;
	}
	if (decimalCount > 1){
		return false;
	}
	if (numbersCount == 0){
		return false;
	}
	if (exponentialCount > 1){
		return false;
	}
	return true;
}

double toNumber(char* input){
	std::stringstream inStream;
	inStream << input;
	double outNum;
	inStream >> outNum;
	return outNum;
}

bool isOperator(char* input){
	if (input[0] == 0){
		return false;
	}
	if (input[0] == '+' && input[1] == 0){
		return true;
	}
	if (input[0] == '-' && input[1] == 0){
		return true;
	}
	if (input[0] == '*' && input[1] == 0){
		return true;
	}
	if (input[0] == '/' && input[1] == 0){
		return true;
	}
	if (input[0] == '^' && input[1] == 0){
		return true;
	}
	return false;
}
bool handleOperator(std::stack<double> &numberStack,char* mathOperator){
	double num1;
	double num2;
	if (!numberStack.empty()){
		num1 = numberStack.top();
		numberStack.pop();
	}else{
		std::cout << "Not enough arguments for " <<
			mathOperator << std::endl;
		return false;
	}
	if (!numberStack.empty()){
		num2 = numberStack.top();
		numberStack.pop();
	}else{
		std::cout << "Not enough arguments for " <<
			mathOperator << std::endl;
		return false;
	}
	switch (mathOperator[0]){
	case '+':
		numberStack.push(num1+num2);
		break;
	case '-':
		numberStack.push(num2-num1);
		break;
	case '*':
		numberStack.push(num1*num2);
		break;
	case '/':
		if (num1 == 0){
			std::cout << "Divide by zero error\n";
			return false;
		}
		numberStack.push(num2/num1);
		break;
	case '^':
		numberStack.push(pow(num2,num1));
		break;
	}
	return true;
}
