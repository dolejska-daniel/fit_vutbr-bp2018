///
/// @file IHasRectBounds.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
/// Implementace založena na https://www.codeproject.com/Articles/30535/A-Simple-QuadTree-Implementation-in-C
///
#pragma once
#include <Utils/RectBounds.h>


namespace Utils
{
	///
	/// @brief
	///
	class IHasRectBounds
	{
	public:
		explicit IHasRectBounds(const RectBounds& rectBounds)
			: _rectBounds(rectBounds)
		{
		}

		RectBounds GetRectBounds() const { return _rectBounds; }

	protected:
		RectBounds _rectBounds;
	};
}
