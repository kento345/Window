#pragma once
#include "device.h"
class command_allocator final
{
public:
	command_allocator() = default;
	~command_allocator();

	bool create(const device& device, const D3D12_COMMAND_LIST_TYPE type) noexcept;

	void reset() noexcept;

	ID3D12CommandAllocator* get() const noexcept;

	D3D12_COMMAND_LIST_TYPE getType() const noexcept;


private:
	ID3D12CommandAllocator* commandAllocator_{};
	D3D12_COMMAND_LIST_TYPE type_{};
};

