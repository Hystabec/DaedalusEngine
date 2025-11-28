#pragma once

// Standard components
#include "idComponent.h"
#include "tagComponent.h"
#include "transformComponent.h"

// Scripting components
#include "nativeScriptComponent.h"
#include "scriptComponent.h"

// Graphics components
#include "cameraComponent.h"
#include "spriteRendererComponent.h"
#include "circleRendererComponent.h"
#include "textComponent.h"

// Physics components
#include "rigidbody2DComponent.h"
#include "boxCollider2DComponent.h"
#include "circleCollider2DComponent.h"

namespace daedalus::scene {

	template<typename... Components>
	struct ComponentGroup
	{
	};

	using AllComponents = 
		ComponentGroup<IDComponent, TagComponent, TransformComponent,
		NativeScriptComponent, ScriptComponent,
		CameraComponent, SpriteRendererComponent, CircleRendererComponent, TextComponent,
		Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;

	/// @brief A group for all components (minus ID component). As when duplicating entities
	/// i dont want to copy the UUID or other unique ids
	using AllModifiableComponents =
		ComponentGroup<TagComponent, TransformComponent, 
		NativeScriptComponent, ScriptComponent,
		CameraComponent, SpriteRendererComponent, CircleRendererComponent, TextComponent,
		Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;

	using AllOptionalComponents =
		ComponentGroup<NativeScriptComponent, ScriptComponent,
		CameraComponent, SpriteRendererComponent, CircleRendererComponent, TextComponent,
		Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;

}