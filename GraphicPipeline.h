#pragma once 

#include <fstream>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanLogicalDevice.h"
#include "VulkanSwapChain.h"

class GraphicPipeline {
private:
	VkPipelineLayout _pipelineLayout = VkPipelineLayout();
	VkRenderPass _renderPass = VkRenderPass();
	VkPipeline _graphicsPipeline = VkPipeline();

	std::vector<char> _readFile(const char* path);
	VkShaderModule _createShaderModule(VulkanLogicalDevice& logical, const std::vector<char>& code);
	void _createRenderPass(VulkanLogicalDevice& logical, VulkanSwapChain swapchain);

public:
	void initialize(VulkanLogicalDevice& logical, VulkanSwapChain swapchain, const char* vertPath, const char* fragPath);
	void dispose(VulkanLogicalDevice& logical) noexcept;
	
	VkRenderPass& renderPass() noexcept;
	VkPipeline& get() noexcept;
};
