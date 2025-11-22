#include "triangle_polygon.h"
#include <cassert>
#include<DirectXMath.h>

namespace {
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}
triangle_polygon::~triangle_polygon() {
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
	if (indexBuffer_) {
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
}

bool triangle_polygon::creat(const device& device)noexcept {
	if (!createVertexBuffer(device)) {
		return false;
	}
	if (!createIndexBuffer(device)) {
		return false;
	}
	return true;
}

bool triangle_polygon::createVertexBuffer(const device& device)noexcept {
	Vertex triangleVertices[] = {
		{{0.0f,0.5f,0.0f},{1.0f,0.0f,0.0f,1.0f}},
		{{0.5f,-0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f}},
		{{-0.5f,-0.5f,0.0f},{0.0f,0.0f,1.0f,1.0f}}
	};

	const auto vertexBufferSize = sizeof(triangleVertices);

	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resorceDesc{};
	resorceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resorceDesc.Alignment = 0;
	resorceDesc.Width = vertexBufferSize;
	resorceDesc.Height = 1;
	resorceDesc.DepthOrArraySize = 1;
	resorceDesc.MipLevels = 1;
	resorceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resorceDesc.SampleDesc.Count = 1;
	resorceDesc.SampleDesc.Quality = 0;
	resorceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resorceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto res = device.get()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resorceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer_));
	if (FAILED(res)) {
		assert(false && "頂点バッファの作成に失敗");
		return false;
	}

	Vertex* data{};

	res = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**> (&data));
	if (FAILED(res)) {
		assert(false && "頂点バッファのマップに失敗");
		return false;
	}

	memcpy_s(data, vertexBufferSize, triangleVertices, vertexBufferSize);

	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = vertexBufferSize;
	vertexBufferView_.StrideInBytes = sizeof(Vertex);

	return true;
}

bool triangle_polygon::createIndexBuffer(const device& device)noexcept {
	uint16_t triangleIndices[] = {
		0,1,2
	};

	const auto indexBufferSize = sizeof(triangleIndices);

	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
}
