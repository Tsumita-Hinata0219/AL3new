#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"


GameScene::GameScene() {
}

GameScene::~GameScene() {

	delete model_; 

	// 自キャラの解放
	delete player_;
	
	// 敵キャラの解放
	delete enemy_;

	// デバッグカメラの解放
	delete debugCamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();



	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();



	
	// キャラクターの生成
	player_ = new Player(); 
	enemy_ = new Enemy();

	// キャラクターの初期化
	player_->Initialize(model_); 
	enemy_->Initialize(model_, enemy_->Velocity());
	



	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() {

	// 自キャラの更新
	player_->Update();

	// 敵キャラの更新
	enemy_->Update();


	#ifdef _DEBUG

	if (input_->TriggerKey(DIK_RETURN)) 
	{
		isDebugCameraActive_ = true;
	}

#endif // DEBUG


	// カメラの処理
	if (isDebugCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>


	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵キャラの描画
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
