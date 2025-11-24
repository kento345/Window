#pragma once

#include"device.h"

class shader final
{
public:

	shader() = default;

	~shader();


	bool create(const device& device)noexcept;

	ID3DBlob* vertexShader() const noexcept;

	ID3DBlob* pixelShader() const noexcept;

private :
	ID3DBlob* vertexShader_{};
	ID3DBlob* pixelShader_{};
};

