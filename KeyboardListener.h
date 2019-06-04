#pragma once

#include <set>
#include <stdexcept>

#include <SDL.h>

#include "IKeyboardSensor.h"

class KeyboardListener {
private:
	std::set<IKeyboardSensor*> _sensor = {};

	void _notifyChangeAndKeyPress(const Uint8* keyBuffer, SDL_KeyboardEvent e) const noexcept;
	void _notifyChangeAndKeyRelease(const Uint8* keyBuffer, SDL_KeyboardEvent e) const noexcept;
public:
	
	void assign(IKeyboardSensor& sensor) noexcept;
	void unassign(IKeyboardSensor& sensor);
	void clear();
	void notify(SDL_Event e) const noexcept;

};

