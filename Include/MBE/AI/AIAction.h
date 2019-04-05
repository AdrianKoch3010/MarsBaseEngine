#pragma once


namespace mbe
{

	class AIAction
	{
	public:
		AIAction();
		virtual ~AIAction() = default;

	public:
		virtual void InitialActions() = 0;

		virtual void Update() = 0;

		inline bool IsCompleted() const { return completed; }

	private:
		bool completed;
	};

} // namespace mbe