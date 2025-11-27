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

     swapChainDesc_ = {};
     swapChainDesc_.BufferCount = 2;                        
     swapChainDesc_.Width = w;                              
     swapChainDesc_.Height = h;                             
     swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    
     swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
     swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    
     swapChainDesc_.SampleDesc.Count = 1;                          



  
     IDXGISwapChain1* tempSwapChain{};
     {
         const auto hr = dxgi.factory()->CreateSwapChainForHwnd(commandQueue.get(), window.handle(),
             &swapChainDesc_, nullptr, nullptr, &tempSwapChain);
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


 IDXGISwapChain3* swap_chain::get() const noexcept {
     if (!swapChain_) {
         assert(false && "スワップチェインが未作成です");
     }
     return swapChain_;
 }

 const DXGI_SWAP_CHAIN_DESC1& swap_chain::getDesc() const noexcept {
     if (!swapChain_) {
         assert(false && "スワップチェインが未作成です");
     }
     return swapChainDesc_;
 }
