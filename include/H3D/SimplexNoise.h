//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file SimplexNoise.h
/// \brief Header file for SimplexNoise.
///
//
//    Adapted from SimplexNoise1234, Copyright 2003-2011, Stefan Gustavson
//    which was released as public domain software without any restrictions
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __SIMPLEXNOISE1234_H__
#define __SIMPLEXNOISE1234_H__

namespace H3D {

	class SimplexNoise1234 {
		// This is a clean, fast, modern and free Perlin Simplex noise class in C++.
		// Being a stand-alone class with no external dependencies, it is
		// highly reusable without source code modifications.

	public:
		SimplexNoise1234();
		virtual ~SimplexNoise1234() {};

		/** 1D, 2D, 3D and 4D float Perlin noise
		*/

		static float noise( float x );
		static float noise( float x, float y );
		static float noise( float x, float y, float z );
		static float noise( float x, float y, float z, float w );

		/** 1D, 2D, 3D and 4D float Perlin noise, with a specified integer period
		*/
		// comment from SU: not implemented yet... :(
		static float pnoise( float x, int px );
		static float pnoise( float x, float y, int px, int py );
		static float pnoise( float x, float y, float z, int px, int py, int pz );
		static float pnoise( float x, float y, float z, float w,
			int px, int py, int pz, int pw );

	private:
		static unsigned char perm[];
		static unsigned char simplex[][4];
		static float  grad( int hash, float x );
		static float  grad( int hash, float x, float y );
		static float  grad( int hash, float x, float y , float z );
		static float  grad( int hash, float x, float y, float z, float t );


		/*
		* Permutation table. This is just a random jumble of all numbers 0-255,
		* repeated twice to avoid wrapping the index at 255 for each lookup.
		* This needs to be exactly the same for all instances on all platforms,
		* so it's easiest to just keep it as static explicit data.
		* This also removes the need for any initialisation of this class.
		*
		* Note that making this an int[] instead of a char[] might make the
		* code run faster on platforms with a high penalty for unaligned single
		* byte addressing. Intel x86 is generally single-byte-friendly, but
		* some other CPUs are faster with 4-aligned reads.
		* However, a char[] is smaller, which avoids cache trashing, and that
		* is probably the most important aspect on most architectures.
		* This array is accessed a *lot* by the noise functions.
		* A vector-valued noise over 3D accesses it 96 times, and a
		* float-valued 4D noise 64 times. We want this to fit in the cache!
		*/



	};
}

#endif

