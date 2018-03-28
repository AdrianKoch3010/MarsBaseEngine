#pragma once

/// @file
/// @brief Abstract Template Class mbe::BaseAnimator

#include <memory>
#include <string>

#include <SFML/System/Time.hpp>

#include <MBE/Core/HandleBase.h>

namespace mbe
{
	template <typename TID = std::string>
	class BaseAnimator
	{
	public:
		typedef std::shared_ptr<BaseAnimator> Ptr;
		typedef std::weak_ptr<BaseAnimator> WPtr;
		typedef std::unique_ptr<BaseAnimator> UPtr;

	public:
		BaseAnimator() = default;
		virtual ~BaseAnimator() = default;

	public:
		virtual void Update(sf::Time frameTime) = 0;

		virtual void PlayAnimation(const TID & id, bool loop = false) = 0;
		virtual void SetPaused(bool value = true) = 0;
		virtual void StopAnimation() = 0;

		virtual bool IsPlayingAnimation() const = 0;
		virtual bool IsPaused() const = 0;
		virtual const TID & GetPlayingAnimation() const = 0;

		virtual bool HasAnimation(const TID & id) const = 0;

		virtual void Destroy() = 0;
		virtual bool IsActive() const = 0;
	};
} // namespace mbe