#include "device.h"
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")


device::~device() {
    
    if (device_) {
        device_->Release();
        device_ = nullptr;
    }
}


bool device::create(const DXGI& dxgi) noexcept {
   
    const auto hr = D3D12CreateDevice(dxgi.displayAdapter(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_));
    if (FAILED(hr)) {
        assert(false && "デバイス作成に失敗");
        return false;
    }

    return true;
}


ID3D12Device* device::get() const noexcept {
    if (!device_) {
        assert(false && "デバイスが未作成です");
    }

    return device_;
}