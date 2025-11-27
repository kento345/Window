#pragma once

#include <Windows.h>
#include <string>

//ウィンドウ制御クラス
class window final
{
public:
    //コンストラクタ
    window() = default;

    // デストラクタ
    ~window() = default;

    /*
    * @brief           ウィンドウの生成 
    * @param instance  インスタンスハンドル
    * @param width     横幅
    * @param heigtht   縦幅
    * @param name      ウィンドウ名
    * @return          生成の成否
    */
    HRESULT create(HINSTANCE instance, int width, int height, std::string_view name) noexcept;

    //@brief メッセージループ
    bool messageLoop() const noexcept;

    //@brief  ウィンドウハンドルを取得
    //@return ウィンドウハンドル
    HWND handle() const noexcept;

    //@brief  ウィンドウのサイズを取得
    //@return ウィンドウのサイズ(横幅,縦幅)
    std::pair<int, int> size() const noexcept;


private:
    HWND handle_{};  /// ウィンドウハンドル
    int  witdh_{};   /// ウィンドウ横幅
    int  height_{};  /// ウィンドウ縦幅
};

