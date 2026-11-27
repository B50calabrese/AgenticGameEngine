#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

#include "engine/core/assert.h"
#include "engine/core/base.h"
#include "engine/core/clock.h"
#include "engine/core/log.h"
#include "engine/core/timestep.h"
#include "engine/core/version.h"
#include "engine/input/input.h"
#include "engine/input/key_codes.h"
#include "engine/input/mouse_button_codes.h"
#include "engine/math/math_utils.h"
#include "engine/math/matrix4.h"
#include "engine/math/transform.h"
#include "engine/math/vector2.h"
#include "engine/math/vector3.h"
#include "engine/math/vector4.h"
#include "engine/platform/window.h"
#include "engine/renderer/buffer.h"
#include "engine/renderer/color.h"
#include "engine/renderer/primitive_renderer.h"
#include "engine/renderer/render_command.h"
#include "engine/renderer/render_queue.h"
#include "engine/renderer/shader.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/vertex_array.h"
#include "engine/ui/rect.h"
#include "engine/ui/ui_context.h"

namespace engine {

// Global engine initialization and lifecycle hooks can be placed here.

}  // namespace engine

#endif  // ENGINE_ENGINE_H_
