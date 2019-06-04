#pragma once

#include <SDL_keycode.h>

class IKeyboardSensor {
public:
	virtual ~IKeyboardSensor() {}

	virtual void onKeyDown(SDL_KeyboardEvent e) = 0;
	virtual void onKeyUp(SDL_KeyboardEvent e) = 0;
	virtual void onKeyUpdate(const Uint8* keys) = 0;
};