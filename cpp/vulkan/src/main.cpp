#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

#include <optional>
#include <unordered_set>

#include <memory_resource>
#include <array>

// --------------------------------------------------------------------
	#define DEBUG
// --------------------------------------------------------------------
//	#define NO_MEM_RSRC
// --------------------------------------------------------------------
namespace app {

	const std::vector<const char*> validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef DEBUG
	bool constexpr enable_validation_layers = false;
#else
	bool constexpr enable_validation_layers = false;
#endif

	static
	GLFWwindow* window;
	
	static
	VkInstance instance;

	static
	VkDebugUtilsMessengerEXT debug_messenger;

	static
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;

	static
	VkDevice device;

	static
	VkQueue graphics_queue, present_queue;

	static
	VkSurfaceKHR surface;

	static
	VKAPI_ATTR VkBool32 VKAPI_CALL debug_call_back(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	static
	bool check_validation_layer_support() {
		std::array<char, (1<<10)*100> stack_mem;
		std::pmr::monotonic_buffer_resource rsrc{stack_mem.data(), stack_mem.size()};
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::pmr::vector<VkLayerProperties> available_layers(layerCount, &rsrc);
		vkEnumerateInstanceLayerProperties(&layerCount, available_layers.data());

		bool layer_found = false;
		for(const char* layer_name : validation_layers) {
			for(const auto& layer_properties : available_layers)
				if(std::strcmp(layer_name, layer_properties.layerName) == 0) {
					layer_found = true;
					break;
				}

			if(!layer_found)
				return false;
		}
		return true;
	}

	static
	std::vector<const char*> get_required_extensions() {
		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

		if(enable_validation_layers)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		
		return extensions;
	}

	static
	void create_instance() {
		if(enable_validation_layers && !check_validation_layer_support()) {
			throw std::runtime_error("Validation layers requested, but not available!");
		}

		VkApplicationInfo app_info {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = "Hello Triangle",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "No Engine",
			.engineVersion = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion = VK_API_VERSION_1_0
		};

		auto extensions = get_required_extensions();

		VkInstanceCreateInfo create_info{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &app_info,
			.enabledLayerCount = 0,
			.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data()
		};

		if(enable_validation_layers) {
			create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
			create_info.ppEnabledLayerNames = validation_layers.data();
		} else {
			create_info.enabledLayerCount = 0;
		}

		auto result = vkCreateInstance(&create_info, nullptr, &instance);
		if(result != VK_SUCCESS)
			throw std::runtime_error("failed to create instadebugCallbacknce!");

	}

	static
	void setup_debug_messenger() {
		if(!enable_validation_layers) 
			return;
		VkDebugUtilsMessengerCreateInfoEXT createInfo {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.messageSeverity  = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType      = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			.pfnUserCallback  = debug_call_back,
			.pUserData = nullptr // Optional
		};

		if (vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debug_messenger) != VK_SUCCESS)
			throw std::runtime_error("failed to set up debug messenger!");
	}

	struct QueueFamilyIndicies {
		std::optional<uint32_t> graphics_family;
		std::optional<uint32_t> present_family;

		bool is_complete() const {
			return graphics_family.has_value() && present_family.has_value();
		}
	};

	static
	QueueFamilyIndicies find_queue_families(VkPhysicalDevice const physical_device) { // assumes a surface is created
		QueueFamilyIndicies indicies;
		uint32_t queue_family_count;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());
		int i = 0;
		for(auto const& queue_family : queue_families) {
			if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indicies.graphics_family = i;
			}
			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);
			if(present_support)
				indicies.present_family = i;
			i++;
		}
		return indicies;
	}

	static
	void pick_physical_device() { // todo: pick best gpu instad of first usable one 
		std::array<char, (1<<10)*100> stack_mem;
		std::pmr::monotonic_buffer_resource rsrc{stack_mem.data(), stack_mem.size()};

		// get devices
		uint32_t device_count;
		vkEnumeratePhysicalDevices(instance, &device_count, nullptr); // leaks memory
		if(device_count == 0)
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		std::pmr::vector<VkPhysicalDevice> devices(device_count, &rsrc);
		vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

		// get first suitable device
		auto const is_suitable = [](VkPhysicalDevice const dev) -> auto {
			auto const check_extension_support = [](VkPhysicalDevice const dev) -> auto {
				return true;
			};
			VkPhysicalDeviceProperties device_properties;
			VkPhysicalDeviceFeatures device_features;
			vkGetPhysicalDeviceProperties(dev, &device_properties);
			vkGetPhysicalDeviceFeatures(dev, &device_features);

			QueueFamilyIndicies const indicies = find_queue_families(dev);

			return (device_properties.deviceType   == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ||
			        device_properties.deviceType   == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)  &&
			        device_features.geometryShader == true                                   &&
					indicies.is_complete() && check_extension_support(dev);
		};

		for (auto const& device : devices) {
			if (is_suitable(device)) {
				physical_device = device;
				break;
			}
    	}
		if (physical_device == VK_NULL_HANDLE)
			throw std::runtime_error("failed to find a suitable GPU!");
	}

	static
	void create_logical_device() {
		std::array<char, (1<<10)*(1<<10)> stack_mem;
		std::pmr::monotonic_buffer_resource rsrc{stack_mem.data(), stack_mem.size()};

		float const queue_priority = 1.0f;
		QueueFamilyIndicies const indicies = find_queue_families(physical_device);
		std::pmr::vector<VkDeviceQueueCreateInfo> queue_create_infos{&rsrc};
		std::pmr::unordered_set<uint32_t> unique_queue_families{&rsrc};
		unique_queue_families.insert(indicies.graphics_family.value());
		unique_queue_families.insert(indicies.present_family.value());

		for(auto const queue_family : unique_queue_families) {
			queue_create_infos.push_back({
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = queue_family,
				.queueCount = 1,
				.pQueuePriorities = &queue_priority
			});
		}
		VkPhysicalDeviceFeatures const device_features {

		};
		VkDeviceCreateInfo device_create_info {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
			.pQueueCreateInfos = queue_create_infos.data(),
			.pEnabledFeatures = &device_features
			
		};
		
		if(vkCreateDevice(physical_device, &device_create_info, nullptr, &device) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device!");

		vkGetDeviceQueue(device, indicies.graphics_family.value(), 0, &graphics_queue);
		vkGetDeviceQueue(device, indicies.present_family.value(),  0, &present_queue);
	}

	static
	void create_surface() {
		auto result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
		if(result != VK_SUCCESS)
			throw std::runtime_error("failed to create surface");
	}

	static
	void init_vulkan() {
		create_instance();      // none
		setup_debug_messenger();
		create_surface();
		pick_physical_device();
		create_logical_device();
	}

	static
	void main_loop() {
		while(!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void init() {
		glfwInit();                                   // leaks memory
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		auto const window_width = 800;
		auto const window_height = 600;
		auto const window_name = "Vulkan";
		window = glfwCreateWindow(window_width, window_height, window_name, nullptr, nullptr);

		init_vulkan();
	}

	void cleanup() {
		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyDevice(device, nullptr);
		vkDestroyInstance(instance, nullptr);
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void run() {
		init();
		main_loop();
		cleanup();
	}
}

int main() {
	app::run();
	return 0;
}