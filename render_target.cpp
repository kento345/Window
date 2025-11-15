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