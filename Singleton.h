#pragma once

/// @file
/// @brief Class template mbe::detail::Singleton

namespace mbe
{
	namespace detail
	{
		/// @brief A template class for a singleton
		/// @details The class that inherits from singleton must add this as a friend
		/// (for the instance function to access the protected / private constructor).
		/// @n The Singleton is created the time the Instance() method is called.
		/// @tparam T The type of the singleton object instance
		template <typename TData>
		class Singleton
		{
		protected:
			/// @brief Protected Default constructor
			/// @details The constructor is made protected so that only a class that derives from singleton can use it.
			Singleton() = default;

		public:
			/// @brief Deleted copy constructor
			/// @details The copy constructor is deleted so that no other instance of the singleton object can be created.
			Singleton(const Singleton &) = delete;

			/// @brief Deleted copy assignment constructor
			/// @details The copy assignment constructor is deleted so that no other instance of the singleton object can be created.
			void operator =(const Singleton &) = delete;

			/// @brief Default destructor
			virtual ~Singleton() = default;

		public:
			/// @brief Method to access the singleton instance
			/// @details This should be the @b only way to get the data/instance of the singleton
			/// @returns A reference to the singleton instance
			static TData & Instance();
		};

#pragma region Template Implementation

		template <class TData>
		inline TData & Singleton<TData>::Instance()
		{
			static TData instance;
			return instance;
		}

#pragma endregion
	}
}

