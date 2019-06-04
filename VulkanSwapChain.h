#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"

class VulkanSwapChain {
private:
	VkSwapchainKHR _swapChain = VkSwapchainKHR();
	VkFormat _swapChainImageFormat = VkFormat();
	VkExtent2D _swapChainExtent = VkExtent2D();
	std::vector<VkImage> _swapChainImages = {};

	VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR _chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

public:
	VkSwapchainKHR& get() noexcept;

	void initialize(VulkanPhysicalDevice physical, VulkanLogicalDevice logical, VkSurfaceKHR surface);
	void dispose(VulkanLogicalDevice logical) noexcept;
	std::vector<VkImage> imageList() noexcept;
	VkExtent2D& extent() noexcept;
	VkFormat& format() noexcept;
};

