#include "postprocess.h"

#include <cstddef>
#include <random>
#include <cmath>
#include <math.h>

namespace engine {

	ShadedBuffer::ShadedBuffer() {};
	ShadedBuffer::~ShadedBuffer() { ShadedBuffer::deleteBufferData(); };

	void ShadedBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &texture);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
		GLenum DrawBuffersShade[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffersShade);
	}

	void ShadedBuffer::deleteBufferData() {
		if (fbo != 0)
			glDeleteFramebuffers(1, &fbo);
		if (texture != 0)
			glDeleteTextures(1, &texture);
		if (depthTexture != 0)
			glDeleteTextures(1, &depthTexture);
	}

	BloomBuffer::BloomBuffer() {};
	BloomBuffer::~BloomBuffer() { BloomBuffer::deleteBufferData(); };

	void BloomBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &texture);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		GLenum DrawBuffersBloom[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffersBloom);
	}

	void BloomBuffer::deleteBufferData() {
		if (fbo != 0)
			glDeleteFramebuffers(1, &fbo);
		if (texture != 0)
			glDeleteTextures(1, &texture);
	}

	ReflectionsBuffer::ReflectionsBuffer() {};
	ReflectionsBuffer::~ReflectionsBuffer() { ReflectionsBuffer::deleteBufferData(); };

	void ReflectionsBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &texture);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		GLenum DrawBuffersReflection[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffersReflection);
	}

	void ReflectionsBuffer::deleteBufferData() {
		if (fbo != 0)
			glDeleteFramebuffers(1, &fbo);
		if (texture != 0)
			glDeleteTextures(1, &texture);
	}

	ReflectionsBlendBuffer::ReflectionsBlendBuffer() {};
	ReflectionsBlendBuffer::~ReflectionsBlendBuffer() { ReflectionsBlendBuffer::deleteBufferData(); };

	void ReflectionsBlendBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &texture);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		GLenum DrawBuffersReflection[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffersReflection);
	}

	void ReflectionsBlendBuffer::deleteBufferData() {
		if (fbo != 0)
			glDeleteFramebuffers(1, &fbo);
		if (texture != 0)
			glDeleteTextures(1, &texture);
	}

	BlurBuffer::BlurBuffer() {};
	BlurBuffer::~BlurBuffer() { BlurBuffer::deleteBufferData(); };

	void BlurBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &texture);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		GLenum DrawBuffersBlur[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffersBlur);
	}

	void BlurBuffer::deleteBufferData() {
		if (fbo != 0)
			glDeleteFramebuffers(1, &fbo);
		if (texture != 0)
			glDeleteTextures(1, &texture);
	}

	PingPongBuffer::PingPongBuffer() {};
	PingPongBuffer::~PingPongBuffer() { PingPongBuffer::deleteBufferData(); };

	void PingPongBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		glGenFramebuffers(2, fbo);
		glGenTextures(2, texture);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[i], 0);
		}
	}

	void PingPongBuffer::deleteBufferData() {
		if (fbo != 0)
			glDeleteFramebuffers(2, fbo);
		if (texture[0] != 0)
			glDeleteTextures(2, texture);
	}

	SsaoBuffer::SsaoBuffer() {};
	SsaoBuffer::~SsaoBuffer() { 
		SsaoBuffer::deleteBufferData();
		if (noiseTexture != 0)
			glDeleteTextures(1, &noiseTexture);
	};

	void SsaoBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &texture);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		GLenum DrawBuffersSsao[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffersSsao);
	}

	void SsaoBuffer::deleteBufferData() {
		if (fbo != 0)
			glDeleteFramebuffers(1, &fbo);
		if (texture != 0)
			glDeleteTextures(1, &texture);
	}

	void SsaoBuffer::generateSampleKernel() {
		std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		for (unsigned int i = 0; i < 64; ++i)
		{
			Vector3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
			sample = (sample).normalized();
			sample *= randomFloats(generator);
			float scale = i / 64.0f;
			scale = 0.1f + 1.0f * (scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}
	}

	void SsaoBuffer::generateNoiseTexture() {
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		std::vector<Vector3> ssaoNoise;
		for (unsigned int i = 0; i < 16; i++)
		{
			Vector3 noise(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, 0.0f); // rotate around z-axis (in tangent space)
			ssaoNoise.push_back(noise);
		}
		glGenTextures(1, &noiseTexture);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
}