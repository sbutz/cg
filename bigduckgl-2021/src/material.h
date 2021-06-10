#pragma once

#include <string>
#include <memory>
#include <map>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <assimp/material.h>
#include "named_map.h"
#include "shader.h"
#include "texture.h"

#define PLAIN_COLOR_SUPPORT true

class Material : public NamedMap<Material> {
public:
    Material(const std::string& name);
    Material(const std::string& name, const std::string& base_path, const aiMaterial* mat_ai);
    virtual ~Material();

    void bind(const std::shared_ptr<Shader>& shader) const;
    void unbind() const;

    void add_texture(const std::string& uniform_name, const std::shared_ptr<Texture2D>& texture);
    bool has_texture(const std::string& uniform_name) const;
    std::shared_ptr<Texture2D> get_texture(const std::string& uniform_name) const;

    // data
    std::map<std::string, std::shared_ptr<Texture2D>> texture_map;
#if PLAIN_COLOR_SUPPORT == true
	glm::vec4 k_amb, k_diff, k_spec;
	float n_spec;
#endif
};

typedef std::shared_ptr<Material> material_ptr;

// variadic alias for std::make_shared<>(...)
template <class... Args> std::shared_ptr<Material> make_material(Args&&... args) {
    return std::make_shared<Material>(args...);
}
