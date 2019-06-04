#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanImageView.h"
#include "GraphicPipeline.h"

class FrameBuffer {
private:
	std::vector<VkFramebuffer> _swapChainFramebuffers = {};

public:

	void initialize(VulkanLogicalDevice& logical, VulkanSwapChain swapchain, VulkanImageView imageview, GraphicPipeline pipeline);
	void dispose(VulkanLogicalDevice& logical) noexcept;

	std::vector<VkFramebuffer>& get() noexcept;
};

