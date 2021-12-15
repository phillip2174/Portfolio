#include "EnemyBehaviours.h"

void Flyer::OnAwake() {
	ENGINE_INFO("flyer init");
	airFollow = GetGameObject()->GetComponent<AirFollowing>();
	airDash = GetGameObject()->GetComponent<AirDash>();

	rigidbody = GetGameObject()->GetComponent<Rigidbody>();
	Enemy::OnAwake();
}

void Flyer::OnEnable() 
{
	Enemy::OnEnable();
	animator->setCurrentState(0);
}

void Flyer::OnUpdate(float dt) {
	if (GetGameObject()->Active() && !isDead) {
		Enemy::OnUpdate(dt);

		if (dashDelayTimer >= dashDelay && glm::length(target->GetPosition() - GetGameObject()->m_transform->GetPosition()) < 100.0f) {

			//GLRenderer::GetInstance()->DrawDebug_Circle(m_gameObject->m_transform->GetPosition().x, m_gameObject->m_transform->GetPosition().y, DashTriggerRadius, 1.0f, 0.0f, 0.0f);
			airDash->TargetLock(target->GetPosition());
			state = EnemyState::Active;

		}
		else if (foundTarget && state != EnemyState::Active) {
			state = EnemyState::Chase;
		}
		else if (state != EnemyState::Active) {
			state = EnemyState::Idle;
		}

		dashDelayTimer += dt;
	}
}

void Flyer::OnFixedUpdate(float dt) {

	if (!affectedByWeapon) {
		switch (state)
		{
		case Idle:
			rigidbody->SetVelocity(glm::vec3(0));
			airDash->Reset();
			break;
		case Chase:
			airFollow->SetPlayer(target);
			airFollow->FollowPlayer(dt);
			break;
		case Active:
			if (dashDelayTimer >= dashDelay) {
				
				if (!setAnimDash) 
				{
					setAnimDash = true;
					animator->setCurrentState(1);
				}

				airDash->Dash(dt);
				if (airDash->DashEnd()) {
					setAnimDash = false;
					state = EnemyState::Idle;
					animator->setCurrentState(0);
					dashDelayTimer = 0;
				}
			}
			else {
				state = EnemyState::Idle;
			}
		default:
			break;
		}
	}

	if (isDead && !setAnimDead) {
		setAnimDead = true;
		animator->setCurrentState(2);
	}
}

void Flyer::SetStats(float Speed, float rotateSpeed, float aimtime, float dashspeed, float HP, float Dmg)
{
	airFollow->SetFlySpeed(Speed);
	airFollow->SetRotRate(rotateSpeed);
	airDash->SetAimTime(aimtime);
	airDash->SetDashSpeed(dashspeed);
	hpSystem->SetMaxHP(HP);

	//will add air dash
	airDash->dashDamage = Dmg;
}