#include "depth_buffer.h"
#include<cassert>

depth_buffer::~depth_buffer()
{
	if (dethBuffer_) {
		dethBuffer_->Release();
		dethBuffer_ = nullptr;
	}
}



bool depth_buffer::create(const device& device, const descriptor_heap& heap,const window& window)noexcept {

	const auto [w, h] = window.size();

	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC depthDesc{};
	depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
}