#pragma once

#include<DirectXMath.h>

class object
{
public:
	struct ConstBufferData
	{
		DirectX::XMMATRIX world_{};
		DirectX::XMFLOAT4 color_{};
	};
public:
	object() = default;

	~object() = default;

	void update()noexcept;

	DirectX::XMMATRIX world()const noexcept;

	DirectX::XMFLOAT4 color()const noexcept;

private:
	DirectX::XMMATRIX world_ = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4 color_ = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	float move_{};
};

