#include "VulkanPresentationQueue.h"

void VulkanPresentationQueue::initialize(VulkanPhysicalDevice device, VkSurfaceKHR surface) {
	QueueFamilyIndices indices = device.queueFamilies(surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		_infos.push_back(queueCreateInfo);
	}
}

VkQueue& VulkanPresentationQueue::queue() noexcept {
	return _queue;
}

std::vector<VkDeviceQueueCreateInfo> VulkanPresentationQueue::infos() const noexcept {
	return _infos;
}
