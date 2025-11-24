#include "root_signature.h"
#include<cassert>


root_signature::~root_signature() {
	if (rootSignature_) {
		rootSignature_->Release();
		rootSignature_ = nullptr;
	}
}


bool root_signature::create(const device& device)noexcept {

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.NumParameters = 0;
	rootSignatureDesc.pParameters = nullptr;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signature{};
	auto res = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signature,
		nullptr
	);

	bool success = SUCCEEDED(res);
	if (!success) {
		assert(false && "ルートシグネチャのシリアライズに失敗");
	}
	else
	{
		res = device.get()->CreateRootSignature(
			0,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature_)
		);

		success &= SUCCEEDED(res);
		if (!success) {
			assert(false && "ルートシグネチャの生成に失敗");
		}
	}

	if (signature) {
		signature->Release();
	}

	return success;
}

ID3D12RootSignature* root_signature::get() const noexcept {
	if (!rootSignature_) {
		assert(false && "ルートシグネチャが生成されていません");
	}

	return rootSignature_;
}