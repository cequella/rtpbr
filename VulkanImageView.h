#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanSwapChain.h"

class VulkanImageView {
private:
	std::vector<VkImageView> _imageView = {};

public:
	void initialize(VulkanLogicalDevice logical, VulkanSwapChain swapchain);
	void dispose(VulkanLogicalDevice logical) noexcept;

	std::vector<VkImageView>& get() noexcept;
};

