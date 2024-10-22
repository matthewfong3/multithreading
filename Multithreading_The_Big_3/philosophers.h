#pragma once
#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

bool quitPhilosophers = false;
bool chopsticks[5] = { true, true, true, true, true };
condition_variable cvp[5];
mutex mp;

void Philosopher(int i) {
	while (!quitPhilosophers) {
		// create lock
		unique_lock<mutex> lck(mp);

		// get the right index 
		int r = (i + 4) % 5; 
		
		// check if this.i && right.i chopsticks are available
		if (chopsticks[i] == true && chopsticks[r] == true) {
			// pick up chopsticks, unlock
			chopsticks[i] = false;
			chopsticks[r] = false;
			lck.unlock();

			cout << "philosopher " << i << " is eating" << endl;
			this_thread::sleep_for(200ms); // philosopher eating

			// lock, set down chopsticks, unlock
			cout << "philosopher " << i << " is done" << endl;
			lck.lock();
			chopsticks[i] = true;
			chopsticks[r] = true;
			lck.unlock();

			// notify adjacent philosophers so they may attempt to eat
			cvp[(i + 1) % 5].notify_all();
			cvp[(i + 4) % 5].notify_all();

			this_thread::sleep_for(200ms); // philosopher thinking
		}
		// philosopher cannot eat, so they must wait
		else cvp[i].wait(lck);
	}
}

void QuitPhilosophers() {
	cin.get();
	quitPhilosophers = true;
}

void RunPhilosophers() {
	thread t1(Philosopher, 0);
	thread t2(Philosopher, 1);
	thread t3(Philosopher, 2);
	thread t4(Philosopher, 3);
	thread t5(Philosopher, 4);
	thread t6(QuitPhilosophers);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
}