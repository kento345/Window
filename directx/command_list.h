#pragma once

#include"../directx/device.h"
#include "../directx/command_allocator.h"
class command_list final
{
public:
	command_list() = default;
	~command_list();

	bool creat(const device& device, const command_allocator& command_Allocator) noexcept;

	void reset(const command_allocator& command_Allocator) noexcept;

	ID3D12GraphicsCommandList* get() const noexcept;

private :
	ID3D12GraphicsCommandList* commandList_{};
};

