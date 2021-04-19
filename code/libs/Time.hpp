#pragma once

class Time {
private:
	static Time* instance;
	static float deltaTime;
	Time() {
		deltaTime = 0.0f;
	}
public:
	/*! \brief Gets time global object */
	static Time* get() {
		if (!instance) {
			instance = new Time();
		}
		return instance;
	}
	/*! \brief Tests time between frames */
	void setDeltaTime(float deltaTime) {
		this->deltaTime = deltaTime;
	}
	/*! \brief Gets times between frames */
	static float getDeltaTime() {
		return deltaTime;
	}
};