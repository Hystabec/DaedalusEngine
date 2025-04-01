#pragma once

//for use by application
#include "src/core.h"
#include "src/application/applicationCore.h"

#include "src/application/time/time.h"
#include "src/application/time/deltaTime.h"

#include "src/application/layers/layer.h"
#include "src/application/layers/imGuiLayer.h"

#include "src/application/input/Input.h"
#include "src/application/input/inputKeyCodes.h"
#include "src/application/input/inputMouseCodes.h"

#include "src/events/keyEvent.h"
#include "src/events/mouseEvent.h"
#include "src/events/windowEvent.h"

#include "src/graphics/renderer/renderer.h"
#include "src/graphics/renderer/renderCommands.h"

#include "src/graphics/shaders/shader.h"
#include "src/graphics/rendering/texture.h"

#include "src/graphics/buffers/vertexBuffer.h"
#include "src/graphics/buffers/indexBuffer.h"
#include "src/graphics/buffers/vertexArray.h"

#include "src/graphics/camera/orthographicCamera.h"


#include "src/debugTools/logging/log.h"

#include "maths/maths.h"

#include "src/utils/timer.h"
#include "src/utils/colourConversionUtils.h"
#include "src/utils/fileUtils.h"


//--ENTRY POINT--
#include "src/entryPoint.h"
//_______________