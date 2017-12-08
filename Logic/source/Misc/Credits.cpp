#include <Misc\Credits.h>
#include <sstream>
using namespace Logic;

Credits::Credits()
{
    std::wstring test = L"Created By: Stockman Games Entertainment\n\nProgrammers:\nAndreas Henriksson\nHenrik Vik\nJakob Nyberg\nSimon Fredholm\nLukas Westling\nEmanuel Bjurman\nFelix Kaaman\nJohan Ottosson\nSimon Sandberg\n\nTechnical Artists:\nJohan Ottosson\nSimon Sandberg\n\nMusic:\nBanana (INSERT REAL NAME)\n"; // See it in game not here dude :>
    std::wstringstream str(test);
    std::wstring temp;

    float x = 300.f, y = 715.5f;
    infoText.reserve(150);

    while (getline(str, temp, L'\n'))
    {
        if (!temp.empty())
        {
            infoText.push_back(temp);

            TextRenderInfo info;
            info.color = DirectX::SimpleMath::Color(1.f, 1.f, 1.f);
            info.font = Resources::Fonts::KG26;
            info.text = infoText[infoText.size() - 1].c_str();
            info.position = DirectX::SimpleMath::Vector2(x, y);
            this->info.push_back(info);
        }

        y += 40;
    }
}

void Credits::update(float deltaTime)
{
    // this is for my own amusement, not intended to be in final product or testing :>
    for (auto &info : info)
        info.position.y -= deltaTime / 70.f;
}

void Credits::render() const
{
    // this is only for fun
    for (auto &info : info)
        if (info.position.y < 750.f && info.position.y > -50.f)
            QueueRender(info);
}