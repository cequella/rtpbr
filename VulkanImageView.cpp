#include "VulkanImageView.h"

void VulkanImageView::initialize(VulkanLogicalDevice logical, VulkanSwapChain swapchain) {
	const auto swapchainImage = swapchain.imageList();
	const size_t swapchainImageSize = swapchainImage.size();
	_imageView.resize(swapchainImageSize);

	for (size_t i = 0; i < swapchainImageSize; i++) {
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapchainImage[i];
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(logical.get(), &createInfo, nullptr, &_imageView[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}

void VulkanImageView::dispose(VulkanLogicalDevice logical) noexcept {
	for (auto imageView : _imageView) {
		vkDestroyImageView(logical.get(), imageView, nullptr);
	}
}

std::vector<VkImageView>& VulkanImageView::get() noexcept {
	return _imageView;
}
