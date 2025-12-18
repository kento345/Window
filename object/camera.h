#pragma once
#include<DirectXMath.h>

class camera
{
public:
	struct ConstBufferData
	{
		DirectX::XMMATRIX view_{};
		DirectX::XMMATRIX projection_{};
	};
public:
	camera() = default;
	~camera() = default;
public:
	void initialize() noexcept;

	void update()noexcept;

	DirectX::XMMATRIX XM_CALLCONV viewMatrix()const noexcept;
	DirectX::XMMATRIX XM_CALLCONV projection()const noexcept;

private:
	DirectX::XMMATRIX view_{};
	DirectX::XMMATRIX projection_{};

	DirectX::XMFLOAT3 position_{};
	DirectX::XMFLOAT3 target_{};
	DirectX::XMFLOAT3 up_{};
};

