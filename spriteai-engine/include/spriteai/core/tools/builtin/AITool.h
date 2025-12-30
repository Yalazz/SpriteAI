#pragma once
#include <memory>
#include <string>
#include "spriteai/core/tools/Tool.h"
#include "spriteai/core/document/Stroke.h"

namespace spriteai::engine::ai { class IAIClient; }

namespace spriteai::core::tools::builtin {

class AITool final : public Tool {
public:
    explicit AITool(std::string mode = "clean");

    std::string id() const override { return "ai"; }
    std::string displayName() const override { return "AI Tool"; }

    void setAIClient(std::shared_ptr<spriteai::engine::ai::IAIClient> client) { m_client = std::move(client); }
    void setMode(std::string mode) { m_mode = std::move(mode); }

    void onPointerDown(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerMove(spriteai::core::document::SpriteDocument&,
                       spriteai::core::command::CommandStack&,
                       const ToolInput&) override;

    void onPointerUp(spriteai::core::document::SpriteDocument&,
                     spriteai::core::command::CommandStack&,
                     const ToolInput&) override;

private:
    std::shared_ptr<spriteai::engine::ai::IAIClient> m_client;
    std::string m_mode;

    bool m_drawing = false;
    spriteai::core::document::Stroke m_input{};
};

} // namespace spriteai::core::tools::builtin
