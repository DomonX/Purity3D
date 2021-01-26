#pragma once

class Time {
private:
	static Time* instance;
	static float deltaTime;
	Time() {
		deltaTime = 0.0f;
	}
public:
	static Time* get() {
		if (!instance) {
			instance = new Time();
		}
		return instance;
	}

	void setDeltaTime(float deltaTime) {
		this->deltaTime = deltaTime;
	}

	static float getDeltaTime() {
		return deltaTime;
	}
};