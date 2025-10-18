// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>
#include <vk_descriptors.h>

struct DeletionQueue
{
    std::deque<std::function<void()>> deletors;

    void push_function(std::function<void()>&& function) {
        deletors.push_back(function);
    }

    void flush() {
        // reverse iterate the deletion queue to execute all the functions
        for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
            (*it)(); //call functors
        }

        deletors.clear();
    }
};

struct DeletionQueue
{
    std::deque<std::function<void()>> deletors;

    void push_function(std::function<void()>&& function)
    {
        deletors.push_back(function);
    }

    void flush()
    {
        for (auto it = deletors.rbegin(); it != deletors.rend(); it++)
        {
            (*it)();
        }

        deletors.clear();
    }
};

struct FrameData
{
    VkCommandPool _commandPool{nullptr};
    VkCommandBuffer _mainCommandBuffer{nullptr};

    VkSemaphore _swapchainSemaphore{nullptr}, _renderSemaphore{nullptr};
    VkFence _renderFence{nullptr};

    DeletionQueue _deletionQueue;
};

constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanEngine
{
public:
    bool       _isInitialized{false};
    int        _frameNumber{0};
    bool       stop_rendering{false};
    VkExtent2D _windowExtent{1700, 900};

    VkInstance               _instance{nullptr};
    VkDebugUtilsMessengerEXT _debug_messenger;
    VkPhysicalDevice         _chosenGPU{nullptr};
    VkDevice                 _device{nullptr};
    VkSurfaceKHR             _surface{nullptr};

    VmaAllocator _allocator;

    DeletionQueue _mainDeletionQueue;

    DescriptorAllocator globalDescriptorAllocator;

    VkDescriptorSet _drawImageDescriptors;
    VkDescriptorSetLayout _drawImageDescriptorLayout;

    VkPipeline _gradientPipeline;
    VkPipelineLayout _gradientPipelineLayout;

    AllocatedImage _drawImage;
    VkExtent2D _drawExtent;

    VkSwapchainKHR _swapchain{nullptr};
    VkFormat       _swapchainImageFormat;

    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _swapchainImageViews;
    VkExtent2D _swapchainExtent;

    struct AllocatedImage
    {
        VkImage image{nullptr};
        VkImageView imageView{nullptr};
        VmaAllocation allocation{nullptr};
        VkExtent3D imageExtent;
        VkFormat imageFormat;
    };

    AllocatedImage _drawImage{nullptr};
    VkExtent2D _drawExtent;

    VmaAllocator _allocator{nullptr};

    DeletionQueue _mainDeletionQueue;

    FrameData _frames[FRAME_OVERLAP];

    FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; }

    VkQueue _graphicsQueue{nullptr};
    uint32_t _graphicsQueueFamily;

    struct SDL_Window *_window{nullptr};

    static VulkanEngine &Get();

    //initializes everything in the engine
    void init();

    //shuts down the engine
    void cleanup();

    //draw loop
    void draw();

    void draw_background(VkCommandBuffer cmd);

    //run main loop
    void run();


private:
    void init_vulkan();

    void init_swapchain();

    void init_commands();

    void init_sync_structures();

    void init_descriptors();

    void init_pipelines();

    void init_background_pipelines();

    void create_swapchain(uint32_t width, uint32_t height);
    void destroy_swapchain();
};
