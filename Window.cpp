#include "Window.h"

std::unique_ptr<Window> Window::WINDOW_INSTANCE = std::unique_ptr<Window>(nullptr);

WindowSettings Window::settings() const noexcept {
	return _settings;
}

void Window::initializeWith(IScene& tScene) {
	_scene = &tScene;

	_startSDL2Subsystems();
	_createWindow();
	_instance.initializeWith(_window);
	_createSurface();
	_physicalDevice.initializeWith(_instance.get(), _surface);
	_logicalDevice.initializeWith(_physicalDevice);
	_swapChain.initialize(_physicalDevice, _logicalDevice, _surface);
	_imageView.initialize(_logicalDevice, _swapChain);
	_pipeline.initialize(_logicalDevice, _swapChain, 
		"C:\\Users\\Usuario\\source\\repos\\RTPBR\\vert.spv", 
		"C:\\Users\\Usuario\\source\\repos\\RTPBR\\frag.spv");
	_frameBuffer.initialize(_logicalDevice, _swapChain, _imageView, _pipeline);

	//------------------------------------------------------------------------------------
	VkCommandPool commandPool;
	QueueFamilyIndices queueFamilyIndices = _physicalDevice.queueFamilies();
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = 0; // Optional

	if (vkCreateCommandPool(_logicalDevice.get(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create command pool!");
	}

	std::vector<VkCommandBuffer> commandBuffers;
	commandBuffers.resize(_frameBuffer.get().size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(_logicalDevice.get(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers!");
	}

	auto extent = _swapChain.extent();
	for (size_t i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording command buffer!");
		}
	
		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _pipeline.renderPass();
		renderPassInfo.framebuffer = _frameBuffer.get()[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = extent;

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline.get());
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}
	}
	//------------------------------------------------------------------------------------
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	if (vkCreateSemaphore(_logicalDevice.get(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS or
		vkCreateSemaphore(_logicalDevice.get(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {
		throw std::runtime_error("failed to create semaphores!");
	}

	uint32_t imageIndex;
	vkAcquireNextImageKHR(_logicalDevice.get(), _swapChain.get(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(_logicalDevice.queue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	
	//TODO Must be in GraphicPipeline
	//renderPassInfo.dependencyCount = 1;
	//renderPassInfo.pDependencies = &dependency;
	
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	
	VkSwapchainKHR swapChains[] = { _swapChain.get() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	vkQueuePresentKHR(_logicalDevice.queue(), &presentInfo);
	//vkQueuePresentKHR(presentQueue, &presentInfo);
	//------------------------------------------------------------------------------------

	do {
		_scene->onUpdate();
	} while (_isRunning);

	vkDestroySemaphore(_logicalDevice.get(), renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(_logicalDevice.get(), imageAvailableSemaphore, nullptr);
	vkDestroyCommandPool(_logicalDevice.get(), commandPool, nullptr);

	_closeEverything();
}

void Window::close() noexcept {
	_isRunning = false;
}

void Window::_closeEverything() {
	_frameBuffer.dispose(_logicalDevice);
	_pipeline.dispose(_logicalDevice);
	_imageView.dispose(_logicalDevice);
	_swapChain.dispose(_logicalDevice);
	_logicalDevice.dispose();
	_instance.dispose();
	// FIXME vkDestroySurfaceKHR(_instance.get(), _surface, nullptr);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Window::_startSDL2Subsystems() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		throw std::runtime_error("SDL2 could not start subsystems");
	}
}

void Window::_createWindow() {
	_window = SDL_CreateWindow(
		_settings.title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_settings.width, _settings.height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
	if (not _window) {
		throw std::runtime_error("SDL2 could not create an window");
	}
}

void Window::_createSurface() {
	if (not SDL_Vulkan_CreateSurface(_window, _instance.get(), &_surface)) {
		throw std::runtime_error("SDL could not create Vulkan surface");
	}
}
