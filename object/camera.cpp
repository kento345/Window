#include "camera.h"
#include<cmath>

namespace {
	constexpr float eyeMoveSpeed_ = 0.06f;
	constexpr float destTargetToView_ = -5.0f;
}

void camera::initialize() noexcept {
	position_ = DirectX::XMFLOAT3(0.0f, 0.0f, destTargetToView_);

	target_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	up_ = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	projection_ = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XM_PIDIV4,
		1280.0f / 720.0f,
		0.1f,
		100.0f
	);
}

void camera::update() noexcept {
	static float angle = 0.0f;
	angle += eyeMoveSpeed_;
	position_.x = destTargetToView_ * std::sinf(angle);
	position_.z = destTargetToView_ * std::cosf(angle);

	view_ = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&position_),
		DirectX::XMLoadFloat3(&target_),
		DirectX::XMLoadFloat3(&up_));
}

DirectX::XMMATRIX XM_CALLCONV camera::viewMatrix() const noexcept{
	return view_;
}

DirectX::XMMATRIX XM_CALLCONV camera::projection()const noexcept {
	return projection_;
}
