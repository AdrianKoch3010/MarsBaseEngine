#pragma once

/// @file
/// @brief Class templates mbe::AssetHolder, mbe::AssetHolder<std::string, TAsset> and specialisation typedefs

#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cassert>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <MBE/Graphics/TextureWrapper.h>
#include <MBE/Core/Utility.h>

namespace mbe
{
	/// @brief A class managing the loading and access of assets
	/// @tparam ID The type of id used to access assets (usually std::string or enum)
	/// @tparam Asset The type of asset to be managed (e.g. sf::Texture).
	/// The Asset must provide a loadFromFile() function
	template <typename TID, class TAsset>
	class AssetHolder : private sf::NonCopyable
	{
	public:
		typedef std::shared_ptr<AssetHolder<TID, TAsset>> Ptr;
		typedef std::weak_ptr<AssetHolder<TID, TAsset>> WPtr;
		typedef std::unique_ptr<AssetHolder<TID, TAsset>> UPtr;

	public:
		/// @brief Default Constructor
		AssetHolder() = default;

		/// @brief Default Destructor
		~AssetHolder() = default;

	public:
		// Rename to LoadFromFile ? --> provide other functions to load / create from other sources
		/// @brief Loads an asset from a file and adds it to the asset dictionary
		/// @details Every asset requires a file path. Any additional arguments can be passed
		/// using the variadic template.
		/// @tparam TArguments The types of the additional parameters passed to the asset's loadFromFile() method
		/// @param id The id that can later be used to access this asset
		/// @param filePath The file path from which the asset will be loaded
		/// @param arguments The additional Parameters for the asset's loadFromFile() function (e.g. fragment shader for sf::Shader)
		/// @throws std::runntime_error if the asset failed to load
		/// @throws std::runntime error if an asset with the same id already exists
		template <typename... TArguments>
		void Load(const TID& id, const std::string& filePath, TArguments&&... arguments);

		/// @brief Returns a reference to the asset with the given id
		/// @param id The id of the asset to get
		/// @throws std::runntime error when trying to get an asset that has not been loaded
		TAsset& Get(const TID& id);

		/// @brief Returns a const reference of the asset with the given id
		/// @details Const overload enabling the class to be used in when only a const version is available
		/// @param id The id of the asset to get
		/// @throws std::runntime error when trying to get an asset that has not been loaded
		const TAsset& Get(const TID& id) const;

		/// @brief An operator overload that allows for accessing assets using the index operator
		/// @param id The id of the asset to get
		/// @returns A reference to the requested asset
		/// @throws Whenever mbe::AssetHolder::Get() throws
		/// @see mbe::AssetHolder::Get()
		inline TAsset& operator [] (const TID& id) { return this->Get(id); }

		/// @brief An operator overload that allows for accessing assets using the index operator
		/// @details Const overload enabling the class to be used in when only a const version is available
		/// @param id The id of the asset to get
		/// @returns A const reference to the requested asset
		/// @throws Whenever mbe::AssetHolder::Get() const throws
		/// @see mbe::AssetHolder::Get() const
		inline const TAsset& operator [] (const TID& id) const { return this->Get(id); }

	protected:
		std::unordered_map<TID, std::unique_ptr<TAsset>> assetDictionary;
	};

#pragma region Template Overload for TID = std::string

	/// @brief A class managing the loading and access of assets
	/// @details Template overload of the mbe::AssetHolder class for TID == std::string
	/// @tparam Asset The type of asset to be managed (e.g. sf::Texture).
	/// The Asset must provide a loadFromFile() function
	/// @attention The id strings are <b>not</b> case sensitive! This is to reduce the likelyhood of mistyping an id which may cause unwanted behaviour.
	template <class TAsset>
	class AssetHolder<std::string, TAsset>
	{
	public:
		typedef std::shared_ptr<AssetHolder<std::string, TAsset>> Ptr;
		typedef std::weak_ptr<AssetHolder<std::string, TAsset>> WPtr;
		typedef std::unique_ptr<AssetHolder<std::string, TAsset>> UPtr;

	public:
		/// @brief Default Constructor
		AssetHolder() = default;

		/// @brief Default Destructor
		~AssetHolder() = default;

	public:
		/// @brief Loads an asset from a file and adds it to the asset dictionary
		/// @details Every asset requires a file path. Any additional arguments can be passed
		/// using the variadic template.
		/// @tparam TArguments The types of the additional parameters passed to the asset's loadFromFile() method
		/// @param id The id that can later be used to access this asset. It is not case sensetive so capital letters do not matter. Only use ASCII strings!
		/// @param filePath The file path from which the asset will be loaded
		/// @param arguments The additional Parameters for the asset's loadFromFile() function (e.g. fragment shader for sf::Shader)
		/// @throws std::runntime_error if the asset failed to load
		/// @throws std::runntime error if an asset with the same id already exists
		template <typename... TArguments>
		void Load(std::string id, const std::string& filePath, TArguments&&... arguments);

		/// @brief Returns a reference to the asset with the given id
		/// @param id The id of the asset to get. It is not case sensetive so capital letters do not matter. Only use ASCII strings!
		/// @throws std::runntime error when trying to get an asset that has not been loaded
		TAsset& Get(std::string id);

