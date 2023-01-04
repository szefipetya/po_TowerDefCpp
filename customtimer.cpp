#include"customtimer.h"
/*template<typename T>
void CustomTimer::setTimeout(void (fun)(T*),T*e, int delay) {
	this->clear = false;
	std::thread t([=]() {
		if (this->clear) return;
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		if (this->clear) return;
		fun(e);
		});
	this->thread = &t;
	t.detach();
}
template<typename T>
void CustomTimer::setInterval(void (fun)(T*),T* e, int interval) {
	this->clear = false;
	std::thread t([=]() {
		while (true) {
			if (this->clear) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			if (this->clear) return;
			fun(e);
		}
		});
	this->thread = &t;
	t.detach();
}

void CustomTimer::stop() {
	this->clear = true;
}*/