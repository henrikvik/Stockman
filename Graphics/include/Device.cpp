#pragma once
#include "Device.h"

namespace Global
{
    ID3D11Device * device = nullptr;
    ID3D11DeviceContext * context = nullptr;
    void * nulls[32] = { nullptr };

}
