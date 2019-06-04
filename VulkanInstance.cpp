#include "VulkanInstance.h"

bool VulkanInstance::_checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : _validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

void VulkanInstance::initializeWith(SDL_Window* window) {
	if (_enableValidationLayers and not _checkValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	appInfo.pApplicationName = "RTPBR";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Utopia";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	unsigned int count;
	SDL_bool success = SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr);
	if (not success) {
		throw std::runtime_error("SDL could not get window instance extensions amount");
	}

	// HERE
	std::vector<const char*> extensions = {
		//Mandatory
	};
	size_t additional_extension_count = extensions.size();
	extensions.resize(additional_extension_count + count);
	// HERE

	success = SDL_Vulkan_GetInstanceExtensions(window, &count, extensions.data() + additional_extension_count);
	if (not success) {
		throw std::runtime_error("SDL could not get window instance extensions names");
	}

	std::vector<const char*> layerNames{};

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = static_cast<uint32_t>(layerNames.size());
	createInfo.ppEnabledLayerNames = layerNames.data();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	if (_enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
		createInfo.ppEnabledLayerNames = _validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &_vkInstance);
	if (result not_eq VK_SUCCESS) {
		throw std::runtime_error("Failed to create Vulkan instance");
	}
}

VkInstance& VulkanInstance::get() noexcept {
	return _vkInstance;
}

void VulkanInstance::dispose() noexcept {
	vkDestroyInstance(_vkInstance, nullptr);
}
