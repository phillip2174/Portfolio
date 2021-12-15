#include "AnimationController.hpp"
#include "Core/Logger.hpp"
#include <algorithm>

AnimationState::AnimationState() 
{

}

AnimationState::AnimationState(std::shared_ptr<Animation> anim) {
	animation = anim;
}

AnimationController::AnimationController()
{
	m_spriteSheetFrameSize = glm::vec2(0.0f, 0.0f);

}

void AnimationController::AddState(std::shared_ptr<AnimationState> state) {
	m_states.push_back(state);

	ENGINE_INFO("number of states: {}", m_states.size());

	//if no default state
	if (m_defaultState.expired()) {
		ENGINE_INFO("default anim state set");
		m_defaultState = state;
	}
}

void AnimationController::AddState(std::shared_ptr<Animation> anim) {
	std::shared_ptr<AnimationState> state = std::make_shared<AnimationState>(anim);
	AddState(state);
}

void AnimationController::AddState(std::shared_ptr<Animation> anim, bool isloop) {
	std::shared_ptr<AnimationState> state = std::make_shared<AnimationState>(anim);
	state->loop = isloop;
	AddState(state);
}

void AnimationController::setSheetSize(glm::vec2 frameSize) {
	m_spriteSheetFrameSize = frameSize;
}

glm::vec2 AnimationController::getSheetSize() {
	return m_spriteSheetFrameSize;
}

std::shared_ptr<AnimationState> AnimationController::GetState(int state)
{
	if (state >= 0 && state < m_states.size()) {
		return m_states[state];
	}
	else {
		ENGINE_WARN("Animation state {} not exist", state);
		return nullptr;
	}
}

void AnimationController::SwapState(int host, int target)
{
	if (host >= 0 && host < m_states.size() 
		&& target >= 0 && target < m_states.size() 
		&& host != target)
	{
		std::iter_swap(m_states.begin() + host, m_states.begin() + target);
	}
}

AnimationController::~AnimationController()
{
}
