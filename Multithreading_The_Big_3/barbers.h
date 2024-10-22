#pragma once
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

bool quitBarbers = false;
int waiting = 0;
int served = 0;
float seconds;
clock_t t = clock();
mutex mb;
condition_variable cvb;

void Barber() {
	while (!quitBarbers) {
		unique_lock<mutex> lck(mb);
		if (waiting < 1) {
			cout << "barber going to sleep" << endl;
			cvb.wait(lck);

			cout << "barber waking up" << endl;
		}
		if (waiting > 0) {
			--waiting;
			++served;
			lck.unlock();
			t += clock() - t;
			seconds = (float)t / (float)CLOCKS_PER_SEC;
			cout << "barber working with client number " << served << " with " << waiting << " clients waiting after " << seconds << " seconds" << endl;
			this_thread::sleep_for(1s);
		}
	}
}

void Client() {
	while (!quitBarbers) {
		this_thread::sleep_for((rand() % 2 + 1) * 1s);
		cout << "new client entering" << endl;
		cout << "number of clients already waiting: " << waiting << endl;
		unique_lock<mutex> lck(mb);
		++waiting;
		lck.unlock();
		cvb.notify_all();
	}
}

void QuitBarbers() {
	cin.get();
	quitBarbers = true;
}

void RunBarbers() {
	thread t1(&Barber);
	thread t2(&Client);
	thread t3(&QuitBarbers);

	t1.join();
	t2.join();
	t3.join();
}