
#include "../window/window.h"
#include"../window/input.h"

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
#include"../directx/constant_buffer.h"
#include"../directx/depth_buffer.h"

#include"../draw_resource/triangle_polygon.h"
#include"../draw_resource/quad_polygon.h"

#include"../object/camera.h"
#include"../object/object.h"

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
        triangleObjectInstnce_.initialize({0.2f,0.0f,-0.1f }, {1,1,1,1});

        if (!quadPolygonInstance_.creat(deviceInstance_)) {
            assert(false && "四角形ポリゴンの作成に失敗しました");
            return false;
        }
        quadObjectInstance_.initialize({ -.2f,0.0f,0.1f }, { 1,1,1,1 });
      

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

        cameraInstance_.initialize();

        if (!constantBufferDescriptorInstance_.create(deviceInstance_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3, true)) {
            assert(false && "定数バッファ用ディスクリプタヒープの作成に失敗しました");
            return false;
        }

        if (!cameraConatantBufferInstance_.create(deviceInstance_, constantBufferDescriptorInstance_, sizeof(camera::ConstBufferData), 0)) {
            assert(false&&"カメラ用のコンスタントバッファの作成に失敗しました");
            return false;
        }

        if (!triangleConstantBufferInstance_.create(deviceInstance_, constantBufferDescriptorInstance_, sizeof(object::ConstBufferData), 1)) {
            assert(false&&"三角形用コンスタントバッファの作成に失敗しました");
            return false;
        }

        if (!quadConstantBufferInstance_.create(deviceInstance_, constantBufferDescriptorInstance_, sizeof(object::ConstBufferData), 2)) {
            assert(false && "四角形用コンスタントバッファの作成に失敗しました");
            return false;
        }

        if (!depthBufferheapInstance_.create(deviceInstance_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1)) {
            assert(false && "デプスバッファ用のディスクリプタヒープの作成に失敗しました");
            return false;
        }

        if (!depthBuuferInstance_.create(deviceInstance_, depthBufferheapInstance_, windowInstance_)) {
            assert(false && "デプスバッファの作成に失敗しました");
            return false;
        }

        return true;
    }

  
    void loop() noexcept {
        while (windowInstance_.messageLoop()) {

            cameraInstance_.update();

            triangleObjectInstnce_.update();
            quadObjectInstance_.update();

            const auto backBufferIndex = swapChainInstance_.get()->GetCurrentBackBufferIndex();

           
            if (frameFenceValue_[backBufferIndex] != 0) {
                fenceInstance_.wait(frameFenceValue_[backBufferIndex]);
            }

        
            commandAllocatorInstance_[backBufferIndex].reset();
          
            commandListInstance_.reset(commandAllocatorInstance_[backBufferIndex]);

          
            auto pToRT = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
            commandListInstance_.get()->ResourceBarrier(1, &pToRT);

          
            D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTargetInstance_.getDescriptorHandle(deviceInstance_, descriptorHeapInstance_, backBufferIndex) };
          
            D3D12_CPU_DESCRIPTOR_HANDLE depthHandle = depthBuuferInstance_.getCPUDescriptorHandle();
            commandListInstance_.get()->OMSetRenderTargets(1, handles, false, &depthHandle);

          
            const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; 
            commandListInstance_.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

            commandListInstance_.get()->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
            //commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());
          
            commandListInstance_.get()->SetGraphicsRootSignature(rootSignatureInstance_.get());

          
            const auto [w, h] = windowInstance_.size();
            D3D12_VIEWPORT viewport{};
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;
            viewport.Width = static_cast<float>(w);
            viewport.Height = static_cast<float>(h);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            commandListInstance_.get()->RSSetViewports(1, &viewport);

          
            D3D12_RECT scissorRect{};
            scissorRect.left = 0;
            scissorRect.top = 0;
            scissorRect.right = w;
            scissorRect.bottom = h;
            commandListInstance_.get()->RSSetScissorRects(1, &scissorRect);

            ID3D12DescriptorHeap* p[] = { constantBufferDescriptorInstance_.get() };
            commandListInstance_.get()->SetDescriptorHeaps(1, p);

            camera::ConstBufferData cameraData{
                DirectX::XMMatrixTranspose(cameraInstance_.viewMatrix()),
                DirectX::XMMatrixTranspose(cameraInstance_.projection()),
            };
            UINT8* pCameraData{};
            cameraConatantBufferInstance_.constanceBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pCameraData));
            memcpy_s(pCameraData, sizeof(cameraData), &cameraData, sizeof(cameraData));
            cameraConatantBufferInstance_.constanceBuffer()->Unmap(0, nullptr);
            commandListInstance_.get()->SetGraphicsRootDescriptorTable(0, cameraConatantBufferInstance_.getGpuDescriptorHandle());

            commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());

         
            {
                quad_polygon::ConstBufferData quadData{
                        DirectX::XMMatrixTranspose(quadObjectInstance_.world()),
                        quadObjectInstance_.color()
                };
                UINT8* pQuadData{};
                quadConstantBufferInstance_.constanceBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pQuadData));
                memcpy_s(pQuadData, sizeof(quadData), &quadData, sizeof(quadData));
                quadConstantBufferInstance_.constanceBuffer()->Unmap(0, nullptr);
                commandListInstance_.get()->SetGraphicsRootDescriptorTable(1, quadConstantBufferInstance_.getGpuDescriptorHandle());

                quadPolygonInstance_.draw(commandListInstance_);
            }
           

            {
                triangle_polygon::ConstBufferData triangleData{
                        DirectX::XMMatrixTranspose(triangleObjectInstnce_.world()),
                        triangleObjectInstnce_.color()
                };
                UINT8* pTriangleData{};
                triangleConstantBufferInstance_.constanceBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pTriangleData));
                memcpy_s(pTriangleData, sizeof(triangleData), &triangleData, sizeof(triangleData));
                triangleConstantBufferInstance_.constanceBuffer()->Unmap(0, nullptr);
                commandListInstance_.get()->SetGraphicsRootDescriptorTable(1, triangleConstantBufferInstance_.getGpuDescriptorHandle());

                trianglePolygonInstance_.draw(commandListInstance_);
            }
        

            

            auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
            commandListInstance_.get()->ResourceBarrier(1, &rtToP);

         
            commandListInstance_.get()->Close();

            
            ID3D12CommandList* ppCommandLists[] = { commandListInstance_.get() };
            commandQueueInstance_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

         
            swapChainInstance_.get()->Present(1, 0);

           
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
    descriptor_heap constantBufferDescriptorInstance_{};
    //カメラ
    camera cameraInstance_{};
    constant_buffer cameraConatantBufferInstance_{};
    //△
    triangle_polygon trianglePolygonInstance_{};
    object triangleObjectInstnce_{};
    constant_buffer triangleConstantBufferInstance_{};
    //□
    quad_polygon quadPolygonInstance_{};
    object quadObjectInstance_{};
    constant_buffer quadConstantBufferInstance_{};

    descriptor_heap depthBufferheapInstance_{};
    depth_buffer depthBuuferInstance_{};
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
   
    Application app;

    if (!app.initialize(hInstance)) {
        assert(false && "アプリケーションの初期化に失敗しました");
    }

  
    app.loop();

    return 0;
}