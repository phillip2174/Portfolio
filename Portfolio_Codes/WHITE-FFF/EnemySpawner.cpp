#include "EnemySpawner.hpp"

void EnemySpawner::OnUpdate(float dt)
{
	if (Spawning) {
		//ENGINE_INFO("Spawner type: {}, interval: {}/{}", SpawnType, SpawnRateCount, 1.0f / SpawnRate);
		SpawnRateCount += dt;

		if (SpawnRateCount > 1.0f / SpawnRate)
		{
			SpawnRateCount = 0;

			SpawnEnemy();
		}
	}
}

void EnemySpawner::OnAwake() 
{
}

GameObject* EnemySpawner::SpawnEnemy() {
	if (Spawning) {

		glm::vec2 SpawnPos;

		switch (SpawningMode)
		{
		case SPAWN_MODE::EDGE:
			SpawnPos = GetRandomPos_Edge();
			break;
		case SPAWN_MODE::PLATFORM:
			SpawnPos = GetRandomPos_Platform();
			break;
		case SPAWN_MODE::RANGE:
			SpawnPos = GetRandomPos_Range(x1, y1, x2, y2);
			break;
		default:
			SpawnPos = glm::vec2(0);
			break;
		}


		if (SpawnPos.x != -1 && SpawnPos.y != -1) 
		{

			return SpawnEnemy(SpawnPos.x, SpawnPos.y);
		}
	}

	return nullptr;
}

GameObject* EnemySpawner::SpawnEnemy(float posX,float posY)
{
	if (SpawnAmplifier != nullptr) {
		if (EnemyPool != nullptr) {
			GameObject* enemy = EnemyPool->GetInactiveObject();
			if (enemy != nullptr)
			{
				enemy->GetComponent<Enemy>()->SetTarget(EnemyTarget->m_transform.get());

				enemy->SetActive(true);
				enemy->GetComponent<HPsystem>()->ResetHP();
				enemy->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(0));

				enemy->m_transform->SetPosition(glm::vec3(posX, posY, 1.0f));

				switch (SpawnType) {
				case POOL_TYPE::ENEMY_FLYER:
					enemy->GetComponent<Flyer>()->SetStats(
						SpawnAmplifier->FlyerSpeed,
						SpawnAmplifier->FlyerRotationSpeed,
						SpawnAmplifier->FlyerAimTime,
						SpawnAmplifier->FlyerDashSpeed,
						SpawnAmplifier->FlyerHP,
						SpawnAmplifier->FlyerDmg
						);
					break;
				case POOL_TYPE::ENEMY_BOMBER:
					enemy->GetComponent<Bomber>()->SetStats(
						SpawnAmplifier->BomberSpeed,
						SpawnAmplifier->BomberRotationSpeed,
						SpawnAmplifier->BomberHP,
						SpawnAmplifier->BomberDmg,
						SpawnAmplifier->BomberAimTime,
						SpawnAmplifier->BomberDashSpeed,
						SpawnAmplifier->BomberExplodeDMG,
						SpawnAmplifier->BomberExplodeRadius
						);
					break;
				case POOL_TYPE::ENEMY_QUEEN:
					enemy->GetComponent<DeQueen>()->SetStats(
						SpawnAmplifier->QueenSpeed,
						SpawnAmplifier->QueenHP,
						SpawnAmplifier->QueenSpawnDelay,
						SpawnAmplifier->QueenUnlockDropChance,
						SpawnAmplifier->QueenHealItemValue
						);
					break;
				case POOL_TYPE::ENEMY_TANK:
					enemy->GetComponent<Tank>()->SetStats(
						SpawnAmplifier->TankSpeed,
						SpawnAmplifier->TankHP
						);
					break;
				case POOL_TYPE::ENEMY_CHARGER:
					enemy->GetComponent<Charger>()->SetStats(
						SpawnAmplifier->ChargerSpeed,
						SpawnAmplifier->ChargerHP,
						SpawnAmplifier->ChargerDashPauseTime,
						SpawnAmplifier->ChargerDashSpeed,
						SpawnAmplifier->ChragerDashDamage
						);
					break;
				case POOL_TYPE::ENEMY_SPITTER:
					enemy->GetComponent<Spitter>()->SetStats(
						SpawnAmplifier->SpitterSpeed,
						SpawnAmplifier->SpitterHP,
						SpawnAmplifier->SpitterFireRate
						);
					break;
				default:
					break;
				}

				return enemy;
			}
		}
		else {
			ENGINE_WARN("No enemy Pool assigned");
		}
	}
	else {
		ENGINE_WARN("No enemy amplifier assigned");
	}

	return nullptr;
}

void EnemySpawner::SetSpawnRange(float x1, float y1, float x2, float y2) {
	this->x1 = x1; this->x2 = x2; this->y1 = y1; this->y2 = y2;
}

void EnemySpawner::SetSpawnRate(float value) {
	this->SpawnRate = value;
	SpawnRateCount = 0;
}

void EnemySpawner::SetSpawnType(int type) {
	this->SpawnType = type;
	EnemyPool = GameController::GetInstance()->GetPool(type);
}

