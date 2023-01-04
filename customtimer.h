#pragma once
#include <iostream>
#include <thread>
#include <chrono>
class Entity;

class CustomTimer {
	bool clear = false;
public:
	int delay = 0;
	template<typename T>
	void setTimeout(void (fun)(T*), T* e, int delay) {
		this->clear = false;
		this->delay = delay;
		std::thread t([=]() {
			if (this->clear) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			if (this->clear) return;
			if (e != nullptr)
			fun(e);
			this->~CustomTimer();
			});
		t.detach();
		thread = &t;
	}
	template<typename T>
	void setInterval(void (fun)(T*), T* e, int interval) {
		this->clear = false;
		this->delay = interval;

		std::thread t([=]() {
			while (!this->clear) {
				if (this->clear) return;
				std::this_thread::sleep_for(std::chrono::milliseconds(interval));
				if (this->clear) return;
				if (e != nullptr)
				fun(e);
			}
			this->~CustomTimer();
			});
		t.detach();
		thread = &t;
	}

	template<typename T>
	void setIntervalWithInit(void (init)(T*), void (fun)(T*), T* e, int interval) {
		this->delay = interval;
		this->clear = false;
		std::thread t([=]() { 
			init(e);
			while (!this->clear) {
				if (this->clear) return;
				std::this_thread::sleep_for(std::chrono::milliseconds(interval));
				if (this->clear) return;
				if(e!=nullptr)
				fun(e);
			}
			this->~CustomTimer();
		});
		t.detach();
		thread = &t;
	}

	void stop() {
		if(this!= nullptr)
		this->clear = true;
	}
	std::thread *thread;
	~CustomTimer() {

	}
};
