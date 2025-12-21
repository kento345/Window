#include "window.h"
#include"input.h"

namespace {
    /*
    * @brief  ウィンドウプロシージャ
    * @param  handle ウィンドウハンドル
    * @param  msg　　メッセージ
    * @param  wParam メッセージパラメータ
    * @param  lParam メッセージパラメータ
    * @return 処理結果
    */
  
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_DESTROY:  // ウィンドウが閉じられたとき
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}  

/*
* @brief ウィンドウの生成
* @param instance インスタンスハンドル
* @param width    横幅
* @param heigth   縦幅
* @param name     ウィンドウ名
* @return 生成の成否
*/
HRESULT window::create(HINSTANCE instance, int width, int height, std::string_view name) noexcept {
    //ウィンドウの定義
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = name.data();
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

   //ウィンドウクラスの登録
    RegisterClass(&wc);

   //ウィンドウの作成
    handle_ = CreateWindow(wc.lpszClassName, wc.lpszClassName,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, instance, nullptr);
    if (!handle_) {
        return E_FAIL;
    }

    // ウインドウの表示
    ShowWindow(handle_, SW_SHOW);

    // ウィンドウを更新
    UpdateWindow(handle_);

    // ウィンドウのサイズを保存
    witdh_ = width;
    height_ = height;

    // 成功を返す
    return S_OK;
}


bool window::messageLoop() const noexcept {
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false; 
        }

        // メッセージ処理
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        static byte keyState[256]{};
        if (GetKeyboardState(keyState)) {
            input::instance().updateKeyState(keyState);
        }
    }

    return true;
}


[[nodiscard]] HWND window::handle() const noexcept {
    return handle_;
}

std::pair<int, int> window::size() const noexcept {
    return { witdh_, height_ };
}