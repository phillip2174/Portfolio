#include "EnemyBehaviours.h"
#include "GameController.hpp"
#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"
#include "Core/Logger.hpp"
#include "Core/Particle/ParticleSystem.h"

#include "ItemDrop.hpp"

void DeQueen::OnAwake() {
	airPatrol = GetGameObject()->GetComponent<AirPatrol>();
	
	SpawnDelay = 0.2f;
	SpawnDelayCount = SpawnDelay;

	FlyerPool = GameController::GetInstance()->GetPool(POOL_TYPE::ENEMY_FLYER);
	BomberPool = GameController::GetInstance()->GetPool(POOL_TYPE::ENEMY_BOMBER);
	ItemPool = GameController::GetInstance()->GetPool(POOL_TYPE::ITEM_DROP);

	Enemy::OnAwake();
}

void DeQueen::OnEnable() 
{
	setParticle = false;
	Enemy::OnEnable();
}

void DeQueen::OnDisable() 
{
	if (queenDeadFluidPtcl != nullptr) 
	{
		queenDeadFluidPtcl->SetActive(false);
	}

	if (queenDeadSmokePtcl != nullptr) {

		queenDeadSmokePtcl->SetActive(false);
	}
}

void DeQueen::SetStats(float Speed, float HP, float SpawnDelay, float unlockchance, float healvalue) {
	airPatrol->SetSpeed(Speed);
	hpSystem->SetMaxHP(HP);
	this->SpawnDelay = SpawnDelay;
	this->HealValue = healvalue;
	this->ItemUnlockDropChance = unlockchance;
}

void DeQueen::OnUpdate(float dt) {
	
	//no need to detect target
	//Enemy::OnUpdate(dt);
	if (!isDead) {
		airPatrol->Patrol();
		GameObject* spawn = nullptr;

		if ((m_gameObject->m_transform->GetPosition().x > DespawnPosX) || (m_gameObject->m_transform->GetPosition().x < -DespawnPosX))
		{
			m_gameObject->SetActive(false);
		}

		SpawnDelayCount -= dt;
		if (SpawnDelayCount <= 0)
		{
			SpawnDelayCount = SpawnDelay;
			int randSpawn = rand() % 2;
			int spawnPosX = airPatrol->queen->GetPosition().x;
			int spawnPosY = airPatrol->queen->GetPosition().y - 100;
			if (randSpawn == 0) {
				//std::shared_ptr<GameObject> flyer = FlyerPool->GetInactiveObject();
				GameObject* flyer = FlyerPool->GetInactiveObject();
				if (flyer != nullptr)
				{
					flyer->SetActive(true);

					flyer->m_transform->SetPosition(glm::vec3(spawnPosX, spawnPosY, 1.0f));

					spawn = GameController::GetInstance()->GetPool(POOL_TYPE::PTCL_ENEMYSPAWN1)->GetGameObject();

					flyer->GetComponent<Enemy>()->SetTarget(GameController::GetInstance()->GetPlayer()->m_transform.get());
				}
			}
			else {
				GameObject* bomber = BomberPool->GetInactiveObject();
				if (bomber != nullptr)
				{
					bomber->SetActive(true);

					bomber->m_transform->SetPosition(glm::vec3(spawnPosX, spawnPosY, 1.0f));

					spawn = GameController::GetInstance()->GetPool(POOL_TYPE::PTCL_ENEMYSPAWN2)->GetGameObject();

					bomber->GetComponent<Enemy>()->SetTarget(GameController::GetInstance()->GetPlayer()->m_transform.get());
				}
			}
			if (spawn != nullptr) {
				spawn->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());
				spawn->GetComponent<ParticleSystem>()->TriggerBurstEmission();
			}
		}
	}

	

	if (isDead && !setParticle) 
	{
		setParticle = true;
		queenDeadFluidPtcl = GameController::GetInstance()->GetPool(POOL_TYPE::PTCL_KILLED_QUEEN_FLUID)->GetGameObject();
		queenDeadFluidPtcl->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());
		queenDeadFluidPtcl->GetComponent<ParticleSystem>()->TriggerBurstEmission();

		queenDeadSmokePtcl = GameController::GetInstance()->GetPool(POOL_TYPE::PTCL_KILLED_QUEEN_SMOKE)->GetGameObject();
		queenDeadSmokePtcl->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());
		queenDeadSmokePtcl->SetActive(true);
	}
	else if (isDead) 
	{
		//queenDeadFluidPtcl->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());
		queenDeadSmokePtcl->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());
	}

}

void DeQueen::OnFixedUpdate(float dt) {

}

//void DeQueen::assignFlyPool(ObjectPool* pool) {
//	this->FlyerPool = pool;
//}
//
//void DeQueen::assignBombPool(ObjectPool* pool) {
//	this->BomberPool = pool;
//}

void DeQueen::SetSpawnDelay(int time) {
	this->SpawnDelay = time;
}

void DeQueen::SpawnItem() 
{
	GameObject* item = ItemPool->GetGameObject();
	GameObject* healItem = ItemPool->GetGameObject();

	if (item != nullptr && healItem != nullptr) {

		healItem->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(0));
		healItem->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());

		item->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(0));

		if (!GameController::GetInstance()->GetGameObject()->GetComponent<EquipmentManager>()->isAllUnlock())
		{
			int randChance = (rand() % 100) + 1;

			if (randChance < ItemUnlockDropChance) 
			{
				item->SetActive(true);
				item->GetComponent<ItemDrop>()->SetType(Drop_Type::Unlock);

				item->m_transform->SetPosition(m_gameObject->m_transform->GetPosition() - glm::vec3(50.0f, 0.0f, 0.0f));


				healItem->SetActive(true);
				healItem->GetComponent<ItemDrop>()->SetType(Drop_Type::Heal);
				healItem->GetComponent<ItemDrop>()->SetHealValue(HealValue);

				healItem->m_transform->SetPosition(m_gameObject->m_transform->GetPosition() + glm::vec3(50.0f, 0.0f, 0.0f));
			}
			else 
			{
				healItem->SetActive(true);
				healItem->GetComponent<ItemDrop>()->SetType(Drop_Type::Heal);
				healItem->GetComponent<ItemDrop>()->SetHealValue(HealValue);
			}
		}
		else 
		{
			healItem->SetActive(true);
			healItem->GetComponent<ItemDrop>()->SetType(Drop_Type::Heal);
			healItem->GetComponent<ItemDrop>()->SetHealValue(HealValue);
		}
		
	}
	else {
		ENGINE_ERROR("{} Can't spawn Item", *m_gameObject);
	}
}