#include "FrameBuffer.h"

void FrameBuffer::initialize(VulkanLogicalDevice& logical, VulkanSwapChain swapchain, VulkanImageView imageview, GraphicPipeline pipeline) {
	auto swapchainImage = imageview.get();
	auto swapchainImageSize = swapchainImage.size();
	_swapChainFramebuffers.resize(swapchainImageSize);

	for (size_t i = 0; i < swapchainImageSize; i++) {
		VkImageView attachments[] = {
			swapchainImage[i]
		};

		auto extent = swapchain.extent();

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = pipeline.renderPass();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(logical.get(), &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void FrameBuffer::dispose(VulkanLogicalDevice& logical) noexcept {
	for (auto framebuffer : _swapChainFramebuffers) {
		vkDestroyFramebuffer(logical.get(), framebuffer, nullptr);
	}
}

std::vector<VkFramebuffer>& FrameBuffer::get() noexcept {
	return _swapChainFramebuffers;
}
