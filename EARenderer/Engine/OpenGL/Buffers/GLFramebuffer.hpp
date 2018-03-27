//
//  GLFramebuffer.hpp
//  EARenderer
//
//  Created by Pavlo Muratov on 29.04.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef GLFramebuffer_hpp
#define GLFramebuffer_hpp

#include "GLNamedObject.hpp"
#include "GLBindable.hpp"
#include "GLViewport.hpp"

#include "GLTexture2D.hpp"
#include "GLHDRTexture2D.hpp"
#include "GLTextureCubemap.hpp"
#include "GLDepthTexture2D.hpp"
#include "GLDepthTextureCubemap.hpp"
#include "GLHDRTextureCubemap.hpp"
#include "GLDepthTexture2DArray.hpp"
#include "GLTexture2DArray.hpp"
#include "GLDepthRenderbuffer.hpp"
#include "GLHDRTexture3D.hpp"

#include "Size2D.hpp"
#include "Range.hpp"

#include <functional>
#include <unordered_set>
#include <vector>

namespace EARenderer {
    
    class GLFramebuffer: public GLNamedObject, public GLBindable {
    private:
        Size2D mSize;
        GLViewport mViewport;
        GLint mMaximumColorAttachments;
        GLint mMaximumDrawBuffers;
        std::unordered_set<GLenum> mDrawBuffers;

        void obtainHardwareLimits();
        void attachTextureToDepthAttachment(const GLTexture& texture, uint16_t mipLevel = 0, int16_t layer = -1);
        void attachTextureToColorAttachment0(const GLTexture& texture, uint16_t mipLevel = 0, int16_t layer = -1);
        
    public:
        enum class ColorAttachment { Attachment0, Attachment1, Attachment2, Attachment3,
            Attachment4, Attachment5, Attachment6, Attachment7 };

        GLFramebuffer(const Size2D& size);
        GLFramebuffer(GLFramebuffer&& that) = default;
        GLFramebuffer& operator=(GLFramebuffer&& rhs) = default;
        ~GLFramebuffer() override;
        
        void bind() const override;
        
        const Size2D& size() const;
        bool isComplete() const;
        const GLViewport& viewport() const;

        void attachTexture(const GLDepthTexture2D& texture, uint16_t mipLevel = 0);

        void attachTexture(const GLTexture2D& texture,
                           ColorAttachment colorAttachment = ColorAttachment::Attachment0,
                           uint16_t mipLevel = 0);

        void attachTexture(const GLTextureCubemap& texture,
                           ColorAttachment colorAttachment = ColorAttachment::Attachment0,
                           uint16_t mipLevel = 0);

        void attachTexture(const GLHDRTexture2D& texture,
                           ColorAttachment colorAttachment = ColorAttachment::Attachment0,
                           uint16_t mipLevel = 0);

        void attachTexture(const GLDepthTextureCubemap& texture,
                           uint16_t mipLevel = 0);

        void attachTexture(const GLHDRTextureCubemap& texture,
                           ColorAttachment colorAttachment = ColorAttachment::Attachment0,
                           uint16_t mipLevel = 0);

        void attachTextureLayer(const GLDepthTexture2DArray& textures,
                                uint16_t layer,
                                uint16_t mipLevel = 0);

        void attachTextureLayer(const GLTexture2DArray& textures,
                                uint16_t layer,
                                ColorAttachment colorAttachment = ColorAttachment::Attachment0,
                                uint16_t mipLevel = 0);

        void attachTextureLayer(const GLTexture2DArray& textures,
                                uint16_t layer,
                                ColorAttachment colorAttachment = ColorAttachment::Attachment0,
                                uint16_t mipLevel = 0);

        void attachTextureLayer(const GLHDRTexture3D& texture,
                                uint16_t layer,
                                ColorAttachment colorAttachment = ColorAttachment::Attachment0,
                                uint16_t mipLevel = 0);

        void attachRenderbuffer(const GLDepthRenderbuffer& renderbuffer);
        
        void detachAllAttachments();
    };
    
}

#endif /* GLFramebuffer_hpp */
