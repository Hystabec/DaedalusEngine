#include "ddpch.h"
#include "sceneSerializer.h"

#include "entity.h"
#include "entityComponents/components.h"
#include "../scripting/scriptEngine.h"
#include "application/uuid.h"
#include "../project/project.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#define LOG_SERIALIZATION_TO_CONSOLE 0

namespace YAML {

	template<>
	struct convert<daedalus::maths::Vec2>
	{
		static Node encode(const daedalus::maths::Vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, daedalus::maths::Vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<daedalus::maths::Vec3>
	{
		static Node encode(const daedalus::maths::Vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, daedalus::maths::Vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<daedalus::maths::Vec4>
	{
		static Node encode(const daedalus::maths::Vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, daedalus::maths::Vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<daedalus::UUID>
	{
		static Node encode(const daedalus::UUID& rhs)
		{
			Node node;
			node.push_back((uint64_t)rhs);
			return node;
		}

		static bool decode(const Node& node, daedalus::UUID& rhs)
		{
			if (!node.IsScalar())
				return false;

			rhs = node.as<uint64_t>();
			return true;
		}
	};

}

namespace daedalus::scene {

	static YAML::Emitter& operator<<(YAML::Emitter& out, const maths::Vec2& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
		return out;
	}

	static YAML::Emitter& operator<<(YAML::Emitter& out, const maths::Vec3& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
	}

	static YAML::Emitter& operator<<(YAML::Emitter& out, const maths::Vec4& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;
	}

	static std::string rigidbody2D_body_type_to_string(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:
			return "Static";
		case Rigidbody2DComponent::BodyType::Dynamic:
			return "Dynamic";
		case Rigidbody2DComponent::BodyType::Kinematic:
			return "Kinematic";
		}

		DD_CORE_ASSERT(false, "Unknown type");
		return {};
	}

	static Rigidbody2DComponent::BodyType rigidbody2D_body_type_from_string(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")
			return Rigidbody2DComponent::BodyType::Static;
		else if(bodyTypeString == "Dynamic")
			return Rigidbody2DComponent::BodyType::Dynamic;
		else if (bodyTypeString == "Kinematic")
			return Rigidbody2DComponent::BodyType::Kinematic;
		
		DD_CORE_ASSERT(false, "Unknown type");
		return Rigidbody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Shr_ptr<Scene>& scene)
		: m_scene(scene)
	{
	}

	template<typename T, typename componentFunction>
	void serialize_component(YAML::Emitter& out, const std::string& componentName, Entity& entity, componentFunction func)
	{
		if (!entity.hasComponent<T>())
			return;
		
		out << YAML::Key << componentName;
		out << YAML::BeginMap;

		func(out, entity.getComponent<T>());

		out << YAML::EndMap;
	}

	static void serialize_entity(YAML::Emitter& out, Entity& entity)
	{
		DD_CORE_ASSERT(entity.hasComponent<IDComponent>());

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.getUUID();
		out << YAML::Key << "Name" << YAML::Value << entity.getName();

		if (entity.hasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << entity.getComponent<TagComponent>().tag;
			out << YAML::EndMap;
		}

		serialize_component<TransformComponent>(out, "TransformComponent", entity, [](YAML::Emitter& out, TransformComponent& tc)
			{
				out << YAML::Key << "Position" << YAML::Value << tc.position;
				out << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
				out << YAML::Key << "Scale" << YAML::Value << tc.scale;
			});

		serialize_component<ScriptComponent>(out, "ScriptComponent", entity, [entity](YAML::Emitter& out, ScriptComponent& sc)
			{
				out << YAML::Key << "ClassName" << YAML::Value << sc.className;

				//Fields
				using namespace scripting;
				auto entityClass = ScriptEngine::getEntityClass(sc.className);
				const auto& fields = entityClass->getFields();
				
				if(fields.size() > 0)
				{
					out << YAML::Key << "ScriptFields" << YAML::Value;
					out << YAML::BeginSeq;
					auto& entityFields = scripting::ScriptEngine::getEntityScriptFields(entity.getUUID());
					for (const auto& [name, field] : fields)
					{
						if (entityFields.find(name) == entityFields.end())
							continue;

						out << YAML::BeginMap;
						out << YAML::Key << "Name" << YAML::Value << name;
						out << YAML::Key << "Type" << YAML::Value << conversions::script_field_type_to_string(field.type);

						out << YAML::Key << "Data" << YAML::Value;
						scripting::ScriptFieldInstance& scriptField = entityFields.at(name);

#define FIELD_TYPE_MACRO(FieldType, Type) case ScriptFieldType::FieldType:\
						out << scriptField.getFieldValue<Type>();\
						break;

						switch (field.type)
						{
						FIELD_TYPE_MACRO(Bool, bool)
						FIELD_TYPE_MACRO(Float, float)
						FIELD_TYPE_MACRO(Double, double)
						FIELD_TYPE_MACRO(Char, char)
						FIELD_TYPE_MACRO(Byte, byte)
						FIELD_TYPE_MACRO(Short, short)
						FIELD_TYPE_MACRO(UShort, uint16_t)
						FIELD_TYPE_MACRO(Int, int)
						FIELD_TYPE_MACRO(UInt, uint32_t)
						FIELD_TYPE_MACRO(Long, long)
						FIELD_TYPE_MACRO(ULong, uint64_t)
						FIELD_TYPE_MACRO(String, std::string)
						FIELD_TYPE_MACRO(Vector2, maths::Vec2)
						FIELD_TYPE_MACRO(Vector3, maths::Vec3)
						FIELD_TYPE_MACRO(Vector4, maths::Vec4)
						FIELD_TYPE_MACRO(MonoScript, UUID)
						}
#undef FIELD_TYPE_MACRO

						out << YAML::EndMap;
					}
					out << YAML::EndSeq;
				}
			});

		serialize_component<CameraComponent>(out, "CameraComponent", entity, [](YAML::Emitter& out, CameraComponent& cc)
			{
				auto& camera = cc.camera;

				out << YAML::Key << "Camera" << YAML::Value;
				out << YAML::BeginMap;
				out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
				out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
				out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
				out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
				out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
				out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
				out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
				out << YAML::EndMap;

				out << YAML::Key << "Primary" << YAML::Value << cc.primary;
				out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.fixedAspectRatio;
			});

		serialize_component<SpriteRendererComponent>(out, "SpriteRendererComponent", entity, [](YAML::Emitter& out, SpriteRendererComponent& src)
			{
				out << YAML::Key << "Colour" << YAML::Value << src.colour;

				out << YAML::Key << "Material" << YAML::Value;
				out << YAML::BeginMap;
				out << YAML::Key << "TextureHandle" << YAML::Value << src.material.texture;
				out << YAML::Key << "TilingFactor" << YAML::Value << src.material.tilingFactor;
				out << YAML::EndMap;
 			});

		serialize_component<CircleRendererComponent>(out, "CircleRendererComponent", entity, [](YAML::Emitter& out, CircleRendererComponent& src)
			{
				out << YAML::Key << "Colour" << YAML::Value << src.colour;
				out << YAML::Key << "Thickness" << YAML::Value << src.thickness;
				out << YAML::Key << "Fade" << YAML::Value << src.fade;
			});

		serialize_component<TextComponent>(out, "TextComponent", entity, [](YAML::Emitter& out, TextComponent& src)
			{
				out << YAML::Key << "Text" << YAML::Value << src.text;
				// TO DO: fontAsset
				out << YAML::Key << "Colour" << YAML::Value << src.colour;
				out << YAML::Key << "Kerning" << YAML::Value << src.kerning;
				out << YAML::Key << "LineSpacing" << YAML::Value << src.lineSpacing;
			});

		serialize_component<Rigidbody2DComponent>(out, "Rigidbody2DComponent", entity, [](YAML::Emitter& out, Rigidbody2DComponent& src)
			{
				out << YAML::Key << "BodyType" << YAML::Value << rigidbody2D_body_type_to_string(src.type);
				out << YAML::Key << "FixedRotation" << YAML::Value << src.fixedRotation;
				out << YAML::Key << "Desity" << YAML::Value << src.desity;
				out << YAML::Key << "Friction" << YAML::Value << src.friction;
				out << YAML::Key << "Restitution" << YAML::Value << src.restitution;
			});

		serialize_component<BoxCollider2DComponent>(out, "BoxCollider2DComponent", entity, [](YAML::Emitter& out, BoxCollider2DComponent& src)
			{
				out << YAML::Key << "Offset" << YAML::Value << src.offset;
				out << YAML::Key << "Size" << YAML::Value << src.size;
			});

		serialize_component<CircleCollider2DComponent>(out, "CircleCollider2DComponent", entity, [](YAML::Emitter& out, CircleCollider2DComponent& src)
			{
				out << YAML::Key << "Offset" << YAML::Value << src.offset;
				out << YAML::Key << "Radius" << YAML::Value << src.radius;
			});

		out << YAML::EndMap;
	}

	void SceneSerializer::serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "FormatVersion" << YAML::Value << m_currentFileFormatVersion;
		out << YAML::Key << "Scene" << YAML::Value << filepath.stem().string();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (auto entityID : m_scene->m_registry.view<entt::entity>())
		{
			Entity entity = { entityID, m_scene.get() };
			if (!entity)
				return;

			serialize_entity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::serializeRuntime(const std::filesystem::path& filepath)
	{
		// TO DO: Implement
		DD_CORE_ASSERT(false, "Not yet implemented");
	}

	bool SceneSerializer::deserialize(const std::filesystem::path& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::Exception e)
		{
			DD_CORE_LOG_ERROR("Failed to load scene file '{}'\n{}", filepath, e.what());
			return false;
		}

		// if the file is of a different format (test with a renamed .png and exception will be thrown when trying to read scene data)
		try
		{
			if (!data["Scene"])
			{
				DD_CORE_LOG_ERROR("Deserialization Error: file contains no scene data '{}'", filepath);
				return false;
			}
		}
		catch (YAML::Exception e)
		{
			DD_CORE_LOG_ERROR("Failed to load scene file '{}'\n{}", filepath, e.what());
			return false;
		}

		uint32_t fileVersion = data["FormatVersion"].as<uint32_t>();
		std::string sceneName = data["Scene"].as<std::string>();
#if LOG_SERIALIZATION_TO_CONSOLE
		DD_CORE_LOG_TRACE("Deserializing scene {}", sceneName);
#endif

		auto entites = data["Entities"];
		if (entites)
		{
			for (auto entity : entites)
			{
				UUID uuid = entity["Entity"].as<UUID>();
				std::string name = entity["Name"].as<std::string>();


#if LOG_SERIALIZATION_TO_CONSOLE
				DD_CORE_LOG_TRACE("Deserialized entity with ID = {}, name = {}", uuid, name);
#endif

				Entity deserializedEntity = m_scene->createEntityWithUUID(uuid, name);

				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					deserializedEntity.getComponent<TagComponent>().tag = tagComponent["Tag"].as<std::string>();

				auto component = entity["TransformComponent"];
				if (component)
				{
					auto& tc = deserializedEntity.addOrRepalaceComponent<TransformComponent>();
					tc.position = component["Position"].as<maths::Vec3>();
					tc.rotation = component["Rotation"].as<maths::Vec3>();
					tc.scale = component["Scale"].as<maths::Vec3>();
				}

				component = entity["ScriptComponent"];
				if (component)
				{
					auto& sc = deserializedEntity.addOrRepalaceComponent<ScriptComponent>();
					sc.className = component["ClassName"].as<std::string>();
					using namespace scripting;
					const auto& scriptFiels = component["ScriptFields"];
					if (scriptFiels)
					{
						Shr_ptr<ScriptClass> entityClass = ScriptEngine::getEntityClass(sc.className);
						if (entityClass)
						{
							const auto& fields = entityClass->getFields();
							auto& entityFields = ScriptEngine::getEntityScriptFields(deserializedEntity.getUUID());

							for (auto& scriptFeild : scriptFiels)
							{

								std::string fieldName = scriptFeild["Name"].as<std::string>();
								std::string typeAsString = scriptFeild["Type"].as<std::string>();
								scripting::ScriptFieldType fieldType = conversions::script_field_type_from_string(typeAsString);

								ScriptFieldInstance& fieldInstance = entityFields[fieldName];
								if (!fields.contains(fieldName))
								{
									DD_LOG_WARN("Entity '{}', Script component field: '{}' not found, while deserializing '{}'", name, fieldName, filepath.filename());
									continue;
								}

								fieldInstance.field = fields.at(fieldName);

#define FIELD_TYPE_MACRO(FieldType, Type) case ScriptFieldType::FieldType:\
							{\
							Type readData = scriptFeild["Data"].as<Type>();\
							fieldInstance.setFieldValue(readData);\
							break;\
							}

								switch (fieldType)
								{
									FIELD_TYPE_MACRO(Bool, bool)
										FIELD_TYPE_MACRO(Float, float)
										FIELD_TYPE_MACRO(Double, double)
										FIELD_TYPE_MACRO(Char, char)
										FIELD_TYPE_MACRO(Byte, byte)
										FIELD_TYPE_MACRO(Short, short)
										FIELD_TYPE_MACRO(UShort, uint16_t)
										FIELD_TYPE_MACRO(Int, int)
										FIELD_TYPE_MACRO(UInt, uint32_t)
										FIELD_TYPE_MACRO(Long, long)
										FIELD_TYPE_MACRO(ULong, uint64_t)
										FIELD_TYPE_MACRO(String, std::string)
										FIELD_TYPE_MACRO(Vector2, maths::Vec2)
										FIELD_TYPE_MACRO(Vector3, maths::Vec3)
										FIELD_TYPE_MACRO(Vector4, maths::Vec4)
										FIELD_TYPE_MACRO(MonoScript, UUID)
								}
#undef FIELD_TYPE_MACRO

							}
						}
					}
				}

				component = entity["CameraComponent"];
				if (component)
				{
					auto& cc = deserializedEntity.addOrRepalaceComponent<CameraComponent>();
					auto cameraProps = component["Camera"];
					cc.camera.setProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.camera.setOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.camera.setOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.primary = component["Primary"].as<bool>();
					cc.fixedAspectRatio = component["FixedAspectRatio"].as<bool>();
				}

				component = entity["SpriteRendererComponent"];
				if (component)
				{
					auto& src = deserializedEntity.addOrRepalaceComponent<SpriteRendererComponent>();
					src.colour = component["Colour"].as<maths::Vec4>();
					auto material = component["Material"];

					if (material["TextureHandle"])
						src.material.texture = material["TextureHandle"].as<AssetHandle>();
					
					if(material["TilingFactor"])
						src.material.tilingFactor = material["TilingFactor"].as<float>();
				}

				component = entity["CircleRendererComponent"];
				if (component)
				{
					auto& src = deserializedEntity.addOrRepalaceComponent<CircleRendererComponent>();
					src.colour = component["Colour"].as<maths::Vec4>();
					src.thickness = component["Thickness"].as<float>();
					src.fade = component["Fade"].as<float>();
				}

				component = entity["TextComponent"];
				if (component)
				{
					auto& src = deserializedEntity.addOrRepalaceComponent<TextComponent>();
					src.text = component["Text"].as<std::string>();
					// TO DO: FontAsset
					src.colour = component["Colour"].as<maths::Vec4>();
					src.kerning = component["Kerning"].as<float>();
					src.lineSpacing = component["LineSpacing"].as<float>();
				}

				component = entity["Rigidbody2DComponent"];
				if (component)
				{
					auto& src = deserializedEntity.addOrRepalaceComponent<Rigidbody2DComponent>();
					src.type = rigidbody2D_body_type_from_string(component["BodyType"].as<std::string>());
					src.fixedRotation = component["FixedRotation"].as<bool>();

					src.desity = component["Desity"].as<float>();
					src.friction = component["Friction"].as<float>();
					src.restitution = component["Restitution"].as<float>();
				}

				component = entity["BoxCollider2DComponent"];
				if (component)
				{
					auto& src = deserializedEntity.addOrRepalaceComponent<BoxCollider2DComponent>();
					src.offset = component["Offset"].as<maths::Vec2>();
					src.size = component["Size"].as<maths::Vec2>();
				}

				component = entity["CircleCollider2DComponent"];
				if (component)
				{
					auto& src = deserializedEntity.addOrRepalaceComponent<CircleCollider2DComponent>();
					src.offset = component["Offset"].as<maths::Vec2>();
					src.radius = component["Radius"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::deserializeRuntime(const std::filesystem::path& filepath)
	{
		// TO DO: Implement
		DD_CORE_ASSERT(false, "Not yet implemented");
		return false;
	}

}