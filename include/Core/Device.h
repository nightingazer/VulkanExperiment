#pragma once

#include "VulkanMaster.h"
#include "Window.h"

// std lib headers
#include <string>
#include <vector>

namespace VKE {

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR        Capabilities;
    std::vector<VkSurfaceFormatKHR> Formats;
    std::vector<VkPresentModeKHR>   PresentModes;
  };

  struct QueueFamilyIndices {
    uint32_t GraphicsFamily;
    uint32_t PresentFamily;
    bool GraphicsFamilyHasValue = false;
    bool PresentFamilyHasValue  = false;
    bool IsComplete() { return GraphicsFamilyHasValue && PresentFamilyHasValue; }
  };

  class Device {
  public:
#if !defined(NDEBUG)
    const bool enableValidationLayers = false;
#else
    static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

    Device(Window& window);
    ~Device();

    // Not copyable or movable
    Device(const Device&) = delete;
    void operator=(const Device&) = delete;
    Device(Device&&) = delete;
    Device& operator=(Device&&) = delete;

    VkCommandPool GetCommandPool() { return m_CommandPool; }
    VkDevice DeviceHandle() { return m_Device; }
    VkSurfaceKHR Surface() { return m_Surface; }
    VkQueue GraphicsQueue() { return m_GraphicsQueue; }
    VkQueue PresentQueue() { return m_PresentQueue; }

    SwapChainSupportDetails GetSwapChainSupport() {
      return QuerySwapChainSupport(
          m_PhysicalDevice
      );
    }
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(m_PhysicalDevice); }
    VkFormat FindSupportedFormat(
        const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features
    );

    // Buffer Helper Functions
    void CreateBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer& buffer,
        VkDeviceMemory& bufferMemory
    );
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void CopyBufferToImage(
        VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount
    );

    void CreateImageWithInfo(
        const VkImageCreateInfo& imageInfo,
        VkMemoryPropertyFlags properties,
        VkImage& image,
        VkDeviceMemory& imageMemory
    );

    VkPhysicalDeviceProperties PhysicalDeviceProperties;

  private:
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateCommandPool();

    // helper functions
    bool IsDeviceSuitable(VkPhysicalDevice device);
    std::vector<const char*> GetRequiredExtensions();
    bool CheckValidationLayerSupport();
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void HasGflwRequiredInstanceExtensions();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

    VkInstance               m_Instance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;
    VkPhysicalDevice         m_PhysicalDevice = VK_NULL_HANDLE;
    Window& m_Window;
    VkCommandPool m_CommandPool;

    VkDevice     m_Device;
    VkSurfaceKHR m_Surface;
    VkQueue      m_GraphicsQueue;
    VkQueue      m_PresentQueue;

    const std::vector<const char*> m_ValidationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> m_DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  };

}  // namespace lve
