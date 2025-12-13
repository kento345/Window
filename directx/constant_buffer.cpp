#include "constant_buffer.h"
#include<cassert>

constant_buffer::~constant_buffer()
{
	if (constantBuffer_) {
		constantBuffer_->Release();
		constantBuffer_ = nullptr;
	}
}


bool constant_buffer::create(const device& device, const descriptor_heap& heap, UINT bufferSize, UINT descriptorIndex) noexcept {

}
