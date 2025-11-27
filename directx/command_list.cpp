#include "command_list.h"
#include<cassert>

command_list::~command_list() {
	if (commandList_) {
		commandList_->Release();
		commandList_ = nullptr;
	}
}

bool command_list::creat(const device& device, const command_allocator& command_allocator)noexcept {
	const auto hr = device.get()->CreateCommandList(0, command_allocator.getType(), command_allocator.get(), nullptr, IID_PPV_ARGS(&commandList_));
	if (FAILED(hr)) {
		assert(false && "コマンドリストの作成に失敗しました");
		return false;
	}

	commandList_->Close();
	return true;
}

void command_list::reset(const command_allocator& command_allocator)noexcept {
	if (!commandList_) {
		assert(false && "コマンドリストが未作成です");
	}

	commandList_->Reset(command_allocator.get(), nullptr);
}

ID3D12GraphicsCommandList* command_list::get() const noexcept {
	if (!commandList_) {
		assert(false && "コマンドリストが未作成です");
	}
	return commandList_;
}