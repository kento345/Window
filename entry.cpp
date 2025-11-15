
#include "window.h"



#include <cassert>
#include <dxgi.h>
#include "DXGI.h"
#include "device.h"
#include "command_queue.h"
#include "swap_chain.h"



namespace {
    constexpr std::string_view APP_NAME = "MyApp";  
}  
class Application final {
public:
   
    Application() = default;

   
    ~Application() = default;

    
    [[nodiscard]] bool initialize(HINSTANCE instance) noexcept {
       
        if (S_OK != windowInstance_.create(instance, 1280, 720, APP_NAME)) {
            assert(false && "ウィンドウの生成に失敗しました");
            return false;
        }

       
        if (!dxgiInstance_.setDisplayAdapter()) {
            assert(false && "DXGIのアダプタ設定に失敗しました");
            return false;
        }

       
        if (!deviceInstance_.create(dxgiInstance_)) {
            assert(false && "デバイスの作成に失敗しました");
            return false;
        }

       
        if (!commandQueueInstance_.create(deviceInstance_)) {
            assert(false && "コマンドキューの作成に失敗しました");
            return false;
        }

       
        if (!swapChainInstance_.create(dxgiInstance_, windowInstance_, commandQueueInstance_)) {
            assert(false && "スワップチェインの作成に失敗しました");
            return false;
        }

        return true;
    }

  
    void loop() noexcept {
        while (windowInstance_.messageLoop()) {
           

           
        }

       
    }

private:
    window       windowInstance_{};       
    DXGI         dxgiInstance_{};         
    device       deviceInstance_{};       
    command_queue commandQueueInstance_{};
    swap_chain    swapChainInstance_{};   
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
   
    Application app;

    if (!app.initialize(hInstance)) {
        assert(false && "アプリケーションの初期化に失敗しました");
    }

    
    app.loop();

    return 0;