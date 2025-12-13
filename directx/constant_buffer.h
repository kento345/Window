#pragma once

#include"device.h"
#include"descriptor_heap.h"


class constant_buffer
{
public:
	constant_buffer() = default;

	~constant_buffer();

	bool create(const device& device, const descriptor_heap& heap, UINT bufferSize, UINT descriptorIndex) noexcept;
	
	ID3D12Resource* constanceBuffer() const noexcept;

	D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle() const noexcept;

private:
	ID3D12Resource* constantBuffer_{};
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_{};
};

