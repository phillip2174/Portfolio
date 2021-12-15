#pragma once

#include "Animation.hpp"
#include <memory>
#include <string>

#include <Serialization/glmCereal.h>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

namespace Tools { class EditorAC; class AnimatorEC; }

class AnimationState {
public:
	std::string stateName = "State";
	std::shared_ptr<Animation> animation;
	bool loop = false;
	std::weak_ptr<AnimationState> nextState;

	AnimationState();
	AnimationState(std::shared_ptr<Animation>);
	
	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			stateName,
			animation,
			loop,
			cereal::defer(nextState)
			);
	}

};

class AnimationController
{
	friend class Tools::EditorAC;
	friend class Tools::AnimatorEC;
protected:
	glm::vec2 m_spriteSheetFrameSize = glm::vec2(1.0f, 1.0f);
	std::vector<std::shared_ptr<AnimationState>> m_states;
public:
	std::weak_ptr<AnimationState> m_defaultState;

	AnimationController();
	void AddState(std::shared_ptr<AnimationState>);
	void AddState(std::shared_ptr<Animation>);
	void AddState(std::shared_ptr<Animation>, bool loop);
	std::shared_ptr<AnimationState> GetState(int state);
	void SwapState(int host, int target);
	void setSheetSize(glm::vec2 frameSize);
	
	glm::vec2 getSheetSize();
	~AnimationController();

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			m_spriteSheetFrameSize,
			m_states,
			m_defaultState
		);

		//for /*default state and*/ next states of each state
		archive.serializeDeferments();
	}
};

