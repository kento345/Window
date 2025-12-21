#pragma once

#include"../directx/device.h"
#include"../directx/command_list.h"
#include <DirectXMath.h>

class quad_polygon
{
public:

	struct ConstBufferData
	{
		DirectX::XMMATRIX world_{}; 
		DirectX::XMFLOAT4 color_{};
	};

public:
	quad_polygon() = default;
	~quad_polygon();

	bool creat(const device& device) noexcept;

	void draw(const command_list& command_list)noexcept;

private:

	bool createVertexBuffer(const device& device)noexcept;

	bool createIndexBuffer(const device& device)noexcept;
private:
	ID3D12Resource* vertexBuffer_{};
	ID3D12Resource* indexBuffer_{};

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};
};

