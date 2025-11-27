#pragma once

#include "DXGI.h"
#include "command_queue.h"
#include "../window/window.h"

class swap_chain final
{
public:
   
    swap_chain() = default;

  
    ~swap_chain();

   
   bool create(const DXGI& dxgi, const window& window, const command_queue& commandQueue) noexcept;

   
   IDXGISwapChain3* get() const noexcept;

  const DXGI_SWAP_CHAIN_DESC1& getDesc() const noexcept;

private:
    IDXGISwapChain3* swapChain_{};
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
};

