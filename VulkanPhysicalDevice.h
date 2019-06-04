#pragma once

#include <stdexcept>
#include <vector>
#include <optional>
#include <set>
#include <algorithm>

#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() and presentFamily.has_value();
	}
};
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanPhysicalDevice {
private:
	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR _surface = VkSurfaceKHR();
	
	const std::vector<const char*> _deviceExtensions = { 
		VK_KHR_SWAPCHAIN_EXTENSION_NAME 
	};

	int _rateDeviceSuitability(const VkPhysicalDevice device) const noexcept;
	bool _checkDeviceExtensionsSupport(VkPhysicalDevice device);
	bool _isDeviceSuitable(VkPhysicalDevice device) noexcept;
	QueueFamilyIndices _listQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails _swapChainSupport(VkPhysicalDevice device);

public:
	void initializeWith(VkInstance instance, VkSurfaceKHR surface);

	VkPhysicalDevice& get() noexcept;

	QueueFamilyIndices queueFamilies() noexcept;
	SwapChainSupportDetails swapChainSupport();
	std::vector<VkLayerProperties> layerProperties();
	std::vector<const char*> deviceExtensions() const noexcept;
};

