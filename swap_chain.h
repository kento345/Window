#pragma once

#include "DXGI.h"
#include "command_queue.h"
#include "window.h"

class swap_chain final
{
public:
   
    swap_chain() = default;

  
    ~swap_chain();

   
   bool create(const DXGI& dxgi, const window& window, const command_queue& commandQueue) noexcept;

   
  IDXGISwapChain* get() const noexcept;

private:
    IDXGISwapChain3* swapChain_{};  
};

