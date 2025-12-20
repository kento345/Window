#include "object.h"
#include<cmath>

void object::initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color)noexcept {
	world_ = DirectX::XMMatrixTranslation(pos.x,pos.y, pos.z);

	color_ = color;
}
//void object::update()noexcept {
//	move_ += 0.02f;
//	world_ = DirectX::XMMatrixTranslation(0.0f, std::sinf(move_) * 1.5f, 0.0f);
//
//	color_ = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//}
void object::update() noexcept {

}

DirectX::XMMATRIX object::world()const noexcept {
	return world_;
}

DirectX::XMFLOAT4 object::color()const noexcept {
	return color_;
}