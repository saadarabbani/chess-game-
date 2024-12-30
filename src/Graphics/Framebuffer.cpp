#include "Framebuffer.h"

#include <iostream>
#include <glad/glad.h>

Framebuffer::Framebuffer(const FramebufferSpecification& spec) : m_Specification(spec) {
	Invalidate();
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(1, &m_ColourAttachment);
}

void Framebuffer::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void Framebuffer::Unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Invalidate() {
	// If framebuffer exists, delete it and create a new one
	if (m_RendererID) {
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColourAttachment);
	}

	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColourAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColourAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourAttachment, 0);

	// For depth buffer
#if 0
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
#endif

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is not complete!\n";

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(uint32_t width, uint32_t height) {
	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalidate();
}

