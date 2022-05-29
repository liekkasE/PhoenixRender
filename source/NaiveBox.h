#pragma once
#include "base/FXCommon.h"
#include "GlobalContext.hpp"
#include "RenderCore.hpp"
#include "Mesh.h"

namespace FX
{

	std::vector<Vertex> naive_box_vertices = {
	{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec4(1.0, 0.0, 0.0, 1.0)},
	{glm::vec3(-1.0f, +1.0f, -1.0f), glm::vec4(1.0, 0.5, 0.0, 1.0)},
	{glm::vec3(+1.0f, +1.0f, -1.0f), glm::vec4(1.0, 0.8, 0.0, 1.0)},
	{glm::vec3(+1.0f, -1.0f, -1.0f), glm::vec4(0.0, 1.0, 0.0, 1.0)},
	{glm::vec3(-1.0f, -1.0f, +1.0f), glm::vec4(0.1, 0.5, 0.0, 1.0)},
	{glm::vec3(-1.0f, +1.0f, +1.0f), glm::vec4(0.5, 0.5, 1.0, 1.0)},
	{glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec4(0.1, 0.8, 1.0, 1.0)},
	{glm::vec3(+1.0f, -1.0f, +1.0f), glm::vec4(0.5, 1.0, 0.3, 1.0)}

	};


    std::vector<Index> naive_box_indices = {
        // 前表面
        0, 1, 2,
        0, 2, 3,

        // 后表面
        4, 6, 5,
        4, 7, 6,

        // 左表面
        4, 5, 1,
        4, 1, 0,

        // 右表面
        3, 2, 6,
        3, 6, 7,

        // 上表面
        1, 5, 6,
        1, 6, 2,

        // 下表面
        4, 0, 3,
        4, 3, 7
    };

}