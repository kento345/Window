#pragma once

#include"device.h"


class fence final	
{
public:
	fence() = default;
	~fence();

	bool create(const device& device)noexcept;

	void wait(UINT64 fenceValue) const noexcept;

	ID3D12Fence* get() const noexcept;

private:
	ID3D12Fence* fence_{};
	HANDLE waitGpuEvent_{};
};

