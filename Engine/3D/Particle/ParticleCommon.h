#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <forward_list>

#include "Camera.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Affin.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class ParticleCommon
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public: // サブクラス
	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial {
		Vector4 color; // 色 (RGBA)
	};

	// 頂点データ構造体
	struct VertexPos
	{
		Vector3 pos; // xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 mat;
		Matrix4 matBillboard;	// ビルボード行列
	};

	//パーティクル一粒
	struct Particle {

		//座標
		Vector3 position = {};
		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};
		//現在フレーム
		int frame = 0;
		//過去フレーム
		int num_frame = 0;

		//スケール
		float scale = 1.0f;
		//初期値
		float s_scale = 1.0f;
		//最終値
		float e_scale = 0.0f;

		Vector4 color; // 色 (RGBA)
	};

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	//static const int vertexCount = 30;//頂点数
	static const int vertexCount = 1024;

private: // 静的メンバ変数
	// デバイス
	ComPtr <ID3D12Device> device;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// コマンドリスト
	ComPtr <ID3D12GraphicsCommandList> cmdList;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// 頂点データ配列
	VertexPos vertices[vertexCount];
	//ビルボード行列
	Matrix4 matBillboard;
	//Y軸回りビルボード行列
	Matrix4 matBillboardY;
	//パーティクル配列
	std::forward_list<Particle>particles;

public: // メンバ関数
	void LoadTexture(const std::string& fileName);

	void Initialize();

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	///	<param name="life">生存時間</param>
	///	<param name="position">初期座標</param>
	///	<param name="velocity">速度</param>
	///	<param name="accel">加速度</param>
	void Add(int life, Vector3 position, Vector3 velociy, Vector3 accel, float start_scale, float end_scale);

	static void SetCamera(Camera* camera) { ParticleCommon::camera = camera; }


private:// 静的メンバ関数
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	void InitializeGraphicsPipeline();

	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	static Camera* camera;
	// ローカルスケール
	Vector3 scale = { 1,1,1 };

	ConstBufferDataMaterial* constMapMaterial = nullptr;
};
