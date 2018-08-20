#include "core.hpp"
#include "Context.hpp"
#include "Logging.hpp"
#include "Assets.hpp"
#include "PerfTimers.hpp"
#include "BaseStructs.hpp"
#include "ResourceLoader.hpp"

void Context::reset(){
    resetTextures();
    resetFbo();
    resetBuffers();
    resetShapes();

    GpuTimerScoped::init();

    { // catch error
        int err = gl::GetError();
        if(err != gl::NO_ERROR_){
            error("FBO fail:", err);
        }
        else
            log("[ OK ] initialisation of FBOs");
    }
}

void Context::resetTextures(){
    log("--textures");
    tex.view = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
    tex.gbuffer.depth = Texture(gl::TEXTURE_2D, gl::DEPTH_COMPONENT32F, window.size.x, window.size.y, 1, gl::DEPTH_COMPONENT, gl::FLOAT, gl::LINEAR, 0);
    tex.full.a = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
    tex.full.b = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
    tex.half.a = Texture(gl::TEXTURE_2D, gl::RGBA16F, 0.5*window.size.x, 0.5*window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
    tex.half.b = Texture(gl::TEXTURE_2D, gl::RGBA16F, 0.5*window.size.x, 0.5*window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
    tex.quarter.a = Texture(gl::TEXTURE_2D, gl::RGBA16F, 0.25*window.size.x, 0.25*window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
    tex.quarter.b = Texture(gl::TEXTURE_2D, gl::RGBA16F, 0.25*window.size.x, 0.25*window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
    tex.ui = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
}
void Context::resetFbo(){
    log("--fbos");
    fbo.drawBuffers[0] = gl::COLOR_ATTACHMENT0;
    fbo.drawBuffers[1] = gl::COLOR_ATTACHMENT1;
    fbo.drawBuffers[2] = gl::COLOR_ATTACHMENT2;

    gl::GenFramebuffers(1, &fbo.full);
        gl::BindFramebuffer(gl::FRAMEBUFFER, fbo.full);
        gl::Viewport(0, 0, window.size.x, window.size.y);
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, tex.view.ID, 0);
        gl::DrawBuffers(1, fbo.drawBuffers);
        gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

    gl::GenFramebuffers(1, &fbo._12);
        gl::BindFramebuffer(gl::FRAMEBUFFER, fbo._12);
        gl::Viewport(0, 0, 0.5*window.size.x, 0.5*window.size.y);
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, tex.half.a.ID, 0);
        gl::DrawBuffers(1, fbo.drawBuffers);
        gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

    gl::GenFramebuffers(1, &fbo._14);
        gl::BindFramebuffer(gl::FRAMEBUFFER, fbo._14);
        gl::Viewport(0, 0, window.size.x/4.f, window.size.y/4.f);
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, tex.quarter.a.ID, 0);
        gl::DrawBuffers(1, fbo.drawBuffers);
        gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

}
void Context::resetBuffers(){
    log("--buffers");
    defaultVAO.setup()();
    for(auto &it : randomBuffers){
        it.setup(32000, true)();
    }
}

void Context::resetShapes(){ // TODO: zwalidować użycia tego
    log("--shapes");
    float point[] = {0.0f, 0.0f, 0.5f, 1.f};
    float quadCentered[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
          0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, 1.0f, 1.0f,
    };
    float quadCorner[] = {
        0.f, 0.f, 0.0f, 0.0f,
        0.f, 1.f, 0.0f, 1.f,
        1.f, 0.f, 1.0f, 0.f,
        1.f, 1.f, 1.0f, 1.f,
    };
    float screen[] = {
        -1.f, -1.f, 0.f, 0.f,
        -1.f,  1.f, 0.f, 1.f,
        1.f, -1.f, 1.f, 0.f,
         1.f,  1.f, 1.f, 1.f,
    };
    float cube[] = {
        // columns
        -0.5, -0.5, -0.5, 1,
        -0.5, -0.5, 0.5, 1,
        0.5, -0.5, -0.5, 1,
        0.5, -0.5, 0.5, 1,
        -0.5, 0.5, -0.5, 1,
        -0.5, 0.5, 0.5, 1,
        0.5, 0.5, -0.5, 1,
        0.5, 0.5, 0.5, 1,
        // bottom
        -0.5, -0.5, -0.5, 1,
        0.5, -0.5, -0.5, 1,
        -0.5, -0.5, -0.5, 1,
        -0.5, 0.5, -0.5, 1,
        0.5, 0.5, -0.5, 1,
        -0.5, 0.5, -0.5, 1,
        0.5, 0.5, -0.5, 1,
        0.5, -0.5, -0.5, 1,
        // top
        -0.5, -0.5, 0.5, 1,
        0.5, -0.5, 0.5, 1,
        -0.5, -0.5, 0.5, 1,
        -0.5, 0.5, 0.5, 1,
        0.5, 0.5, 0.5, 1,
        -0.5, 0.5, 0.5, 1,
        0.5, 0.5, 0.5, 1,
        0.5, -0.5, 0.5, 1,
    };
    shape.point.setup(point, 4, false)();
    shape.quadCentered.setup(quadCentered, 16, false)();
    shape.quadCorner.setup(quadCorner, 16, false)();
    shape.screen.setup(screen, 16, false)();
    shape.cube.setup(cube, 24*4, false)();

    std::vector<glm::vec4> vertices = {
        // point
        glm::vec4(0,0,0,1),
        // quad center, xyzw
        glm::vec4(-0.5, -0.5, 0, 1),
        glm::vec4(-0.5, 0.5, 0, 1),
        glm::vec4(0.5, -0.5, 0, 1),
        glm::vec4(0.5, 0.5, 0, 1),
        // quad corner, xyzw
        glm::vec4(0,0,0,1),
        glm::vec4(0,1,0,1),
        glm::vec4(1,0,0,1),
        glm::vec4(1,1,0,1),
        // quad center, xyuv
        glm::vec4(-0.5, -0.5, 0, 0),
        glm::vec4(-0.5, 0.5, 0, 1),
        glm::vec4(0.5, -0.5, 1, 0),
        glm::vec4(0.5, 0.5, 1, 1),
        // quad corner, xyuv
        glm::vec4(0,0,0,0),
        glm::vec4(0,1,0,1),
        glm::vec4(1,0,1,0),
        glm::vec4(1,1,1,1),
        // screen, xyuv
        glm::vec4(0,0,0,0),
        glm::vec4(0,1,0,1),
        glm::vec4(1,0,1,0),
        glm::vec4(1,1,1,1)
    };
    std::vector<glm::vec2> texcoords = {
        glm::vec2(0,0),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,0),
        glm::vec2(1,1),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,0),
        glm::vec2(1,1),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,0),
        glm::vec2(1,1),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,0),
        glm::vec2(1,1)
    };

    shape.vao.setup()
          .addBuffer(vertices)
          .addBuffer(texcoords)
          ();
}

