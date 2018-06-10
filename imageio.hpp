#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "vector.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#define __STDC_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace pathtracing
{

inline void WritePPM(uint32_t w, uint32_t h,
					 const Vector3 *Ls, const char *fname = "./cpp-image.ppm") noexcept
{

	FILE *fp;

#ifdef __WIN32__
	fopen_s(&fp, fname, "w");
#else
	fp = fopen(fname, "w");
	if (!fp)
	{
		perror("File opening failed");
	}
#endif

	fprintf(fp, "P3\n%u %u\n%u\n", w, h, 255u);
	for (size_t i = 0; i < w * h; ++i)
	{
		fprintf(fp, "%u %u %u ", ToByte(Ls[i].m_x),
				ToByte(Ls[i].m_y),
				ToByte(Ls[i].m_z));
	}

	fclose(fp);
}
} // namespace pathtracing
