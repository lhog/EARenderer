//
// Created by Pavlo Muratov on 2018-12-12.
// Copyright (c) 2018 MPO. All rights reserved.
//

#include "GLSLOmnidirectionalPenumbra.hpp"

namespace EARenderer {

    GLSLOmnidirectionalPenumbra::GLSLOmnidirectionalPenumbra()
    :
    GLProgram("FullScreenQuad.vert", "OmnidirectionalPenumbra.frag", "")
    { }

    void GLSLOmnidirectionalPenumbra::setCamera(const Camera& camera) {
        glUniformMatrix4fv(uniformByNameCRC32(uint32_constant<ctcrc32("uCameraViewInverse")>).location(), 1, GL_FALSE,
                glm::value_ptr(camera.inverseViewMatrix()));
        glUniformMatrix4fv(uniformByNameCRC32(uint32_constant<ctcrc32("uCameraProjectionInverse")>).location(), 1, GL_FALSE,
                glm::value_ptr(camera.inverseProjectionMatrix()));
    }

    void GLSLOmnidirectionalPenumbra::setGBuffer(const SceneGBuffer& GBuffer) {
        setUniformTexture(uint32_constant<ctcrc32("uGBufferHiZBuffer")>, *GBuffer.HiZBuffer);
    }

    void GLSLOmnidirectionalPenumbra::setShadowMap(const GLDepthTextureCubemap &shadowMap, const GLSampler& bilinearSampler) {
        setUniformTexture(uint32_constant<ctcrc32("uOmnidirectionalShadowMapBilinearSampler")>, shadowMap, &bilinearSampler);
    }

    void GLSLOmnidirectionalPenumbra::setLight(const PointLight &light) {
        glUniform3fv(uniformByNameCRC32(uint32_constant<ctcrc32("uPointLight.position")>).location(), 1, glm::value_ptr(light.position()));
        glUniform3fv(uniformByNameCRC32(uint32_constant<ctcrc32("uPointLight.radiantFlux")>).location(), 1, reinterpret_cast<const GLfloat *>(&light.color()));
        glUniform1f(uniformByNameCRC32(uint32_constant<ctcrc32("uPointLight.nearPlane")>).location(), light.nearClipPlane());
        glUniform1f(uniformByNameCRC32(uint32_constant<ctcrc32("uPointLight.farPlane")>).location(), light.farClipPlane());
    }

}