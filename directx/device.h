#pragma once

#include "../directx/DXGI.h"
#include <d3d12.h>

class device final
{
public:
  
    device() = default;

   
    ~device();

    
    [[nodiscard]] bool create(const DXGI& dxgi) noexcept;

   
    [[nodiscard]] ID3D12Device* get() const noexcept;


private:
    ID3D12Device* device_;  /// デバイス
};

