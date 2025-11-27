#pragma once

#include "DXGI.h"
#include <d3d12.h>
//#include <d3d12.h>

class device final
{
public:
  
    device() = default;

   
    ~device();

    
   bool create(const DXGI& dxgi) noexcept;

   
   ID3D12Device* get() const noexcept;


private:
    ID3D12Device* device_;  /// デバイス
};

