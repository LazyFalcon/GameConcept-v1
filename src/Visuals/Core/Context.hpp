#pragma once
#include "gl_core_4_5.hpp"
#include "FBO.hpp"
#include "GPUResources.hpp"
#include "Texture.hpp"
#include "Window.hpp"

class Context
{
private:
    Window &window;
    void resetTextures();
    void resetFbo();
    void resetBuffers();
    void resetShapes();
public:
    struct {
        Texture view;
        Texture depth;
        struct {
            Texture a;
            Texture b;
        } full;
        struct {
            Texture a;
            Texture b;
        } half;
        struct {
            Texture a;
            Texture b;
        } quarter;
        Texture ui;
    } tex;

    struct {
        VAO vao;
        VBO point {};
        VBO quadCentered {}; // xyyv
        VBO quadCorner {}; // xyuv
        VBO screen {}; // xyuv
        VBO cube {};
    } shape;

    std::array<VBO,6> randomBuffers;
    u32 randomBufferId {0};
    VBO& getRandomBuffer(){
        randomBufferId = (randomBufferId+1)%6;
        return randomBuffers[randomBufferId];
    }
    VBO& getBuffer(){
        randomBufferId = (randomBufferId+1)%6;
        return randomBuffers[randomBufferId];
    }
    VAO defaultVAO;
    u32 currentFbo;
    FBO fbo;

    Context(Window &window) : window(window), fbo(window){}
    void reset();

    void beginFrame();
    void endFrame();

    void setupFramebufferForShadowMapGeneration();
    void setupFramebufferForGBufferGeneration();
    void setupFramebufferForLighting();
    void setupFramebufferForLightingWithAddionalDepth();
    void setupFramebufferForLDRProcessing();

    void bindTexture(const Texture &texture);

    void setupDefaultBuffer();
    void setupMainFrameBuffer();
    void cullBackFaces();
    void cullFrontFaces();

    void unbindFBO();
    void drawScreen();

    bool _errors(const std::string &text, const std::string &file, int line, const std::string &fun);
};
#define errors() _errors("", __FILE__, __LINE__, __FUNCTION__)
