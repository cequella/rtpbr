#pragma once

#include <memory>
#include <stdexcept>
#include <set>
#include <vector>

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "IScene.h"

#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanImageView.h"
#include "GraphicPipeline.h"
#include "FrameBuffer.h"

struct WindowSettings {
	std::string title;
	unsigned width;
	unsigned height;
};

class Window {
private:
	static std::unique_ptr<Window> WINDOW_INSTANCE;

	bool _isRunning = true;

	// Window settings
	const WindowSettings _settings = { "RTPBR" , 800, 600};
	
	// Vulkan settings
	VulkanInstance _instance = VulkanInstance();
	VulkanPhysicalDevice _physicalDevice = VulkanPhysicalDevice();
	VulkanLogicalDevice _logicalDevice = VulkanLogicalDevice();
	VulkanSwapChain _swapChain = VulkanSwapChain();
	VulkanImageView _imageView = VulkanImageView();
	GraphicPipeline _pipeline = GraphicPipeline();
	FrameBuffer _frameBuffer = FrameBuffer();

	VkSurfaceKHR _surface = VkSurfaceKHR();
	
	SDL_Window* _window = nullptr;
	IScene* _scene = nullptr;
	
	Window() {}

	void _closeEverything();
	void _startSDL2Subsystems();
	void _createWindow();
	void _createSurface();

public:
	static Window& instance() {
		if (not WINDOW_INSTANCE) {
			WINDOW_INSTANCE = std::unique_ptr<Window>(new Window);
		}
		return *WINDOW_INSTANCE;
	}

	WindowSettings settings() const noexcept;

	void initializeWith(IScene& tScene);
	void close() noexcept;
};