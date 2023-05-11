#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "FbxLoader.h"
#include "FPS.h"

#include "GameScene.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

#pragma region WindowsAPI初期化処理
	//ポインタ
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	FPS* fps = new FPS;
	Input* input = nullptr;
	ImGuiManager* imgui = nullptr;

	GameScene* gameScene = nullptr;

	//windowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//入力の初期化　
	input = new Input();
	input->Initialize(winApp);

	// ImGuiの初期化
	imgui = new ImGuiManager();
	imgui->Initialize(winApp,dxCommon);

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
#pragma endregion

#pragma region DirectX初期化処理
	//3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
	//パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

#pragma endregion

#pragma region 描画初期化処理
	//ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, input);

	//FPS変えたいとき
	fps->SetFrameRate(60);

#pragma endregion
	//ゲームループ
	while (true) {
#pragma region ウィンドウメッセージ処理

		//アプリケーションが終わる時にmessageがWM_QUITになる
		if (winApp->ProcessMessage()) {
			break;
		}if (input->PushKey(DIK_ESCAPE)) {
			break;
		}

		fps->FpsControlBegin();

#pragma endregion

#pragma region DirectX毎フレーム処理
		//DireceX毎フレーム処理
		//入力の更新
		input->Update();	

		// ゲームシーンの毎フレーム処理
		gameScene->Update();

#pragma endregion

#pragma region グラフィックスコマンド

		//描画コマンド
		dxCommon->PreDraw();

		//Imgui受付開始
		imgui->Begin();
		//デモウィンドウの表示オン
		ImGui::ShowDemoWindow();

		//ゲームシーンの描画
		gameScene->Draw();

		//Imgui受付終了
		imgui->End();

		//Imgui描画
		imgui->Draw();

		//描画終了
		dxCommon->PostDraw();

		fps->FpsControlEnd();

#pragma endregion

#pragma region 画面入れ替え

#pragma endregion
	}
#pragma region  WindowsAPI後始末
	delete gameScene;
	imgui->Finalize();
	FbxLoader::GetInstance()->Finalize();

	//WindowsAPIの終了処理
	winApp->Finalize();
	delete imgui;

	//入力開放
	delete input;
	//WindowsAPI開放
	delete winApp;
	delete dxCommon;

	delete fps;
	
#pragma endregion
	return 0;
}