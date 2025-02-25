#include "geometrybuffer.h"
#include <cstddef>
#include <iostream>

namespace engine {

	GBuffer::GBuffer() {
		fbo = 0;
		depthTexture = 0;
	}

	GBuffer::~GBuffer() {
		deleteBufferData();
	}

	void GBuffer::deleteBufferData() {
		if (texture[0] != 0)
			glDeleteTextures(GBuffer::GB_NUMBER_OF_TEXTURES, texture);
		if (depthTexture != 0)
			glDeleteTextures(1, &depthTexture);
		if (fbo != 0)
			glDeleteFramebuffers(1, &fbo);
	}

	void GBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		
		// Geometry FBO
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glGenTextures(GBuffer::GB_NUMBER_OF_TEXTURES, texture);
		glGenTextures(1, &depthTexture);
		
		for (unsigned int i = 0; i < GBuffer::GB_NUMBER_OF_TEXTURES; i++) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			if (i == GBuffer::GB_METALLIC_ROUGHNESS_AO || i == GBuffer::GB_ALBEDO) 
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			else 
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture[i], 0);
		}

		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, DrawBuffers);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::setBufferToRead(GB_TEX_TYPE texType) {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + texType);
	}
}