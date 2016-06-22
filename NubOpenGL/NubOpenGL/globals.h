#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

enum Errors
{
	SUCC = 0,
	GENERIC_ERROR,
	SUPER_BAD_ERROR
};

namespace glsettings
{
	const int majorVer = 3;
	const int minorVer = 3;
}

//TODO: probably gonna read this from a file eventually lol
namespace windowsettings
{
	const int width = 1024;
	const int height = 768;
}

#endif