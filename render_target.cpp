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

}