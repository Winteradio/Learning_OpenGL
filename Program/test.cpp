#include <iostream>

using namespace std;

int main(){
	 
	int cpp = (int) __cplusplus;
	int year = cpp / 100;
	int month = cpp % 100;

	cout << " Year : " << year << " Month : " << month << endl;

	return 0;
}

