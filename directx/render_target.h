#pragma once

#include"device.h"
#include"swap_chain.h"
#include"descriptor_heap.h"
#include<vector>

class render_target final
{
public:
	render_target() = default;

	~render_target();

	bool createBackBuffer(const device& device, const swap_chain& swapc_hain, const descriptor_heap& heap)noexcept;

	D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(const device& deice, const descriptor_heap& heap, UINT index) const noexcept;

	ID3D12Resource* get(UINT index) const noexcept;

private:
	std::vector<ID3D12Resource*> renderTargets_;
};

