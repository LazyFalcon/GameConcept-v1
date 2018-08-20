#pragma once

class Context;
class Effects;
class GBufferSamplers;
class LightRendering;
class ObjectBatchedRender;
class RendererUtils;
class SceneRenderer;
class ShadowCaster;
class UIRender;
class VfxEffects;
class Window;

class GraphicEngine
{
private:
    Window &window;
public:
    GraphicEngine(Window &window);
    ~GraphicEngine();
    void reset();
    void beginFrame();
    void endFrame();
    void takeScreenShot();
    void blurAndStoreRenderedScene();

    std::unique_ptr<Context> context;
    std::unique_ptr<UIRender> uiRender;
};
