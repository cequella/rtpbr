#pragma once

#include <stdexcept>
#include <vector>

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

class VulkanInstance {
private:
	VkInstance _vkInstance = VkInstance();

#ifdef NDEBUG
	const bool _enableValidationLayers = false;
#else
	const bool _enableValidationLayers = true;
#endif

	const std::vector<const char*> _validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	bool _checkValidationLayerSupport();

public:
	void initializeWith(SDL_Window* window);

	VkInstance& get() noexcept;
	void dispose() noexcept;
};

