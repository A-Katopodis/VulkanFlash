#include <iostream>
#include "VulkanFlash.h"

static void updateUBO();

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


		/*
		*The A and Z. Whatever value you want to pass has to be on this struct.
		* It will be rendered automatically
		*/
		VulkanFlashObject sample1Object;


		const std::vector<Vertex> squareSun ={
			{ { -0.1f, -0.1f, 0.0f },{ 1.0f, 1.0f, 0.0f } },
		{ { 0.1f, -0.1f, 0.0f },{ 1.0f, 1.0f, 0.0f } },
		{ { 0.1f, 0.1f, 0.0f },{ 1.0f, 1.0f, 0.0f } },
		{ { -0.1f, 0.1f, 0.0f },{ 1.0f, 1.0f, 0.0f } }
		};

		const std::vector<uint16_t> indicesSun = {
			0, 1, 2, 2, 3, 0
		};



		const std::vector<Vertex> earth = {
			{ { -0.1f, -0.1f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
		{ { 0.1f, -0.1f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
		{ { 0.1f, 0.1f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
		{ { -0.1f, 0.1f, 0.0f },{ 0.0f, 0.0f, 1.0f } }
		};

		const std::vector<uint16_t> indicesEarth = {
			0, 1, 2, 2, 3, 0
		};


		const std::vector<Vertex> moon = {
			{ { -0.1f, -0.1f, 0.0f },{ 0.5f, 0.5f, 0.5f } },
		{ { 0.1f, -0.1f, 0.0f },{ 0.5f, 0.5f, 0.5f } },
		{ { 0.1f, 0.1f, 0.0f },{ 0.5f, 0.5f, 0.5f } },
		{ { -0.1f, 0.1f, 0.0f },{ 0.5f, 0.5f, 0.5f } }
		};

		const std::vector<uint16_t> moonIndices = {
			0, 1, 2, 2, 3, 0
		};


		sample1Object.bindVertices(squareSun);
		sample1Object.bindIndices(indicesSun);

		sample1Object.bindVertices(earth);
		sample1Object.bindIndices(indicesEarth);

		sample1Object.bindVertices(moon);
		sample1Object.bindIndices(moonIndices);

		/*A simple triangle with vertices, no depth*/


		/*
		*Call bind vertices to whatever you want to render
		*/
		/*
		*Pass the shaders for the states
		*/
		sample1Object.fragmentShaderPath = "Shaders/shaderOnlyUniform.frag";
		sample1Object.vertexShaderPath = "Shaders/shaderOnlyUniform.vert";

		/*
		*You have to disable the uniforms for the objects if you will not use them
		*/

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
			updateUBO();
			VulkanFlash::updateVulkanObjectsUniforms();
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
static void updateUBO()
{
	/*They need reference otherwise it explodes! BOOM*/
	UniformBufferObject& obj1 = VulkanFlash::vulkanObjects.at(0).uniformHandles.at(0);
	UniformBufferObject& obj2 = VulkanFlash::vulkanObjects.at(0).uniformHandles.at(1);
	UniformBufferObject& obj3 = VulkanFlash::vulkanObjects.at(0).uniformHandles.at(2);

	static auto startTime = std::chrono::high_resolution_clock::now();


	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();


	obj1.model = glm::scale(glm::mat4(1.0), glm::vec3(3));

	obj1.view = glm::mat4(1.f);

	obj1.proj = glm::ortho(-2.f, 2.f, -2.f, 2.f);


	obj2.model = 		// rotate the object object around (0,0,1) axis
		glm::rotate(glm::mat4(1.0), time, glm::vec3(0, 0, 1)) *
		// translate it to the right so we can define a radius (remember rotation and scaling happens around (0,0,0))!!!!!
		glm::translate(glm::mat4(1.f), glm::vec3(1, 0, 0)) *
		// scale the cube to make it smaller
		glm::scale(glm::mat4(1.0), glm::vec3(1));


	obj2.view = glm::mat4(1.f);

	obj2.proj = glm::ortho(-2.f, 2.f, -2.f, 2.f);


	obj3.model = 		
						// put it to rotate around the center of earth
		glm::rotate(glm::mat4(1.0), time, glm::vec3(0, 0, 1)) * glm::translate(glm::mat4(1.f), glm::vec3(1, 0, 0)) *
		// rotate around (0,0,0) with radius 0.25
		glm::rotate(glm::mat4(1.0), 3.f * time, glm::vec3(0, 0, 1)) * glm::translate(glm::mat4(1.f), glm::vec3(0.25, 0, 0)) *
		// scale to make it smaller
		glm::scale(glm::mat4(1.0), glm::vec3(0.5));


	obj3.view = glm::mat4(1.f);

	obj3.proj = glm::ortho(-2.f, 2.f, -2.f, 2.f);

}
