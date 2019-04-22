#pragma once

#include <memory>

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

		virtual void Update() = 0;

		inline bool IsCompleted() const { return completed; }

		inline void SetCompleted(bool value = true) { completed = value; }

	protected:
		bool completed;
	};

} // namespace mbe