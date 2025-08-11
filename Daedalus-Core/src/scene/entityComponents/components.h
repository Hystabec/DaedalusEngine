#pragma once

// Standard components
#include "idComponent.h"
#include "tagComponent.h"
#include "transformComponent.h"

// Graphics components
#include "cameraComponent.h"
#include "spriteRendererComponent.h"
#include "circleRendererComponent.h"

// Physics components
#include "rigidbody2DComponent.h"
#include "boxCollider2DComponent.h"
#include "circleCollider2DComponent.h"

// Scripting components
#include "nativeScriptComponent.h"

namespace daedalus::scene {

	template<typename... Components>
	struct ComponentGroup
	{
	};

	using AllComponents = 
		ComponentGroup<IDComponent, TagComponent, TransformComponent,
		CameraComponent, SpriteRendererComponent, CircleRendererComponent,
		Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent,
		NativeScriptComponent>;

	using AllOptionalComponents =
		ComponentGroup<CameraComponent, SpriteRendererComponent, CircleRendererComponent,
		Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent,
		NativeScriptComponent>;

}