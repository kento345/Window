#include "window.h"


namespace {
    /*
    * @brief  ウィンドウプロシージャ
    * @param  handle
    * @param  msg
    * @param  wParam メッセージパラメータ
    * @param  lParam メッセージパラメータ
    * @return 処理結果
    */
  
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_DESTROY: 
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}  


[[nodiscard]] HRESULT window::create(HINSTANCE instance, int width, int height, std::string_view name) noexcept {
    
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = name.data();
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

   
    RegisterClass(&wc);

   
    handle_ = CreateWindow(wc.lpszClassName, wc.lpszClassName,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, instance, nullptr);
    if (!handle_) {
        return E_FAIL;
    }

    
    ShowWindow(handle_, SW_SHOW);

    
    UpdateWindow(handle_);

   
    witdh_ = width;
    height_ = height;

   
    return S_OK;
}


[[nodiscard]] bool window::messageLoop() const noexcept {
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;  
        }

        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}


[[nodiscard]] HWND window::handle() const noexcept {
    return handle_;
}

[[nodiscard]] std::pair<int, int> window::size() const noexcept {
    return { witdh_, height_ };
}