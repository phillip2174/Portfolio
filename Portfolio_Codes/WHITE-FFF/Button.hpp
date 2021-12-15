#pragma once
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Component.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/UIComponents/TextRenderer.hpp"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"
#include "Core/EC/Components/SoundPlayer.hpp"
#include "Graphic/Texture.hpp"

#include <memory>

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

enum BUTTON_TYPE {
	UICONTROL = 0,
	STATECONTROL,
	LOADOUTSELECT,
	OPTION
};


struct OnHoverModifier {
	glm::vec3 ReColor = glm::vec3(1, 1, 1);
	string ReTexturePath = "";
	string ReText = "";
	glm::vec3 ReTextColor = glm::vec3(-1, 0, 0);
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			ReColor,
			ReTexturePath,
			ReText,
			ReTextColor
		);
	}
};

class Button : public BehaviourScript {
private:
	TextRenderer* text;
	MeshRenderer* mesh;
	SoundPlayer* sp;
	glm::vec2 m_pos;

	int DefaultTexture = -1;
	string DefaultText = "";
	glm::vec3 DefaultTextColor;
	
	int OnHoverTex = -1;
	bool playEnd = false;

	Graphic::Texture OnHoverTexObj;
	Graphic::Texture DefaultTexObj;
public:
	OnHoverModifier hoverModifier;
	bool isOnHover = false;
	
	//type of the button
	int buttonType = 0;
	//enum for the func paramiter of that type
	int onClickType = 0;

	Button();

	void ModifyOnHover();

	void SetModifier(OnHoverModifier modifier) { this->hoverModifier = modifier; }
	void SetButtonType(int buttonType, int onClickType) {this->buttonType = buttonType; this->onClickType = onClickType; }

	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	void OnClick();
	~Button();

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			hoverModifier,
			buttonType,
			onClickType
		);
	}
};

CEREAL_REGISTER_TYPE(Button);