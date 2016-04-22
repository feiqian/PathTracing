#include "Timer.h"

void TotalTimer::print() {
	if(msg.empty()) return;
	printf("%s end,which spent %lf secs in total\n",
		msg.c_str(), timer.duration());
	msg.clear();
}

TotalTimer::~TotalTimer() {
	print();
}

void TotalTimer::restart(const std::string& msg) {
	print();
	this->TotalTimer::TotalTimer(msg);
}
