#pragma once

#include <set>

#include <vulkan/vulkan.h>

#include "VulkanPhysicalDevice.h"

class VulkanLogicalDevice {
private:
	VkDevice _logicalDevice = VkDevice();
	VkQueue _queue = VkQueue();

	std::vector<VkDeviceQueueCreateInfo> _createPresentationQueue(VulkanPhysicalDevice physicalDevice);

public:
	VkDevice& get() noexcept;
	VkQueue& queue() noexcept;

	void initializeWith(VulkanPhysicalDevice physicalDevice);
	void dispose() noexcept;
};

