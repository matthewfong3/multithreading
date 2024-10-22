#include "philosophers.h"
#include "smokers.h"
#include "barbers.h"
#include <iostream>

using namespace std;

int main() {
	cout << "Running barbers thread\n" << endl;
	RunBarbers();
	cout << "\n--------------------------------------\n";

	cin.get();
	cout << "Running philosophers thread\n" << endl;
	RunPhilosophers();
	cout << "\n--------------------------------------\n";

	cin.get();
	cout << "Running smokers thread\n" << endl;
	RunSmokers();
	cout << endl;
	return 0;
}