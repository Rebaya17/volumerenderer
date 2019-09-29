#include "transferfunction.hpp"


// Private methods

// Update the function
void TransferFunction::update() {
    // Get the first node
    unsigned int a = static_cast<unsigned int>(*node.begin()) << 2U;
    glm::vec4 color_a(data[a], data[a + 1U], data[a + 2U], data[a + 3U]);

    // Fill with the node to the left
    for (unsigned int i = 0U; i < a; i += 4U) {
        data[i     ] = color_a.r;
        data[i + 1U] = color_a.g;
        data[i + 2U] = color_a.b;
        data[i + 3U] = color_a.a;
    }

    // Set the default values
    for (std::set<GLubyte>::const_iterator i = ++node.begin(); i != node.end(); i++) {
        // Get the right color
        const unsigned int b = static_cast<unsigned int>(*i) << 2U;
        const glm::vec4 color_b(data[b], data[b + 1U], data[b + 2U], data[b + 3U]);

        // Interpolate colors
        const glm::vec4 m = glm::vec4(color_b - color_a) / static_cast<float>((b - a) >> 2U);
        for (unsigned int j = a; j < b; j += 4U) {
            const glm::vec4 color = color_a + static_cast<float>((j - a) >> 2U) * m;
            data[j     ] = static_cast<GLubyte>(color.r);
            data[j + 1U] = static_cast<GLubyte>(color.g);
            data[j + 2U] = static_cast<GLubyte>(color.b);
            data[j + 3U] = static_cast<GLubyte>(color.a);
        }

        // Update the left color
        a = b;
        color_a = color_b;
    }

    // Fill with the node to the left
    for (unsigned int i = a; i < 1020U; i += 4U) {
        data[i     ] = color_a.r;
        data[i + 1U] = color_a.g;
        data[i + 2U] = color_a.b;
        data[i + 3U] = color_a.a;
    }


    // Bind texture
    glBindTexture(GL_TEXTURE_1D, texture);

    // Update the data
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Unbind texture
    glBindTexture(GL_TEXTURE_1D, GL_FALSE);
}


// Constructors

/** Transfer function constructor */
TransferFunction::TransferFunction() :
    // Texture attributes
    texture(GL_FALSE) {
    // Generate the texture
    glGenTextures(1, &texture);

    // Bind texture
    glBindTexture(GL_TEXTURE_1D, texture);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Unbind texture
    glBindTexture(GL_TEXTURE_1D, GL_FALSE);

    // Load the default values
    reset();
}


// Getters

// Get the node color
glm::uvec4 TransferFunction::getNode(const GLubyte &index) const{
    const unsigned int i = static_cast<unsigned int>(index) << 2;
    return glm::uvec4(data[i], data[i + 1], data[i + 2], data[i + 3]);
}

// Get the current node color
glm::uvec4 TransferFunction::getCurrentNode() const{
    const unsigned int i = static_cast<unsigned int>(current_node) << 2;
    return glm::uvec4(data[i], data[i + 1], data[i + 2], data[i + 3]);
}

// Get the current node index
GLubyte TransferFunction::getCurrentNodeIndex() const{
    return current_node;
}


// Setters

// Set the node color
void TransferFunction::setNode(const GLubyte &index, const glm::uvec4 &color){
    // Set the color
    const unsigned int i = static_cast<unsigned int>(index) << 2;
    data[i]     = glm::clamp(color.r, 0U, 255U);
    data[i + 1] = glm::clamp(color.g, 0U, 255U);
    data[i + 2] = glm::clamp(color.b, 0U, 255U);
    data[i + 3] = glm::clamp(color.a, 0U, 255U);

    // Add node
    node.insert(index);

    // Update the function
    update();
}

// Set the current node color
void TransferFunction::setCurrentNode(const glm::uvec4 &color){
    // Set the color
    const unsigned int i = static_cast<unsigned int>(current_node) << 2;
    data[i]     = glm::clamp(color.r, 0U, 255U);
    data[i + 1] = glm::clamp(color.g, 0U, 255U);
    data[i + 2] = glm::clamp(color.b, 0U, 255U);
    data[i + 3] = glm::clamp(color.a, 0U, 255U);

    // Update the function
    update();
}

// Set the current node index
void TransferFunction::setCurrentNodeIndex(const GLubyte &index){
    current_node = index;
}


// Methods

// Bind the transfer function
void TransferFunction::bind(GLSLProgram *const program, const GLint &index) {
    // Check the program status
    if ((program == nullptr) || (!program->isValid())) {
        return;
    }

    // Use the program
    program->use();

    // Set uniform
    program->setUniform("u_trans_func", index);

    // Bind texture
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_1D, texture);
}

// Reset
void TransferFunction::reset() {
    // Default colors
    data[0] = 0U;
    data[1] = 0U;
    data[2] = 0U;
    data[3] = 0U;
    data[1020] = 255U;
    data[1021] = 255U;
    data[1022] = 255U;
    data[1023] = 255U;

    // Default nodes
    node.clear();
    node.insert(0U);
    node.insert(255U);

    // Select the first node
    current_node = 0U;

    // Update the texture
    update();
}


// Select the previous node as current
GLubyte TransferFunction::selectPreviousNode() {
    // Get the current node
    std::set<GLubyte>::const_iterator prev = node.find(current_node);

    // Select the nearest node
    if (prev == node.end()) {
        for (std::set<GLubyte>::const_iterator i = node.begin(); *i < current_node; i++);
    }
    
    // Select the previous if is not the first
    if (prev != node.begin()) {
        prev--;
    }

    // Update the current node and return
    return current_node = *prev;
}

//Select the next node as current
GLubyte TransferFunction::selectNextNode() {
    // Get the next node
    std::set<GLubyte>::const_iterator next = ++node.find(current_node);
    
    // Update the current node if is not the last
    if (next != node.end()) {
        current_node = *next;
    }

    // Return the new current node
    return current_node;
}


// Remove node
void TransferFunction::removeNode(const GLubyte &index) {
    // Check the minimum size
    if (node.size() == 2U) return;

    // Check if is the current node
    if (index == current_node) {
        removeCurrentNode();
        return;
    }

    // Remove node
    node.erase(index);
}

// Remove the current node
void TransferFunction::removeCurrentNode() {
    // Check the minimum size
    if (node.size() == 2U) return;

    // Get the new current node
    std::set<GLubyte>::const_iterator index = node.find(current_node);
    if (index == node.end()) {
        selectPreviousNode();
        return;
    }

    // Remove node and update the current
    const GLubyte new_current = *(std::next(index) == node.end() ? --index : ++index);
    node.erase(current_node);
    current_node = new_current;
}


// Destructor

// Transfer function destructor
TransferFunction::~TransferFunction() {
    glDeleteTextures(1, &texture);
}