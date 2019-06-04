#include "KeyboardListener.h"

void KeyboardListener::_notifyChangeAndKeyPress(const Uint8* keyBuffer, SDL_KeyboardEvent e) const noexcept {
	for (const auto& sensor : _sensor) {
		sensor->onKeyUpdate(keyBuffer);
		sensor->onKeyDown(e);
	}
}

void KeyboardListener::_notifyChangeAndKeyRelease(const Uint8* keyBuffer, SDL_KeyboardEvent e) const noexcept {
	for (const auto& sensor : _sensor) {
		sensor->onKeyUpdate(keyBuffer);
		sensor->onKeyUp(e);
	}
}

void KeyboardListener::assign(IKeyboardSensor& sensor) noexcept {
	_sensor.insert(&sensor);
}

void KeyboardListener::unassign(IKeyboardSensor& sensor) {
	bool sensorNotAvailable = true;
	for (auto target = _sensor.begin(); target not_eq _sensor.end(); target++) {
		if (*target == &sensor) {
			sensorNotAvailable = false;
			_sensor.erase(target);
			break;
		}
	}

	if (sensorNotAvailable) {
		throw std::logic_error("Sensor does not belongs to this listener");
	}
}

void KeyboardListener::clear() {
	_sensor.clear();
}

void KeyboardListener::notify(SDL_Event e) const noexcept {
	if (e.type == SDL_KEYDOWN) {
		const Uint8* keyBuffer = SDL_GetKeyboardState(NULL);
		_notifyChangeAndKeyPress(keyBuffer, e.key);
	} else if (e.type == SDL_KEYUP)  { // if SDL_KEYUP
		const Uint8* keyBuffer = SDL_GetKeyboardState(NULL);
		_notifyChangeAndKeyRelease(keyBuffer, e.key);
	}
}
