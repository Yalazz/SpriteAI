#pragma once
#include <memory>
#include "theme/CanvasTheme.h"

class ThemeManager
{
public:
    static ThemeManager& instance()
    {
        static ThemeManager inst;
        return inst;
    }

    void setTheme(std::unique_ptr<CanvasTheme> t)
    {
        theme = std::move(t);
    }

    const CanvasTheme& current() const
    {
        return *theme;
    }

private:
    std::unique_ptr<CanvasTheme> theme;
};
