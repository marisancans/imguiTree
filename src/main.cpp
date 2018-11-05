#include <stdio.h>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "gridWindow.h"
#include "treeWindow.h"

#include "game.h"
#include <chrono>
#include <thread>

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#define GLEW_STATIC
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    
    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    
    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() != 0;
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }
    
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    
    bool showGridWindow = true;
    bool showTreeWindow = true;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    
    
    // GAME SETTINGS <--------------------------------------------------
    
    PlayerIdx firstTurn = P1;
    
    
    GameSettings gameSettings;
    gameSettings.maxLayer = 3;
    gameSettings.maxBoardX = 20;
    gameSettings.maxBoardY = 20;
    gameSettings.scrolling = ImVec2(0.0f, 0.0f);
    gameSettings.showGrid = false;
    gameSettings.levelOffsetX = 100;
    gameSettings.levelOffsetY = 100;
    gameSettings.speedMS = 500;
    for(int p = 0; p < PLAYER_COUNT; p++)
        for(int i = 0; i < 8; i++)
            gameSettings.movRange[p][i][0] = true;
    for(int i = 0; i < 8; i += 2)
        gameSettings.movRange[P2][i][1] = true;
    
    gameSettings.startPos[P1] = {0, 0};
    gameSettings.startPos[P2] = {gameSettings.maxBoardX - 1, gameSettings.maxBoardY - 1};
    
    // Init
    game::init(game::PCvsPC, firstTurn, gameSettings);
    
    
    
    float angle = 0;
    
    bool* lala;
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
    
        glfwPollEvents();
    
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
    
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    
        glMatrixMode(GL_MODELVIEW_MATRIX);
        glLoadIdentity();
        angle += 1.f;
        glRotatef(angle, 0, 1, 0);
        
        glMatrixMode(GL_PROJECTION_MATRIX);
        
        glOrtho(-display_w/2,display_w/2, -display_h/2,display_h/2, -1000, 1000);
        
        glBegin(GL_TRIANGLES);
        glColor3f(1, 1, 1);
    
        glVertex2f(50, 50);
        glVertex2f(0, 50);
        glVertex2f(50, 0);
        glEnd();
        
        gridWindow(&showGridWindow, gameSettings);
        treeWindow(&showTreeWindow, gameSettings);
//        ImGui::ShowDemoWindow(lala);
        
        // Rendering
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
