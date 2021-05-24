#include <iostream>
#include "Scanner.h"

using namespace std;

int main()
{
	Scanner scanner;
	while (auto token = scanner.lex())
	{
		cout << token << endl;
		cout << "[Identifier: " << scanner.matched() << "]" << endl;
	}

	return 0;
};
