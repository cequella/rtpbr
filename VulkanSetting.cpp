#include "VulkanSetting.h"

void VulkanSettings::initializeWith(SDL_Window* window) {
	_instance.initializeWith(window);
	_physicalDevice.initializeWith(_instance.get());
	_logicalDevice.initializeWith(_physicalDevice);
}

void VulkanSettings::dispose() {
	_logicalDevice.dispose();
	_instance.dispose();
}

std::vector<VkLayerProperties> VulkanSettings::layerProperties() {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> vectorOfLayerProperties(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, vectorOfLayerProperties.data());

	return vectorOfLayerProperties;
}

VulkanInstance& VulkanSettings::vkInstance() noexcept {
	return _instance;
}

VulkanPhysicalDevice& VulkanSettings::vkPhysicalDevice() noexcept {
	return _physicalDevice;
}

VulkanLogicalDevice& VulkanSettings::vkLogicalDevice() noexcept {
	return _logicalDevice;
}
