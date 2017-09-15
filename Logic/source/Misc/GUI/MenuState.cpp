#include "..\..\..\include\Misc\GUI\MenuState.h"


Logic::MenuState::MenuState()
{
}

Logic::MenuState::~MenuState()
{
	for (int i = 0; i < m_buttons.size(); i++)
	{
		delete m_buttons[i];
	}
}

void Logic::MenuState::initialize(std::function<void(void)> callBack)
{
	DirectX::SimpleMath::Vector2 pos(200.0f, 200.0f);
	DirectX::SimpleMath::Vector2 texCoordStart(1.0f, 1.0f);
	DirectX::SimpleMath::Vector2 texCoordEnd(0.0f, 0.0f);
	float height = 200.0f;
	float width = 200.0f;
	std::string texture = "Bla";
	m_buttons.push_back(new Button());
	m_buttons.at(0)->initialize(pos, texCoordStart, texCoordEnd, height, width, texture, callBack);
}

void Logic::MenuState::update()
{
	auto Mouse = DirectX::Mouse::Get().GetState();
	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons.at(i)->update(Mouse);
	}
}
