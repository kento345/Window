
#include "../window/window.h"

#include "../directx/device.h"
#include "../directx/DXGI.h"
#include"../directx/command_allocator.h"
#include"../directx/command_list.h"
#include"../directx/command_queue.h"
#include"../directx/swap_chain.h"
#include"../directx/descriptor_heap.h"
#include"../directx/render_target.h"
#include"../directx/fence.h"
#include"../directx/root_signature.h"
#include"../directx/shader.h"
#include"../directx/pipline_state_object.h"

#include"../draw_resource/triangle_polygon.h"

#include <cassert>

/*namespace {
    constexpr std::string_view APP_NAME = "MyApp";  
} */ 
class Application final {
public:
   
    Application() = default;

   
    ~Application() = default;

    
    [[nodiscard]] bool initialize(HINSTANCE instance) noexcept {
       
       
        if (S_OK != windowInstance_.create(instance, 1280, 720, "MyApp")) {
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


        if (!descriptorHeapInstance_.create(deviceInstance_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapChainInstance_.getDesc().BufferCount)) {
            assert(false && "ディスクリプタヒープの作成に失敗しました");
            return false;
        }

        if (!renderTargetInstance_.createBackBuffer(deviceInstance_, swapChainInstance_, descriptorHeapInstance_)) {
            assert(false && "レンダーターゲットの作成に失敗しました");
            return false;
        }

        if (!commandAllocatorInstance_[0].create(deviceInstance_, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }
        if (!commandAllocatorInstance_[1].create(deviceInstance_, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }

        if (!commandListInstance_.creat(deviceInstance_, commandAllocatorInstance_[0])) {
            assert(false && "コマンドリストの作成に失敗しました");
            return false;
        }

        if (!fenceInstance_.create(deviceInstance_)) {
            assert(false && "フェンスの作成に失敗しました");
            return false;
        }

      
        if (!trianglePolygonInstance_.creat(deviceInstance_)) {
            assert(false && "三角形ポリゴンの作成に失敗しました");
            return false;
        }

        if (!rootSignatureInstance_.create(deviceInstance_)) {
            assert(false && "ルートシグネチャの作成に失敗しました");
            return false;
        }

        if (!shaderInstance_.create(deviceInstance_)) {
            assert(false && "シェーダーの作成に失敗しました");
            return false;
        }

        if (!piplineStateObjectInstance_.create(deviceInstance_, shaderInstance_, rootSignatureInstance_)) {
            assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
            return false;
        }

        return true;
    }

  
    void loop() noexcept {
        while (windowInstance_.messageLoop()) {
            // 現在のバックバッファインデックスを取得
            const auto backBufferIndex = swapChainInstance_.get()->GetCurrentBackBufferIndex();

            // 以前のフレームの GPU の処理が完了しているか確認して待機する
            if (frameFenceValue_[backBufferIndex] != 0) {
                fenceInstance_.wait(frameFenceValue_[backBufferIndex]);
            }

            // コマンドアロケータリセット
            commandAllocatorInstance_[backBufferIndex].reset();
            // コマンドリストリセット
            commandListInstance_.reset(commandAllocatorInstance_[backBufferIndex]);

            // リソースバリアでレンダーターゲットを Present から RenderTarget へ変更
            auto pToRT = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
            commandListInstance_.get()->ResourceBarrier(1, &pToRT);

            // レンダーターゲットの設定
            D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTargetInstance_.getDescriptorHandle(deviceInstance_, descriptorHeapInstance_, backBufferIndex) };
            commandListInstance_.get()->OMSetRenderTargets(1, handles, false, nullptr);

            // レンダーターゲットのクリア
            const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // クリア
            commandListInstance_.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);


            // パイプラインステートの設定
            commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());
            // ルートシグネチャの設定
            commandListInstance_.get()->SetGraphicsRootSignature(rootSignatureInstance_.get());

            // ビューポートの設定
            const auto [w, h] = windowInstance_.size();
            D3D12_VIEWPORT viewport{};
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;
            viewport.Width = static_cast<float>(w);
            viewport.Height = static_cast<float>(h);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            commandListInstance_.get()->RSSetViewports(1, &viewport);

            // シザー矩形の設定
            D3D12_RECT scissorRect{};
            scissorRect.left = 0;
            scissorRect.top = 0;
            scissorRect.right = w;
            scissorRect.bottom = h;
            commandListInstance_.get()->RSSetScissorRects(1, &scissorRect);

            // ポリゴンの描画
            trianglePolygonInstance_.draw(commandListInstance_);


            // リソースバリアでレンダーターゲットを RenderTarget から Present へ変更
            auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
            commandListInstance_.get()->ResourceBarrier(1, &rtToP);

            // コマンドリストをクローズ
            commandListInstance_.get()->Close();

            // コマンドキューにコマンドリストを送信
            ID3D12CommandList* ppCommandLists[] = { commandListInstance_.get() };
            commandQueueInstance_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

            // プレゼント
            swapChainInstance_.get()->Present(1, 0);

            // フェンスにフェンス値を設定
            commandQueueInstance_.get()->Signal(fenceInstance_.get(), nextFenceValue_);
            frameFenceValue_[backBufferIndex] = nextFenceValue_;
            nextFenceValue_++;

        }

       
    }


    D3D12_RESOURCE_BARRIER resourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
        D3D12_RESOURCE_BARRIER barrier{};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = resource;
        barrier.Transition.StateBefore = from;
        barrier.Transition.StateAfter = to;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        return barrier;
    }

private:
    window       windowInstance_{};       
    DXGI         dxgiInstance_{};         
    device       deviceInstance_{};       
    command_queue commandQueueInstance_{};
    swap_chain    swapChainInstance_{};   
    descriptor_heap descriptorHeapInstance_{};
    render_target renderTargetInstance_{};
    command_allocator commandAllocatorInstance_[2]{};
    command_list commandListInstance_{};

    fence fenceInstance_{};
    UINT64 frameFenceValue_[2]{};
    UINT64 nextFenceValue_ = 1;

    root_signature rootSignatureInstance_{};
    shader shaderInstance_{};
    pipline_state_object piplineStateObjectInstance_{};
    triangle_polygon trianglePolygonInstance_{};
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
   
    Application app;

    if (!app.initialize(hInstance)) {
        assert(false && "アプリケーションの初期化に失敗しました");
    }

  
    app.loop();

    return 0;
}