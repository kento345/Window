#include "swap_chain.h"
#include <cassert>


swap_chain::~swap_chain() {
    if (swapChain_) {
        swapChain_->Release();
        swapChain_ = nullptr;
    }
}


 bool swap_chain::create(const DXGI& dxgi, const window& window, const command_queue& commandQueue) noexcept {

     const auto [w, h] = window.size();

    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.BufferCount = 2;                          
    desc.Width = w;                                
    desc.Height = h;                               
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    
    desc.SampleDesc.Count = 1;                          

  
    IDXGISwapChain1* tempSwapChain{};
    {
        const auto hr = dxgi.factory()->CreateSwapChainForHwnd(commandQueue.get(), window.handle(),
            &desc, nullptr, nullptr, &tempSwapChain);
        if (FAILED(hr)) {
            assert(false && "スワップチェインの作成に失敗");
            return false;
        }
    }

    
    {
      
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain_));
      
        tempSwapChain->Release();

        if (FAILED(hr)) {
            assert(false && "スワップチェインのアップグレードに失敗");
            return false;
        }
    }

    return true;
}


[[nodiscard]] IDXGISwapChain* swap_chain::get() const noexcept {
    if (!swapChain_) {
        assert(false && "スワップチェインが未作成です");
        return nullptr;
    }
    return swapChain_;
}
