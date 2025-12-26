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
	const auto size = (sizeof(bufferSize) + 255) & ~255;

	D3D12_HEAP_PROPERTIES heapPros{};
	heapPros.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resouceDesc{};
	resouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resouceDesc.Width  = size;
	resouceDesc.Height = 1;
	resouceDesc.DepthOrArraySize = 1;
	resouceDesc.MipLevels = 1;
	resouceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resouceDesc.SampleDesc.Count = 1;
	resouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	const auto res = device.get()->CreateCommittedResource(
		&heapPros,
		D3D12_HEAP_FLAG_NONE,
		&resouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constantBuffer_));
	if (FAILED(res))
	{
		assert(false && "コンスタントバッファの作成に失敗しました");
		false;
	}

	auto heapType = heap.getType();
	if (heapType != D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) {
		assert(false && "ディスクリプタヒープのタイプが CBV_SRV_UAV ではありません");
		false;
	}

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constantBuffer_->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = size;

	UINT cbvDescriptorSize = device.get()->GetDescriptorHandleIncrementSize(heap.getType());

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap.get()->GetCPUDescriptorHandleForHeapStart();

	cpuHandle.ptr += descriptorIndex * cbvDescriptorSize;

	device.get()->CreateConstantBufferView(&cbvDesc, cpuHandle);

	gpuHandle_ = heap.get()->GetGPUDescriptorHandleForHeapStart();

	gpuHandle_.ptr += descriptorIndex * cbvDescriptorSize;

	return true;
}

ID3D12Resource* constant_buffer::constanceBuffer()const noexcept {
	assert(constantBuffer_ && "コンスタントバッファが未作成です");
	return constantBuffer_;
}

D3D12_GPU_DESCRIPTOR_HANDLE constant_buffer::getGpuDescriptorHandle()const noexcept {
	assert(constantBuffer_ && "コンスタントバッファが未作成です");
	return gpuHandle_;
}