		/// @brief Returns a const reference of the asset with the given id
		/// @details Const overload enabling the class to be used in when only a const version is available
		/// @param id The id of the asset to get. It is not case sensetive so capital letters do not matter. Only use ASCII strings!
		/// @throws std::runntime error when trying to get an asset that has not been loaded
		const TAsset& Get(std::string id) const;

		/// @brief An operator overload that allows for accessing assets using the index operator
		/// @param id The id of the asset to get. It is not case sensetive so capital letters do not matter. Only use ASCII strings!
		/// @returns A reference to the requested asset
		/// @throws Whenever mbe::AssetHolder::Get() throws
		/// @see mbe::AssetHolder::Get()
		inline TAsset& operator [] (std::string id) { return this->Get(id); }

		/// @brief An operator overload that allows for accessing assets using the index operator
		/// @details Const overload enabling the class to be used in when only a const version is available
		/// @param id The id of the asset to get. It is not case sensetive so capital letters do not matter.
		/// @returns A const reference to the requested asset
		/// @throws Whenever mbe::AssetHolder::Get() const throws
		/// @see mbe::AssetHolder::Get() const
		inline const TAsset& operator [] (std::string id) const { return this->Get(id); }

	protected:
		std::unordered_map<std::string, std::unique_ptr<TAsset>> assetDictionary;
	};

#pragma endregion

#pragma region Template Implementations for AssetHolder<TID, TAsset>

	template<typename TID, class TAsset>
	template<typename ...TArguments>
	inline void AssetHolder<TID, TAsset>::Load(const TID& id, const std::string& filePath, TArguments&& ...arguments)
	{
		auto asset = std::make_unique<TAsset>();

		// Check for loading errors and throw appropreate exeption
		if (!asset->loadFromFile(filePath, std::forward<TArguments>(arguments)...))
			throw std::runtime_error("AssetHolder: Failed to load: " + filePath);

		// Insert the loaded asset into the map
		// std::map::insert returns false if the key already exists
		auto inserted = assetDictionary.insert(std::make_pair(id, std::move(asset)));
		if (inserted.second == false)
			throw std::runtime_error("AssetHolder: An assest with the same key already exists");
	}

	template<typename TID, class TAsset>
	inline TAsset& AssetHolder<TID, TAsset>::Get(const TID& id)
	{
		auto it = assetDictionary.find(id);
		// Throw if the id could not be found
		if (it == assetDictionary.end())
			throw std::runtime_error("AssetHolder: No asset exists under this id");

		return *(it->second);
	}

	template<typename TID, class TAsset>
	inline const TAsset& AssetHolder<TID, TAsset>::Get(const TID& id) const
	{
		auto it = assetDictionary.find(id);
		// Throw if the id could not be found
		if (it == assetDictionary.end())
			throw std::runtime_error("AssetHolder: No asset exists under this id");

		return *(it->second);
	}

#pragma endregion

#pragma region Template Implementations for AssetHolder<std::string, TAsset>

	template<class TAsset>
	template<typename ...TArguments>
	inline void AssetHolder<std::string, TAsset>::Load(std::string id, const std::string& filePath, TArguments && ...arguments)
	{
		NormaliseIDString(id);

		auto asset = std::make_unique<TAsset>();

		// Check for loading errors and throw appropreate exeption
		if (!asset->loadFromFile(filePath, std::forward<TArguments>(arguments)...))
			throw std::runtime_error("AssetHolder: Failed to load: " + filePath);

		// Insert the loaded asset into the map
		// std::map::insert returns false if the key already exists
		auto inserted = assetDictionary.insert(std::make_pair(id, std::move(asset)));
		if (inserted.second == false)
			throw std::runtime_error("AssetHolder: An assest with the same key already exists (Key = " + id + ")");
	}

	template<class TAsset>
	inline TAsset& AssetHolder<std::string, TAsset>::Get(std::string id)
	{
		NormaliseIDString(id);

		auto it = assetDictionary.find(id);
		// Throw if the id could not be found
		if (it == assetDictionary.end())
			throw std::runtime_error("AssetHolder: No asset exists under this id: " + id);

		return *(it->second);
	}

	template<class TAsset>
	inline const TAsset& AssetHolder<std::string, TAsset>::Get(std::string id) const
	{
		NormaliseIDString(id);

		auto it = assetDictionary.find(id);
		// Throw if the id could not be found
		if (it == assetDictionary.end())
			throw std::runtime_error("AssetHolder: No asset exists under this id: " + id);

		return *(it->second);
	}

#pragma endregion

#pragma region Specialisation Typedefs

	/// @brief A mbe::AssetHolder that holds mbe::TextureWrappers
	template <typename TID = std::string>
	using TextureWrapperHolder = AssetHolder<TID, TextureWrapper>;

	/// @brief A mbe::AssetHolder that holds sf::Textures
	template <typename TID = std::string>
	using TextureHolder = AssetHolder<TID, sf::Texture>;

	/// @brief A mbe::AssetHolder that hold sf::Fonts
	template <typename TID = std::string>
	using FontHolder = AssetHolder<TID, sf::Font>;

	/// @brief A mbe::AssetHolder that holds sf::Shaders
	template <typename TID = std::string>
	using ShaderHolder = AssetHolder<TID, sf::Shader>;

	/// @brief A mbe::AssetHolder that holds sf::SoundBuffers
	template <typename TID = std::string>
	using SoundBufferHolder = AssetHolder<TID, sf::SoundBuffer>;

#pragma endregion

} // namespace mbe