bool Context::_errors(const std::string &text, const std::string &file, int line, const std::string &fun){
    int err = gl::GetError();
    if(err != gl::NO_ERROR_){
        error("GLError:", err, file+"#", line, ":", fun, "::", text);
        // hardPause();
        return true;
    }
    return false;
}

void Context::bindTexture(const Texture &t0){
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
}

void Context::setupDefaultBuffer(){
    gl::Viewport(0, 0, window.size.x, window.size.y);
    gl::ClearColor(0.2f, 0.2f, 0.2f, 1.f);
    gl::ClearDepth(1);
    gl::ClearStencil(0);
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
    gl::DisableVertexAttribArray(0);
    gl::DepthFunc(gl::LEQUAL);
    gl::DepthRange(0.0f, 1.0f);
    gl::Disable(gl::BLEND);

    gl::DepthMask(gl::TRUE_);
    gl::Enable(gl::CULL_FACE);
    gl::Enable(gl::DEPTH_TEST);
    gl::FrontFace(gl::CCW);
    gl::BindTexture(gl::TEXTURE_2D, 0);

    errors();
}
void Context::setupMainFrameBuffer(){
    gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo.super);
    gl::FramebufferTexture2D(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, tex.view.ID, 0);
    gl::DrawBuffers(1, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x, window.size.y);

    gl::DepthRange(0.0f, 1.0f);
    gl::DepthFunc(gl::LEQUAL);
    gl::Disable(gl::DEPTH_TEST);
    gl::DepthMask(gl::TRUE_);
    gl::FrontFace(gl::CCW);

    gl::ClearColor(130/256.f, 140/256.f, 151/256.f, 1.f);
    gl::ClearDepth(1);
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

    gl::Disable(gl::BLEND);
    gl::DepthMask(gl::TRUE_);
    // gl::Enable(gl::CULL_FACE);
    gl::Disable(gl::CULL_FACE);
    gl::DisableVertexAttribArray(0);
    gl::BindTexture(gl::TEXTURE_2D, 0);

    defaultVAO.bind();

    errors();
}
void Context::unbindFBO(){
    gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, 0);
    currentFbo = 0xffffff;
}

