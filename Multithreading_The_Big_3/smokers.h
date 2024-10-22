#pragma once
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

bool quitSmokers = false;
mutex ms;
condition_variable cvs;
bool paper = false;
bool matches = false;
bool tobacco = false;

void Smoker(int i) {
	while (!quitSmokers) {
		unique_lock<mutex> lck(ms);

		if (matches && tobacco) {
			matches = false;
			tobacco = false;
		}
		else if (paper && tobacco) {
			paper = false;
			tobacco = false;
		}
		else if (paper && matches) {
			paper = false;
			tobacco = false;
		}
		lck.unlock();

		cout << "smoker " << i << " is smoking" << endl;
		this_thread::sleep_for(200ms);

		cvs.notify_all();
	}
}

void Server() {
	while (!quitSmokers) {
		unique_lock<mutex> lck(ms);
		int rng = rand() % 3;

		switch (rng) {
		case 0: // do not serve paper
			paper = false;
			matches = true;
			tobacco = true;
			break;
		case 1: // do not serve matches
			paper = true;
			matches = false;
			tobacco = true;
			break;
		case 2: // do not serve tobacco
			paper = true;
			matches = true;
			tobacco = false;
			break;
		}

		cvs.wait(lck); // wait for smoker to finish smoking before serving the next batch of nasty
	}
}

void QuitSmokers() {
	cin.get();
	quitSmokers = true;
}

void RunSmokers() {
	thread t1(Smoker, 0);
	thread t2(Smoker, 1);
	thread t3(Smoker, 2);
	thread t4(&Server);
	thread t5(&QuitSmokers);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
}