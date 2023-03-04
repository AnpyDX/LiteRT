#include "Buffer.h"
#include <stdexcept>
using namespace LRT;


Buffer::Buffer(void* data, std::size_t size, GLenum sampleType)
{
    glGenBuffers(1, &m_buffer_id);
    glBindBuffer(GL_TEXTURE_BUFFER, m_buffer_id);
    glBufferData(GL_TEXTURE_BUFFER, size, data, GL_STATIC_DRAW);

    glGenTextures(1, &m_tex_id);
    glBindTexture(GL_TEXTURE_BUFFER, m_tex_id);
    glTexBuffer(GL_TEXTURE_BUFFER, sampleType, m_buffer_id);
}

Buffer::~Buffer()
{
    destroy();
}

unsigned int Buffer::getID() const
{
    if (!m_has_destroyed) {
        return m_tex_id;
    }
    throw std::runtime_error("Try to get a destroyed buffer's id!");
}

void Buffer::destroy()
{
    if (!m_has_destroyed) {
        glDeleteBuffers(1, &m_buffer_id);
        glDeleteTextures(1, &m_tex_id);
        m_has_destroyed = true;
    }
}