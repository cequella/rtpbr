#include "VulkanSwapChain.h"

VkSwapchainKHR& VulkanSwapChain::get() noexcept {
	return _swapChain;
}

void VulkanSwapChain::initialize(VulkanPhysicalDevice physical, VulkanLogicalDevice logical, VkSurfaceKHR surface) {
	SwapChainSupportDetails swapChainSupport = physical.swapChainSupport();

	VkSurfaceFormatKHR surfaceFormat = _chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = _chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = _chooseSwapExtent(swapChainSupport.capabilities);

	const int MINIMUM_IMAGE_COUNT = 1;
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + MINIMUM_IMAGE_COUNT;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	QueueFamilyIndices indices = physical.queueFamilies();
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	VkResult result = vkCreateSwapchainKHR(logical.get(), &createInfo, nullptr, &_swapChain);
	if (result not_eq VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	// Crete swapchain image list
	vkGetSwapchainImagesKHR(logical.get(), _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logical.get(), _swapChain, &imageCount, _swapChainImages.data());
	
	// Saving infos
	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = extent;
}

void VulkanSwapChain::dispose(VulkanLogicalDevice logical) noexcept {
	vkDestroySwapchainKHR(logical.get(), _swapChain, nullptr);
}

std::vector<VkImage> VulkanSwapChain::imageList() noexcept {
	return _swapChainImages;
}

VkExtent2D& VulkanSwapChain::extent() noexcept {
	return _swapChainExtent;
}

VkFormat& VulkanSwapChain::format() noexcept {
	return _swapChainImageFormat;
}

VkSurfaceFormatKHR VulkanSwapChain::_chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	if (availableFormats.size() == 1 and availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::_chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	// TODO See if it explictly enable VSync
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		} else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

VkExtent2D VulkanSwapChain::_chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { 800, 600 };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}