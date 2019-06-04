#include "VulkanPhysicalDevice.h"
#include "Window.h"

int VulkanPhysicalDevice::_rateDeviceSuitability(const VkPhysicalDevice device) const noexcept {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	//Find for device features
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	int score = 0;
	// Discrete GPUs have a significant performance advantage
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	return score;
}

bool VulkanPhysicalDevice::_checkDeviceExtensionsSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool VulkanPhysicalDevice::_isDeviceSuitable(VkPhysicalDevice device) noexcept {
	QueueFamilyIndices indices = _listQueueFamilies(device);
	bool extensionsSupported = _checkDeviceExtensionsSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapchainDetails = _swapChainSupport(device);
		swapChainAdequate = !swapchainDetails.formats.empty() && !swapchainDetails.presentModes.empty();
	}

	return indices.isComplete() and extensionsSupported and swapChainAdequate;
}

QueueFamilyIndices VulkanPhysicalDevice::_listQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> _listQueueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, _listQueueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : _listQueueFamilies) {
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);

		if (queueFamily.queueCount > 0) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			if (presentSupport) {
				indices.presentFamily = i;
			}
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

SwapChainSupportDetails VulkanPhysicalDevice::_swapChainSupport(VkPhysicalDevice device) {
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

	if (formatCount not_eq 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

	if (presentModeCount not_eq 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}
void VulkanPhysicalDevice::initializeWith(VkInstance instance, VkSurfaceKHR surface) {
	_surface = surface;

	// Check if has a available device
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	// Get device list
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// Check better device
	int highScore = 0;
	for (const auto& device : devices) {
		int currentScore = _rateDeviceSuitability(device);
		if (currentScore > highScore) {
			if (_isDeviceSuitable(device)) {
				_physicalDevice = device;
			}
			highScore = currentScore;
		}
	}

	if (_physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("Unavailable suitable physical device");
	}
}

VkPhysicalDevice& VulkanPhysicalDevice::get() noexcept {
	return _physicalDevice;
}

QueueFamilyIndices VulkanPhysicalDevice::queueFamilies() noexcept {
	return _listQueueFamilies(_physicalDevice);
}

SwapChainSupportDetails VulkanPhysicalDevice::swapChainSupport() {
	return _swapChainSupport(_physicalDevice);
}

std::vector<VkLayerProperties> VulkanPhysicalDevice::layerProperties() {
	uint32_t layerCount = 0;
	vkEnumerateDeviceLayerProperties(_physicalDevice, &layerCount, nullptr);

	std::vector<VkLayerProperties> vectorOfLayerProperties(layerCount);
	vkEnumerateDeviceLayerProperties(_physicalDevice, &layerCount, vectorOfLayerProperties.data());

	return vectorOfLayerProperties;;
}

std::vector<const char*> VulkanPhysicalDevice::deviceExtensions() const noexcept {
	return _deviceExtensions;
}
