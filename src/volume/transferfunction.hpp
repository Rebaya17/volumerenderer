#ifndef __TRANSFER_FUNCTION_HPP_
#define __TRANSFER_FUNCTION_HPP_

#include "../scene/glslprogram.hpp"

#include "../glad/glad.h"

#include <set>


/** The transfer function class */
class TransferFunction {
    private:
        // Attributes

        /** Texture ID */
        GLuint texture;

        /** Texture data */
        GLubyte data[1024];


        /** Nodes */
        std::set<GLubyte> node;

        /** Current node */
        GLubyte current_node;


        // Constructors

        /** Disable the default copy constructor */
        TransferFunction(const TransferFunction &) = delete;

        /** Disable the assignation operator */
        TransferFunction &operator=(const TransferFunction &) = delete;


        // Methods

        /** Update the function */
        void update();



    public:
        // Constructors

        /** Transfer function constructor */
        TransferFunction();


        // Getters

        /** Get the node color */
        glm::uvec4 getNode(const GLubyte &index) const;

        /** Get the current node color */
        glm::uvec4 getCurrentNode() const;

        /** Get the current node index */
        GLubyte getCurrentNodeIndex() const;


        // Setters

        /** Set the node color */
        void setNode(const GLubyte &index, const glm::uvec4 &color);

        /** Set the current node color */
        void setCurrentNode(const glm::uvec4 &color);

        /** Set the current node index */
        void setCurrentNodeIndex(const GLubyte &index);


        // Methods

        /** Bind the transfer function */
        void bind(GLSLProgram *const program, const GLint &index = 0);

        /** Reset */
        void reset();


        /** Select the previous node as current */
        GLubyte selectPreviousNode();

        /** Select the next node as current */
        GLubyte selectNextNode();


        /** Remove node */
        void removeNode(const GLubyte &index);

        /** Remove the current node */
        void removeCurrentNode();


        // Destructor

        /** Transfer function destructor */
        ~TransferFunction();
};

#endif // __TRANSFER_FUNCTION_HPP_