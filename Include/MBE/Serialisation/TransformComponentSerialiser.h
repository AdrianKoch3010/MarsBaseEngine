#pragma once

/// @file
/// @brief class mbe::TransformComponentSerialiser

#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::TransformComponent
	/// @details XML format
	/// @code
	/// <Component type="TransformComponent">
	///		<Position x="float" y="float" />
	///		<Origin x="float y="float" />
	///		<Scale x="float" y="float" />
	///		<Rotation>float</Rotation>
	/// </Component>
	/// @endcode
	class TransformComponentSerialser : public ComponentSerialser
	{
	public:
		typedef std::shared_ptr<TransformComponentSerialser> Ptr;
		typedef std::weak_ptr<TransformComponentSerialser> WPtr;
		typedef std::unique_ptr<TransformComponentSerialser> UPtr;

	public:
		/// @brief Default constructor
		TransformComponentSerialser() = default;

		/// @brief Default destructor
		~TransformComponentSerialser() = default;

	public:

		/// @brief Loads the transform component data and adds it to the entity
		/// @details The xml element stores the position, origin, scale and rotation
		/// @param entity The entity to which the component is added
		/// @param componentData The XML element which is parsed and used to determine the value of the component
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		/// @brief Stores the entity's transform component an xml element
		/// @details Stores the position, origin, scale and rotation of the transform component
		/// @param entity The entity whos transform component is stored. This entity must have an mbe::TransformComponent
		/// @param document The xml document to which the transform component is stored to
		/// @param componentData The xml element to which the transfrom component is stored to
		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;
	};

} // namespace mbe