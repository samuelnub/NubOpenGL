#pragma once
#ifndef VERTICESTEST_H
#define VERTICESTEST_H

#include "globals.h"
#include <vector>

namespace verts
{
	const std::vector<Vertex> triangle1 =
	{
		{-0.5f, -0.5f, 0.0f,
		1.0f, 0.0f, 0.0f },

		{0.5f, -0.5f, 0.0f,
		0.0f, 1.0f, 0.0f },

		{0.0f, 0.75f, 0.0f,
		0.0f, 0.0f, 1.0f }
	};

	const std::vector<Vertex> quad1 =
	{
		{0.5f,  0.5f, 0.0f,
		1.0f, 0.0f, 0.0f },

		{0.5f, -0.5f, 0.0f,
		0.0f, 1.0f, 0.0f },

		{-0.5f, -0.5f, 0.0f,
		0.0f, 0.0f, 1.0f },

		{-0.5f,  0.5f, 0.0f,
		1.0f, 0.0f, 0.0f }
	};

	const std::vector<GLushort> quad1indices =
	{
		0, 1, 3,
		1, 2, 3
	};
}

#endif