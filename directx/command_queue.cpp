#include "command_queue.h"
#include <cassert>



command_queue::~command_queue() {
    
    if (commandQueue_) {
        commandQueue_->Release();
        commandQueue_ = nullptr;
    }
}


[[nodiscard]] bool command_queue::create(const device& device) noexcept {
    
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;                                  


    const auto hr = device.get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue_));
    if (FAILED(hr)) {
        assert(false && "コマンドキューの作成に失敗");
        return false;
    }
    return true;
}


[[nodiscard]] ID3D12CommandQueue* command_queue::get() const noexcept {
    if (!commandQueue_) {
        assert(false && "コマンドキューが未作成です");
    }
    return commandQueue_;
}