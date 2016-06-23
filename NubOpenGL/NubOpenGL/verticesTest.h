#pragma once
#ifndef VERTICESTEST_H
#define VERTICESTEST_H

#include "globals.h"
#include <vector>

namespace verts
{
	const std::vector<Vertex> triangle1 =
	{
		{-0.5f, -0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{0.0f, 0.5f, 0.0f}
	};
}

#endif