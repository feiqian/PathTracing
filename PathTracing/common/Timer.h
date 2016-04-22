#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
#include <functional>
#include <map>
#include <iostream>

class Timer {
public:
	Timer() {
		restart();
	}

	// return current unix timestamp
	void restart() {
		m_start_time = std::chrono::high_resolution_clock::now();
	}

	// return duration in seconds
	double duration() const {
		auto now = std::chrono::high_resolution_clock::now();
		auto m = std::chrono::duration_cast<std::chrono::microseconds>(now - m_start_time).count();
		return m * 1.0 / 1e6;
	}

protected:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;

};

class TotalTimer {
public:
	TotalTimer(const std::string& msg):
		msg(msg) {
			printf("%s start...\n",msg.c_str());
			timer.restart();
	}

	~TotalTimer();

	void print();
	void restart(const std::string& msg);

	std::string msg;
	Timer timer;
};

#endif
