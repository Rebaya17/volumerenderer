#include <iostream>

/** Main function */
int main (int argc, char **argv) {
    // Print arguments
    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }

    return 0;
}