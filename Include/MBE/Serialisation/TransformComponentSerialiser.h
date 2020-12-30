#pragma once

/// @file
/// @brief class mbe::TransformComponentSerialiser

#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::TransformComponent
	/// @details The defaults are the same as the mbe::TransformComponent itself
	/// @n@n **XML format**
	/// @code
	/// <Component type="TransformComponent">
	///		<!-- optional - default: x=0 y=0 -->
	///		<Position x="float" y="float" />
	///		<!-- optional - default: x=0 y=0 -->
	///		<Origin x="float y="float" />
	///		<!-- optional - default: x=1 y=1 -->
	///		<Scale x="float" y="float" />
	///		<!-- optional - default: 0 -->
	///		<Rotation>float</Rotation>
	/// </Component>
	/// @endcode
	class TransformComponentSerialiser : public ComponentSerialiser
	{
	public:
		typedef std::shared_ptr<TransformComponentSerialiser> Ptr;
		typedef std::weak_ptr<TransformComponentSerialiser> WPtr;
		typedef std::unique_ptr<TransformComponentSerialiser> UPtr;

	public:
		/// @brief Default constructor
		TransformComponentSerialiser() = default;

		/// @brief Default destructor
		~TransformComponentSerialiser() = default;

	public:

		/// @brief Loads the transform component data and adds it to the entity
		/// @details The XML element stores the position, origin, scale and rotation
		/// @param entity The entity to which the transform component is added
		/// @param componentData The XML element being parsed
		/// @throws std::runtime_error if the parsing fails
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const override;

		/// @brief Stores the entity's transform component to an XML element
		/// @details Stores the position, origin, scale and rotation of the transform component
		/// @param entity The entity whos transform component is stored. This entity must have an mbe::TransformComponent
		/// @param document The XML document to which the transform component is stored
		/// @param componentData The XML element to which the transfrom component is stored
		/// @throws std::runtime_error if the entity doesn't have an mbe::TransformComponent
		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const override;
	};

} // namespace mbe