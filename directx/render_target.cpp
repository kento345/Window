#include "render_target.h"
#include<cassert>

render_target::~render_target() {
	for (auto& rt : renderTargets_) {
		if (rt) {
			rt->Release();
			rt = nullptr;
		}
	}
	renderTargets_.clear();
}

bool render_target::createBackBuffer(const device& device, const swap_chain& swap_chain, const descriptor_heap& heap)noexcept {
	const auto& desc = swap_chain.getDesc();

	renderTargets_.resize(desc.BufferCount);

	auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();

	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープがRTVではありません");

	for (uint8_t i = 0; i < desc.BufferCount; i++) {
		const auto hr = swap_chain.get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
		if (FAILED(hr)) {
			assert(false && "バックバッファの取得に失敗しました");
			return false;
		}

		device.get()->CreateRenderTargetView(renderTargets_[i], nullptr, handle);
		handle.ptr += device.get()->GetDescriptorHandleIncrementSize(heapType);
	}

	return true;
}

D3D12_CPU_DESCRIPTOR_HANDLE render_target::getDescriptorHandle(const device& device, const descriptor_heap& heap, UINT index) const noexcept {

	if (index >= renderTargets_.size() || !renderTargets_[index]) {
		assert(false && "不正なレンダーターゲットです");
	}

	auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();

	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプが RTV ではありません");

	handle.ptr += index * device.get()->GetDescriptorHandleIncrementSize(heapType);
	return handle;
}
//uint32_t
ID3D12Resource* render_target::get(UINT index)const noexcept {
	if (index >= renderTargets_.size() || !renderTargets_[index]) {
		assert(false && "不正なレンダーターゲットです");
		return nullptr;
	}
	return renderTargets_[index];
}