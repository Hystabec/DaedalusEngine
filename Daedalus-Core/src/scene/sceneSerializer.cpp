#include "ddpch.h"
#include "sceneSerializer.h"

#include "entity.h"
#include "entityComponents/components.h"

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

	template<typename T>
	void serialize_component(YAML::Emitter& out, const std::string& componentName, Entity& entity, void(*func)(YAML::Emitter&, T&))
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
			});

		serialize_component<CircleRendererComponent>(out, "CircleRendererComponent", entity, [](YAML::Emitter& out, CircleRendererComponent& src)
			{
				out << YAML::Key << "Colour" << YAML::Value << src.colour;
				out << YAML::Key << "Thickness" << YAML::Value << src.thickness;
				out << YAML::Key << "Fade" << YAML::Value << src.fade;
			});

		serialize_component<Rigidbody2DComponent>(out, "Rigidbody2DComponent", entity, [](YAML::Emitter& out, Rigidbody2DComponent& src)
			{
				out << YAML::Key << "BodyType" << YAML::Value << rigidbody2D_body_type_to_string(src.type);
				out << YAML::Key << "FixedRotation" << YAML::Value << src.fixedRotation;
			});

		serialize_component<BoxCollider2DComponent>(out, "BoxCollider2DComponent", entity, [](YAML::Emitter& out, BoxCollider2DComponent& src)
			{
				out << YAML::Key << "Offset" << YAML::Value << src.offset;
				out << YAML::Key << "Size" << YAML::Value << src.size;
				out << YAML::Key << "Desity" << YAML::Value << src.desity;
				out << YAML::Key << "Friction" << YAML::Value << src.friction;
				out << YAML::Key << "Restitution" << YAML::Value << src.restitution;
			});

		serialize_component<CircleCollider2DComponent>(out, "CircleCollider2DComponent", entity, [](YAML::Emitter& out, CircleCollider2DComponent& src)
			{
				out << YAML::Key << "Offset" << YAML::Value << src.offset;
				out << YAML::Key << "Radius" << YAML::Value << src.radius;
				out << YAML::Key << "Desity" << YAML::Value << src.desity;
				out << YAML::Key << "Friction" << YAML::Value << src.friction;
				out << YAML::Key << "Restitution" << YAML::Value << src.restitution;
			});

		out << YAML::EndMap;
	}

	void SceneSerializer::serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
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
		// I dont really like try statements
		// but this was the easiest ways to handle a few issues 

		// TO DO: Either remove the try block and do some error checking before hand
		// or expand the catch statement to give a reason why the deserializtion failed
		try
		{
			std::ifstream stream(filepath);
			std::stringstream strStream;
			strStream << stream.rdbuf();

			YAML::Node data = YAML::Load(strStream.str());
			if (!data["Scene"])
			{
				DD_CORE_LOG_ERROR("Deserialization Error: file contains no scene data: {}", filepath.string());
				return false;
			}

			std::string sceneName = data["Scene"].as<std::string>();
#if LOG_SERIALIZATION_TO_CONSOLE
			DD_CORE_LOG_TRACE("Deserializing scene {}", sceneName);
#endif

			auto entites = data["Entities"];
			if (entites)
			{
				for (auto entity : entites)
				{
					uint64_t uuid = entity["Entity"].as<uint64_t>();

					std::string name;
					auto tagComponent = entity["TagComponent"];
					if (tagComponent)
						name = tagComponent["Tag"].as<std::string>();

#if LOG_SERIALIZATION_TO_CONSOLE
					DD_CORE_LOG_TRACE("Deserialized entity with ID = {}, name = {}", uuid, name);
#endif

					Entity deserializedEntity = m_scene->createEntityWithUUID(uuid, name);

					auto component = entity["TransformComponent"];
					if (component)
					{
						auto& tc = deserializedEntity.addOrRepalaceComponent<TransformComponent>();
						tc.position = component["Position"].as<maths::Vec3>();
						tc.rotation = component["Rotation"].as<maths::Vec3>();
						tc.scale = component["Scale"].as<maths::Vec3>();
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
					}

					component = entity["CircleRendererComponent"];
					if (component)
					{
						auto& src = deserializedEntity.addOrRepalaceComponent<CircleRendererComponent>();
						src.colour = component["Colour"].as<maths::Vec4>();
						src.thickness = component["Thickness"].as<float>();
						src.fade = component["Fade"].as<float>();
					}

					component = entity["Rigidbody2DComponent"];
					if (component)
					{
						auto& src = deserializedEntity.addOrRepalaceComponent<Rigidbody2DComponent>();
						src.type = rigidbody2D_body_type_from_string(component["BodyType"].as<std::string>());
						src.fixedRotation = component["FixedRotation"].as<bool>();
					}

					component = entity["BoxCollider2DComponent"];
					if (component)
					{
						auto& src = deserializedEntity.addOrRepalaceComponent<BoxCollider2DComponent>();
						src.offset = component["Offset"].as<maths::Vec2>();
						src.size = component["Size"].as<maths::Vec2>();

						src.desity = component["Desity"].as<float>();
						src.friction = component["Friction"].as<float>();
						src.restitution = component["Restitution"].as<float>();
					}

					component = entity["CircleCollider2DComponent"];
					if (component)
					{
						auto& src = deserializedEntity.addOrRepalaceComponent<CircleCollider2DComponent>();
						src.offset = component["Offset"].as<maths::Vec2>();
						src.radius = component["Radius"].as<float>();

						src.desity = component["Desity"].as<float>();
						src.friction = component["Friction"].as<float>();
						src.restitution = component["Restitution"].as<float>();
					}
				}
			}

			return true;
		}
		catch (...)
		{
			DD_CORE_LOG_ERROR("Deserialization Error: Could not deserialize scene: {}", filepath.string());
			return false;
		}
	}

	bool SceneSerializer::deserializeRuntime(const std::filesystem::path& filepath)
	{
		// TO DO: Implement
		DD_CORE_ASSERT(false, "Not yet implemented");
		return false;
	}

}