#include "core.hpp"
#include "Context.hpp"
#include "GraphicEngine.hpp"
#include "ui-renderer.hpp"
#include "Window.hpp"

GraphicEngine::GraphicEngine(Window &window) :
    window(window),
    context(std::make_unique<Context>(window)),
    uiRender(std::make_unique<UIRender>(window, *context))
    {
        context->reset();
    }

GraphicEngine::~GraphicEngine(){
    log("~GraphicEngine");
};
void GraphicEngine::beginFrame(){context->beginFrame();}
void GraphicEngine::endFrame(){context->endFrame();}
void GraphicEngine::takeScreenShot(){
    // utils->takeScreenShot();
}
void GraphicEngine::blurAndStoreRenderedScene(){
    // utils->blurBuffer();
}
