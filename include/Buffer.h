/**
* Data Buffer Object
* (Wrapper of GL_TEXTURE_BUFFER object)
*/

#pragma once
#include <vector>
#include <glad/glad.h>

namespace LRT
{
    class Buffer
    {
    public:
        Buffer() = delete;
        Buffer(void* data, std::size_t size, GLenum sampleType);
        ~Buffer();
        
        /* Get GL_TEXTURE ID */
        unsigned int getID() const;
        
        /* Destroy Buffer */
        void destroy();

    private:
        unsigned int m_buffer_id;
        unsigned int m_tex_id;
        bool m_has_destroyed = false;
    };
}