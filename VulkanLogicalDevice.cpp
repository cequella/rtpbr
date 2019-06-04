#include "VulkanLogicalDevice.h"

void VulkanLogicalDevice::initializeWith(VulkanPhysicalDevice physicalDevice) {
	QueueFamilyIndices indices = physicalDevice.queueFamilies();

	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	auto presentationQueue = _createPresentationQueue(physicalDevice);
	auto extensions = physicalDevice.deviceExtensions();

	VkPhysicalDeviceFeatures deviceFeatures = {};
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(presentationQueue.size());
	createInfo.pQueueCreateInfos = presentationQueue.data();

	VkResult result = vkCreateDevice(physicalDevice.get(), &createInfo, nullptr, &_logicalDevice);
	if (result not_eq VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device");
	}

	vkGetDeviceQueue(_logicalDevice, indices.presentFamily.value(), 0, &_queue);
}

std::vector<VkDeviceQueueCreateInfo> VulkanLogicalDevice::_createPresentationQueue(VulkanPhysicalDevice device) {
	QueueFamilyIndices indices = device.queueFamilies();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };


	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	return queueCreateInfos;
}

VkDevice& VulkanLogicalDevice::get() noexcept {
	return _logicalDevice;
}

VkQueue& VulkanLogicalDevice::queue() noexcept {
	return _queue;
}

void VulkanLogicalDevice::dispose() noexcept {
	vkDestroyDevice(_logicalDevice, nullptr);
}
