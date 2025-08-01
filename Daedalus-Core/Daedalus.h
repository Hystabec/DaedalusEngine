#pragma once

//for use by application
#include "src/core.h"

// ---application
#include "src/application/applicationCore.h"

#include "src/application/time/time.h"
#include "src/application/time/deltaTime.h"

#include "src/application/layers/layer.h"
#include "src/application/layers/imGuiLayer.h"

#include "src/application/input/input.h"
#include "src/application/input/inputCodes.h"

#include "src/application/uuid.h"

// ---events
#include "src/events/keyEvent.h"
#include "src/events/mouseEvent.h"
#include "src/events/windowEvent.h"

// ---graphics
#include "src/graphics/renderer/renderer.h"
#include "src/graphics/renderer/renderer2D.h"
#include "src/graphics/renderer/renderCommands.h"

#include "src/graphics/renderer/framebuffer.h"

#include "src/graphics/shaders/shaderLibrary.h"
#include "src/graphics/shaders/shader.h"
#include "src/graphics/rendering/texture.h"
#include "src/graphics/rendering/subTexture2D.h"

#include "src/graphics/buffers/vertexBuffer.h"
#include "src/graphics/buffers/indexBuffer.h"
#include "src/graphics/buffers/vertexArray.h"

#include "src/graphics/camera/orthographicCamera.h"
#include "src/graphics/camera/orthographicCameraController.h"

// ---maths
#include "src/maths/maths.h"

// ---Scene
#include "src/scene/scene.h"
#include "src/scene/entityComponents/components.h"
#include "src/scene/entity.h"
#include "src/scene/scriptableEntity.h"

// ---utils
#include "src/utils/timer.h"
#include "src/utils/colourConversionUtils.h"
#include "src/utils/fileUtils.h"

// ---debug
#include "src/debugTools/logging/log.h"
#include "src/debugTools/logging/logFormatMacro.h"
#include "src/debugTools/profiling/instrumentor.h"