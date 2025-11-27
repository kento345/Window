#include "descriptor_heap.h"
#include<cassert>

descriptor_heap::~descriptor_heap() {
	if (heap_) {
		heap_->Release();
		heap_ = nullptr;
	}
}
bool descriptor_heap::create(const device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible) noexcept {
	
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = type;
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	type_ = type;

	HRESULT hr = device.get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));
	if (FAILED(hr)) {
		assert(false && "ディスクリプタヒープの生成に失敗しました");
		return false;
	}

	return true;
}

ID3D12DescriptorHeap* descriptor_heap::get() const noexcept {
	if (!heap_) {
		assert(false && "ディスクリプタヒープが未生成です");
	}

	return heap_;
}

D3D12_DESCRIPTOR_HEAP_TYPE descriptor_heap::getType() const noexcept {
	if(!heap_) {
		assert(false && "ディスクリプタヒープが未生成です");
	}
	return type_;
}