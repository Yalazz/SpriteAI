#pragma once
#include <string>

namespace spriteai::core::document { class SpriteDocument; }
namespace spriteai::core::command { class CommandStack; }

namespace spriteai::core::tools {

struct ToolInput {
    float x = 0.0f;
    float y = 0.0f;
    float pressure = 1.0f;
    bool primaryDown = false;
    bool alt = false;
    bool shift = false;
};

class Tool {
public:
    virtual ~Tool() = default;

    virtual std::string id() const = 0;
    virtual std::string displayName() const = 0;

    virtual void onPointerDown(spriteai::core::document::SpriteDocument&,
                               spriteai::core::command::CommandStack&,
                               const ToolInput&) = 0;

    virtual void onPointerMove(spriteai::core::document::SpriteDocument&,
                               spriteai::core::command::CommandStack&,
                               const ToolInput&) = 0;

    virtual void onPointerUp(spriteai::core::document::SpriteDocument&,
                             spriteai::core::command::CommandStack&,
                             const ToolInput&) = 0;
};

} // namespace spriteai::core::tools
