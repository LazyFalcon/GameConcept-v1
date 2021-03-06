#pragma once
#include "Assets.hpp"

class Yaml;
struct LightSourceContainer;

class ResourceLoader
{
public:
    ResourceLoader() : id(s_id++){}
    ~ResourceLoader();

    std::vector<float> model_vertices;
    std::vector<float> model_coords;
    std::vector<float> model_normals;
    std::vector<u32> model_indices;

    static u32 s_id;
     u32 id;

    bool loadTangents { false };

    void loadResources(const Yaml &cfg);
    bool loadShaders();
    bool reloadShader(const std::string &filename);

    Image loadImage(const std::string &name);
    bool loadImage(const Yaml &cfg);
    void loadImages(const std::string& dir);
    bool loadFonts();
    bool loadFont(const std::string &font, std::vector<std::string> &imagesToLoad);
    bool loadImageSet(const Yaml &cfg);
    std::string loadFile(std::string fname);
    void printShaderInfoLog(i32 shader);
    void fillBuffers();

    assets::TextureArray loadTextureArray(const std::string &folder, const std::string &containerName);
    assets::TextureArray loadCubeMap(const std::string &folder);
};
