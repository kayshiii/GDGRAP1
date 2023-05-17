// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


#include <GLFW/glfw3.h>

int main(void)
{
    //start of initilization
    //creates window
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Kassandra So", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //end of initialization

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES); //top
        glVertex2f(0.f, 0.5f);
        glVertex2f(-0.5f, 0.f);
        glVertex2f(0.5f, 0.f);
        glEnd();

        glBegin(GL_TRIANGLES); //bot left
        glVertex2f(-0.3f, -0.7f);
        glVertex2f(0.3f, -0.7f);
        glVertex2f(-0.5f, -0.f);
        glEnd();

        glBegin(GL_TRIANGLES); //bot left
        glVertex2f(0.3f, -0.7f);
        glVertex2f(-0.5f, 0.f);
        glVertex2f(0.5f, 0.f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}