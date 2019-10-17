#include <iostream>
#include "VulkanFlash.h"

int main() {
	try {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			throw std::runtime_error("Could not initialize SDL.");
		}
		/*
		 *Initialize your own SDL Window, we need it for the instance extensions.
		 * The API supports resizable window.
		 */
		SDL_Window* window = SDL_CreateWindow("Vulkan Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
		if (window == NULL) {
			throw std::runtime_error("Could not create SDL window.");
		}


		/*A simple triangle with vertices, no depth*/
		const std::vector<Vertex> triangle = {
			{ { 0.0f, -0.5f,0.0f },{ 1.0f, 1.0f, 1.0f } },
		{ { 0.5f, 0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
		{ { -0.5f, 0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f } }
		};


		/*
		 *The A and Z. Whatever value you want to pass has to be on this struct.
		 * It will be rendered automatically
		 */
		VulkanFlashObject sample1Object;


		/*
		 *Call bind vertices to whatever you want to render
		 */
		sample1Object.bindVertices(triangle);

		/*
		 *Pass the shaders for the states
		 */
		sample1Object.fragmentShaderPath = "Shaders/sample1.frag";
		sample1Object.vertexShaderPath = "Shaders/sample1.vert";

		/*
		 *You have to disable the uniforms for the objects if you will not use them
		 */
		sample1Object.uniformsEnabled = false;

		/*
		 *Add the object to the API
		 */
		VulkanFlash::vulkanObjects.push_back(sample1Object);

		/*
		 *Initialize Vulkan! This handles everything, needs the SDL_Window
		 */
		VulkanFlash::vulkanStart(window);


		
		bool stillRunning = true;
		while (stillRunning) {

			SDL_Event event;
			while (SDL_PollEvent(&event)) {

				switch (event.type) {

				case SDL_QUIT:
					stillRunning = false;
					break;
				case SDL_WINDOWEVENT_RESIZED:
					VulkanFlash::recreateSwapChain();
					break;
				default:
					// Do nothing.
					break;
				}
			}
			/*
			 *Needs to be called to output to the surface.
			 * Handles the queues and the sychronization for you
			 */
			VulkanFlash::drawFrame();
			SDL_Delay(10);
		}
		vkDeviceWaitIdle(VulkanFlash::device);

	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	/*
	 *Always call it to clean up any objects
	 * If you created anything extra that's not inside vulkanObjects
	 * It's your responsibility to safelly delete them
	 */
	VulkanFlash::vulkanEnd();

}
