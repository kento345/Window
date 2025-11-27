#pragma once
#include"device.h"
class descriptor_heap final
{
public:
	descriptor_heap() = default;
	~descriptor_heap();

	bool create(const device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false) noexcept;


	ID3D12DescriptorHeap* get() const noexcept;

	D3D12_DESCRIPTOR_HEAP_TYPE getType() const noexcept;

private:
	ID3D12DescriptorHeap* heap_{};
	D3D12_DESCRIPTOR_HEAP_TYPE type_{};
};

