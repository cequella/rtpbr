#pragma once

#include <set>

#include <vulkan/vulkan.h>

#include "VulkanLogicalDevice.h"

class VulkanPresentationQueue {
private:
	std::vector<VkDeviceQueueCreateInfo> _infos = {};

public:	

	void initialize(VulkanPhysicalDevice device, VkSurfaceKHR surface);
	VkQueue& queue() noexcept;	
	std::vector<VkDeviceQueueCreateInfo> infos() const noexcept;
};

