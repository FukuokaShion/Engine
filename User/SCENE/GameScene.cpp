#include "GameScene.h"
#include "FbxLoader.h"

/// <summary>
	/// コンストクラタ
	/// </summary>
GameScene::GameScene() {
}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {
	delete spriteCommon;
	delete camera;

	delete floor;
	delete floorMD;
	delete skydome;
	delete skydomeMD;

	delete fbxObject3d_;
	delete fbxModel_;
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize(DirectXCommon* dxCommon, Input* input) {
	// nullチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	camera->SetEye({0,3,-8});
	camera->SetTarget({ 0,3,0 });

	ParticleManager::SetCamera(camera);
	Object3d::SetCamera(camera);

	floorMD = Model::LoadFromOBJ("floor");
	floor = Object3d::Create();
	floor->SetModel(floorMD);
	floor->wtf.position = (Vector3{ 0, -10, 0 });

	skydomeMD = Model::LoadFromOBJ("skydome");
	skydome = Object3d::Create();
	skydome->SetModel(skydomeMD);
	skydome->wtf.scale = (Vector3{ 1000, 1000, 1000 });


	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon->GetDevice());
	FBXObject3d::SetCamera(camera);
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	fbxModel_ = FbxLoader::GetInstance()->LoadModelFromFile("Player");


	fbxObject3d_ = new FBXObject3d;
	fbxObject3d_->Initialize();
	fbxObject3d_->SetModel(fbxModel_);
	fbxObject3d_->SetScale({ 0.01,0.01,0.01 });
	fbxObject3d_->SetPosition({ -10,10,40 });
	fbxObject3d_->PlayAnimation();
	

	Reset();
}

void GameScene::Reset() {

}

/// <summary>
/// 毎フレーム処理
/// </summary>
void GameScene::Update() {
	camera->Update();
	floor->Update();
	skydome->Update();
	fbxObject3d_->Update();
}

/// <summary>
/// 描画
/// </summary>
void GameScene::Draw() {

	/// <summary>
	/// 3Dオブジェクトの描画
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// <summary>
	//3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	//// 3Dオブクジェクトの描画
	floor->Draw();
	skydome->Draw();
	fbxObject3d_->Draw(dxCommon->GetCommandList());
	//3Dオブジェクト描画後処理
	Object3d::PostDraw();


	//// パーティクルの描画

	// パーティクル描画前処理
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	// パーティクル描画後処理
	ParticleManager::PostDraw();

}