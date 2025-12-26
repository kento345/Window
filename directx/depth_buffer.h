#pragma once

#include"device.h"
#include"descriptor_heap.h"
#include"../window/window.h"

class depth_buffer
{
public:
	depth_buffer() = default;
	~depth_buffer();

	bool create(const device& device, const descriptor_heap& heap, const window& window)noexcept;

	ID3D12Resource* dethBuffer()const noexcept;

	D3D12_CPU_DESCRIPTOR_HANDLE getCPUDescriptorHandle() const noexcept;

private:
	ID3D12Resource* depthBuffer_;
	D3D12_CPU_DESCRIPTOR_HANDLE handle_{};
};