void EnemySpawner::updateSpawner() {

	if (SpawnPreset != nullptr) {
		switch (SpawnType)
		{
		case POOL_TYPE::ENEMY_FLYER:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->FlyerRatio;
			break;
		case POOL_TYPE::ENEMY_BOMBER:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->BomberRatio;
			break;
		case POOL_TYPE::ENEMY_QUEEN:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->QueenRatio;
		case POOL_TYPE::ENEMY_COCOON:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->CocoonRatio;
			break;
		case POOL_TYPE::ENEMY_TANK:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->TankRatio;
			break;
		case POOL_TYPE::ENEMY_CHARGER:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->ChargerRatio;
			break;
		case POOL_TYPE::ENEMY_SPITTER:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->SpitterRatio;
			break;
		default:
			break;
		}
	}
	else {
		ENGINE_WARN("No enemy preset assigned");
	}
	
	//SpawnRateCount = SpawnRate;
	//ENGINE_INFO("Spawner Spawn Rate: {}", SpawnRate);
}


glm::vec2 EnemySpawner::GetRandomPos_Edge() {
	int winWidth;
	int winHeight;

	cam = Graphic::getCamera();
	glm::vec3 camPos = cam->GetCampos();

	winWidth = Graphic::Window::GetWidth() * cam->GetZoom();
	winHeight = Graphic::Window::GetHeight() * cam->GetZoom();

	glm::vec2 tmp = GetRandomPos_Range(camPos.x + (winWidth * 0.25f), camPos.y + (winHeight * 0.25f), camPos.x - (winWidth * 0.25f), camPos.y - (winHeight * 0.25f));

	if (tmp.x >= 0) 
	{
		tmp.x += (winWidth / 2);
	}
	else 
	{
		tmp.x -= (winWidth / 2);
	}

	if (tmp.y >= 0) 
	{
		tmp.y += (winHeight / 2);
	}
	else 
	{
		tmp.y -= (winHeight / 2);
	}

	return tmp;
}

glm::vec2 EnemySpawner::GetRandomPos_Platform() {
	int winWidth;
	int winHeight;

	cam = Graphic::getCamera();
	glm::vec3 camPos = cam->GetCampos();

	winWidth = Graphic::Window::GetWidth() * cam->GetZoom();
	winHeight = Graphic::Window::GetHeight() * cam->GetZoom();

	Physic::PhysicScene* ps = Physic::PhysicScene::GetInstance();
	Platforms = ps->GetColliderLayer(ps->GetLayerFromString("Platform"));
	
	int randPlatform = rand() % Platforms.size();
	
	Transform* pf = Platforms.at(randPlatform)->GetGameObject()->m_transform.get();

	glm::vec2 tmp(-1, -1);

	while(camPos.x - (winWidth / 2) < (pf->GetPosition().x + pf->GetScale().x) &&
		  (camPos.x - (winWidth / 2) + winWidth) > pf->GetPosition().x &&
		  camPos.y - (winHeight / 2) < (pf->GetPosition().y + pf->GetScale().y) &&
		  (camPos.y - (winHeight / 2) + winHeight) >  pf->GetPosition().y)
	{
		Platforms.erase(Platforms.begin() + randPlatform);

		if (Platforms.size() == 0) 
		{
			//GAME_INFO("NO PLATFORM DETECTED!");

			return tmp;
		}
		else {
			randPlatform = rand() % Platforms.size();
			pf = Platforms.at(randPlatform)->GetGameObject()->m_transform.get();
		}
	}

	//GAME_INFO("Spawing on Platform!");

	Transform* enem = EnemyPool->GetGameObject()->m_transform.get();

	float spposY = (enem->GetTrueScale().y + pf->GetTrueScale().y) * 0.5f;

	if (SpawnType != POOL_TYPE::ENEMY_COCOON)
	{
		tmp = GetRandomPos_Range(pf->GetPosition().x + (pf->GetTrueScale().x / 2), pf->GetPosition().y + spposY + 50.0f, pf->GetPosition().x - (pf->GetTrueScale().x / 2), pf->GetPosition().y + spposY + 50.0f);
	}
	else
	{
		tmp = GetRandomPos_Range(pf->GetPosition().x + (pf->GetTrueScale().x / 2), pf->GetPosition().y - spposY, pf->GetPosition().x - (pf->GetTrueScale().x / 2), pf->GetPosition().y - spposY);
	}

	return tmp;
}

glm::vec2 EnemySpawner::GetRandomPos_Range(float rx1, float ry1, float rx2, float ry2) {

	float randPosX, randPosY;

	if (rx2 == rx1) {
		randPosX = rx1;
	}
	else if (rx2 > rx1) {
		randPosX = (rand() % (int)(rx2 - rx1 + 1)) + rx1;
	}
	else {
		randPosX = (rand() % (int)(rx1 - rx2 + 1)) + rx2;
	}

	if (ry2 == ry1) {
		randPosY = ry1;
	}
	else if (ry2 > ry1) {
		randPosY = (rand() % (int)(ry2 - ry1 + 1)) + ry1;
	}
	else {
		randPosY = (rand() % (int)(ry1 - ry2 + 1)) + ry2;
	}
	
	return glm::vec2(randPosX, randPosY);
}
