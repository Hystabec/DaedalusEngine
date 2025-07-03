#include "ddpch.h"
#include "sceneSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "entity.h"
#include "components.h"

namespace YAML {

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
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "123456789"; // TO DO: Entity ID goes here

		if (entity.hasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << entity.getComponent<TagComponent>().Tag;
			out << YAML::EndMap;
		}

		serialize_component<TransformComponent>(out, "TransformComponent", entity, [](YAML::Emitter& out, TransformComponent& tc)
			{
				out << YAML::Key << "Position" << YAML::Value << tc.Position;
				out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
				out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			});

		serialize_component<CameraComponent>(out, "CameraComponent", entity, [](YAML::Emitter& out, CameraComponent& cc)
			{
				auto& camera = cc.Camera;

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

				out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
				out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;
			});

		serialize_component<SpriteRendererComponent>(out, "SpriteRendererComponent", entity, [](YAML::Emitter& out, SpriteRendererComponent& src)
			{
				out << YAML::Key << "Colour" << YAML::Value << src.Colour;
			});

		out << YAML::EndMap;
	}

	void SceneSerializer::serialize(const std::string& filepath)
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

	void SceneSerializer::serializeRuntime(const std::string& filepath)
	{
		// TO DO: Implement
		DD_CORE_ASSERT(false, "Not yet implemented");
	}

	bool SceneSerializer::deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		DD_CORE_LOG_TRACE("Deserializing scene {}", sceneName);

		auto entites = data["Entities"];
		if (entites)
		{
			for (auto entity : entites)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TO DO: add unique ids

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				DD_CORE_LOG_TRACE("Deserialized entity with ID = {}, name = {}", uuid, name);

				Entity deserializedEntity = m_scene->createEntity(name);

				auto component = entity["TransformComponent"];
				if (component)
				{
					auto& tc = deserializedEntity.addOrRepalaceComponent<TransformComponent>();
					tc.Position = component["Position"].as<maths::Vec3>();
					tc.Rotation = component["Rotation"].as<maths::Vec3>();
					tc.Scale = component["Scale"].as<maths::Vec3>();
				}

				component = entity["CameraComponent"];
				if (component)
				{
					auto& cc = deserializedEntity.addOrRepalaceComponent<CameraComponent>();
					auto cameraProps = component["Camera"];
					cc.Camera.setProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.setOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.setOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = component["Primary"].as<bool>();
					cc.FixedAspectRatio = component["FixedAspectRatio"].as<bool>();
				}

				component = entity["SpriteRendererComponent"];
				if (component)
				{
					auto& src = deserializedEntity.addOrRepalaceComponent<SpriteRendererComponent>();
					src.Colour = component["Colour"].as<maths::Vec4>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::deserializeRuntime(const std::string& filepath)
	{
		// TO DO: Implement
		DD_CORE_ASSERT(false, "Not yet implemented");
		return false;
	}

}