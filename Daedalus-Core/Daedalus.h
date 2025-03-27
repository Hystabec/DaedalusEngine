#pragma once

//for use by application
#include "src/core.h"
#include "src/applicationCore.h"

#include "src/application/time.h"
#include "src/application/deltaTime.h"

#include "src/application/layers/layer.h"
#include "src/application/layers/imGuiLayer.h"

#include "src/application/Input.h"
#include "src/application/inputKeyCodes.h"
#include "src/application/inputMouseCodes.h"

#include "src/events/keyEvent.h"
#include "src/events/mouseEvent.h"
#include "src/events/windowEvent.h"

#include "src/graphics/renderer/renderer.h"
#include "src/graphics/renderer/renderCommands.h"

#include "src/graphics/shader.h"
#include "src/graphics/buffers/vertexBuffer.h"
#include "src/graphics/buffers/indexBuffer.h"
#include "src/graphics/buffers/vertexArray.h"
#include "src/graphics/camera/orthographicCamera.h"


#include "src/debugTools/logging/log.h"


#include "maths/maths.h"

#include "src/utils/timer.h"
#include "src/utils/colourConversionUtils.h"


//--ENTRY POINT--
#include "src/entryPoint.h"
//_______________