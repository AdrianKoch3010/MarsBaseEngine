#pragma once

/// @file
/// @brief Class mbe::AIAction

// An action is the simplest type of AI behaviour. Its performed by an mbe::Entity and, therefore, lives in its mbe::AIComponent

#include <memory>

#include <MBE/Core/Entity.h>

namespace mbe
{

	class AIAction
	{
	public:
		typedef std::shared_ptr<AIAction> Ptr;
		typedef std::weak_ptr<AIAction> WPtr;
		typedef std::unique_ptr<AIAction> UPtr;

	public:
		AIAction();
		virtual ~AIAction() = default;

	public:
		virtual void InitialActions() = 0;

		// Maybe add virtual custom update method. The actual update method calls initial actions the first time it is called

		virtual void Update() = 0;

		// Get the handle id of the entity the action belongs to
		virtual Entity::HandleID GetHandleID() const = 0;

		inline bool IsCompleted() const { return completed; }

		inline void SetCompleted(bool value = true) { completed = value; }

	protected:
		bool completed;
	};

} // namespace mbe