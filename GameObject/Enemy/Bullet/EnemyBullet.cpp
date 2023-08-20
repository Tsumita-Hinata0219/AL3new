﻿#include "EnemyBullet.h"
#include "Player.h"




/// <summary>
/// 初期化
/// </summary>
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("/picture/eneBullet.png");

	// Z方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;


	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);

	// 省都対象を自分以外に設定
	SetCollisionMask(kCollisionAttributePlayer);
}



/// <summary>
/// 更新処理
/// </summary>
void EnemyBullet::Update() {

	// 自キャラまでのベクトル
	toPlayer_ = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);

	// ベクトルを正規化する
	Vector3 normalizeToPlayer = Normalize(toPlayer_);
	Vector3 normalizeVelocity = Normalize(velocity_);


	// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_ = sLerp(normalizeVelocity, normalizeToPlayer, 0.05f);


	// 弾をプレイヤーのほうへ回転
	Vector3 kVelocity = velocity_;
	velocityZ_ = sqrt((kVelocity.x * kVelocity.x) + (kVelocity.z * kVelocity.z));
	height_ = -kVelocity.y;
	worldTransform_.rotation_.y = std::atan2(kVelocity.x, kVelocity.z);
	worldTransform_.rotation_.x = std::atan2(height_, velocityZ_);

	
	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	
	// ワールドトラスフォームの更新
	worldTransform_.UpdateMatrix();


	// 時間経過でデス
	if (--deathTimer_ <= 0) {

		isDead_ = true;
	}

	// Enemyデバッグ
	ImGui::Begin("EnemyBullet");

	// float3入力ボックス
	ImGui::InputFloat3("bulletPosition", &worldTransform_.translation_.x);

	ImGui::End();
}



/// <summary>
/// 衝突時判定
/// </summary>
void EnemyBullet::onCollision() {

	// デスフラグを立てる
	isDead_ = true;
}



/// <summary>
/// 描画処理
/// </summary>
void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


Vector3 EnemyBullet::GetWorldPosition() {

	// ワールド座標を取得
	Vector3 worldPos{};

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}