#include <iostream>
#include <stdexcept>

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#undef main

#include "IScene.h"
#include "Window.h"

#include "IO.h"

class InitialScene final : public IScene, IKeyboardSensor {
private:
	Window& window = Window::instance();
	KeyboardListener keyboardListener = KeyboardListener();

public:
	InitialScene() {
		keyboardListener.assign(*this);
	}

	// IScene
	virtual void onUpdate() override {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			keyboardListener.notify(e);
		}
	}
	virtual void onRender(double) override {
	}

	// IKeyboardSensor
	virtual void onKeyDown(SDL_KeyboardEvent e) override {
		if (e.keysym.sym == SDLK_ESCAPE) {
			window.close();
		}
	}
	virtual void onKeyUp(SDL_KeyboardEvent e) override {
	}
	virtual void onKeyUpdate(const Uint8* keys) override {
	}
};


int main() {
	Window& window = Window::instance();
	InitialScene scene;

	try {
		window.initializeWith(scene);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
