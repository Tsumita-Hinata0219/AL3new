#include "Skydome.h"



// 初期化処理
void Skydome::Initialize() {

}

// 更新処理
void Skydome::Update() {

}


// 描画処理
void Skydome::Draw(ViewProjection viewProjection) {

	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection);
}
