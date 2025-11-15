#pragma once

#include <Windows.h>
#include <string>


class window final
{
public:
   
    window() = default;

  
    ~window() = default;

   
    [[nodiscard]] HRESULT create(HINSTANCE instance, int width, int height, std::string_view name) noexcept;

   
    [[nodiscard]] bool messageLoop() const noexcept;

    
    [[nodiscard]] HWND handle() const noexcept;

   
    [[nodiscard]] std::pair<int, int> size() const noexcept;


private:
    HWND handle_{}; 
    int  witdh_{};  
    int  height_{}; 
};

