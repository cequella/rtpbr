#pragma once

#include <vector>
#include <stdexcept>

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <optional>

#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanInstance.h"

class VulkanSettings {
private:
	VulkanInstance _instance = VulkanInstance();
	VulkanPhysicalDevice _physicalDevice = VulkanPhysicalDevice();
	VulkanLogicalDevice _logicalDevice = VulkanLogicalDevice();

public:
	void initializeWith(SDL_Window* window);
	void dispose();

	static std::vector<VkLayerProperties> layerProperties();

	VulkanInstance& vkInstance() noexcept;
	VulkanPhysicalDevice& vkPhysicalDevice() noexcept;
	VulkanLogicalDevice& vkLogicalDevice() noexcept;

};