void Context::drawScreen(){
    defaultVAO.bind();
    shape.screen.bind().attrib(0).pointer_float(4).divisor(0);
    gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);
    gl::BindVertexArray(0);
}

void Context::beginFrame(){
    gl::Viewport(0, 0, window.size.x, window.size.y);
    gl::ClearColor(0.9, 0.88, 0.87, 1.f);
    gl::ClearDepth(1);
    gl::ClearStencil(0);
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
    gl::DisableVertexAttribArray(0);
    gl::DepthFunc(gl::LEQUAL);
    gl::DepthRange(0.0f, 1.0f);
    gl::Disable(gl::BLEND);

    gl::DepthMask(gl::TRUE_);
    gl::Enable(gl::CULL_FACE);
    gl::Enable(gl::DEPTH_TEST);
    gl::FrontFace(gl::CCW);
    gl::BindTexture(gl::TEXTURE_2D, 0);

    errors();
}
void Context::endFrame(){
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
    gl::DisableVertexAttribArray(0);
    gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, 0);
    gl::BindFramebuffer(gl::READ_FRAMEBUFFER, 0);

    gl::Disable(gl::DEPTH_TEST);
    gl::Disable(gl::BLEND);
    gl::Disable(gl::CULL_FACE);
    gl::DepthMask(gl::FALSE_);

    auto shader = assets::getShader("ApplyFBO");
    shader.bind();
    shader.texture("uTexture", tex.view, 0);
    drawScreen();

    gl::UseProgram(0);
    gl::BindVertexArray(0);
    gl::DisableVertexAttribArray(0);
}
void Context::cullBackFaces(){
    gl::CullFace(gl::BACK);
}
void Context::cullFrontFaces(){
    gl::CullFace(gl::FRONT);
}

// Image splashScreen {};
// void drawSplashScreen(const std::string &filename){
//     if(splashScreen.ID) gl::DeleteTextures(1, &splashScreen.ID);
//     else {
//         ResourceLoader loader;
//         loader.reloadShader("ApplyFBO");
//         splashScreen = loader.loadImage(filename);

//         glfwShowWindow(Global::main.window);
//         glfwSetWindowPos(Global::main.window, Global::main.size.x/2.f - splashScreen.width/2, Global::main.size.y/2.f - splashScreen.height/2);
//         glfwSetWindowSize(Global::main.window, splashScreen.width, splashScreen.height);
//     }
//     gl::Viewport(0, 0, splashScreen.width, splashScreen.height);
//     gl::ClearColor(0.f, 0.f, 0.f, 0.f);
//     gl::Clear(gl::COLOR_BUFFER_BIT);
//     gl::DepthMask(gl::FALSE_);
//     gl::Enable(gl::BLEND);
//     gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

//     gl::BindBuffer(gl::ARRAY_BUFFER, 0);
//     gl::DisableVertexAttribArray(0);
//     gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, 0);

//     auto shader = assets::getShader("ApplyFBO");
//     shader.bind();
//     float width = Global::main.size.x;
//     float height = Global::main.size.y;

//     shader.texture("uTexture", splashScreen.ID);

//     drawScreen();

//     glfwSwapBuffers(Global::main.window);

//     CHECK_FOR_ERRORS
// }
