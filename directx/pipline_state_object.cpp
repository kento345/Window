#include "pipline_state_object.h"
#include <cassert>



pipline_state_object::~pipline_state_object()
{
	if (pipelineState_) {
		pipelineState_->Release();
		pipelineState_ = nullptr;
	}
}


bool pipline_state_object::create(const device& device, const shader& shader, const root_signature& root_signature)noexcept {

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{"POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{   "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	D3D12_DEPTH_STENCIL_DESC depthStateDesc{};
	depthStateDesc.DepthEnable = true;
	depthStateDesc.StencilEnable = false;
	depthStateDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	D3D12_RENDER_TARGET_BLEND_DESC renderBlendDesc{};
	renderBlendDesc.BlendEnable = true;

	renderBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	renderBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	renderBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;

	renderBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	renderBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	renderBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

	renderBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_BLEND_DESC blendDesc{};
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		blendDesc.RenderTarget[i] = renderBlendDesc;
	}

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


	/*const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
		FALSE,
		FALSE,
		D3D12_BLEND_ONE,
		D3D12_BLEND_ZERO,
		D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE,
		D3D12_BLEND_ZERO,
		D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL,
	};
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
		blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;
	}*/

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = root_signature.get();
	psoDesc.VS = { shader.vertexShader()->GetBufferPointer(), shader.vertexShader()->GetBufferSize() };
	psoDesc.PS = { shader.pixelShader()->GetBufferPointer(), shader.pixelShader()->GetBufferSize() };
	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.BlendState = blendDesc;
	psoDesc.DepthStencilState = depthStateDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;

	auto res = device.get()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_));

	if (FAILED(res)) {
		assert(false && "パイプラインステートの作成に失敗");
	}

	return true;
}


ID3D12PipelineState* pipline_state_object::get()const noexcept {
	if (!pipelineState_) {
		assert(false && "パイプラインステートが未作成です");
	}
	return pipelineState_;
}