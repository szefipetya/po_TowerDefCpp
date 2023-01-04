#pragma once
class EventArgs
{
public:
	EventArgs(int d) {
		data = d;
	}
	EventArgs() {
	}
	virtual ~EventArgs() {

	}
	int data=0;
};

