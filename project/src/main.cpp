#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>

// Functions Signature
void Init();
void LoadShaders();
void LoadObjects();
void RenderLoop();
void Quit();
void Update(SDL_Window * m_window);
void Draw();

// Variables

unsigned int VBO;
unsigned int VAO;
unsigned int shaderProgram;

static bool m_isClosed = false;

int width = 500;
int height = 500;

SDL_Window *m_window;
SDL_GLContext m_glContext;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

// Shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;void main(){gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;void main(){FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";

int main(int argc, char *argv[]){
	
	Init();
	LoadShaders();
	LoadObjects();
	RenderLoop();
	Quit();
	
	return 0;
}

void Init(){
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE			 , 8);	// Number of bits used to display shades of red
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE		 , 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE		 , 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE		 , 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE 		 , 32);	// Sum of bits above. Space of a pixel
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER		 , 1);	// Enable double buffer
	
	SDL_Init (SDL_INIT_EVERYTHING);
	
	m_window = SDL_CreateWindow ( 	"Titulo",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									width,
									height,
									SDL_WINDOW_OPENGL
								);
											
	if(m_window == NULL){
		std::cout << SDL_GetError();
	}
								
	m_glContext = SDL_GL_CreateContext(m_window);	
	
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		//return -1;
	}
}

void LoadShaders(){
	
	// Vertex Shader creation and compiling 
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check if its compilation was sucessfull
	int  vertexCompilationSuccess;
	char vertexInfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompilationSuccess);
	if(!vertexCompilationSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexInfoLog << std::endl;
	}
	
	// Vertex Shader creation and compiling 
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check if its compilation was sucessfull
	int  fragmentCompilationSuccess;
	char fragmentInfoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompilationSuccess);
	if(!fragmentCompilationSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog << std::endl;
	}
	
	// Create Program
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	// Check if its creation was successfull
	int programSuccess ;
	char shaderProgramInfoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
	if(!programSuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, shaderProgramInfoLog);
	}
}

void LoadObjects(){

	glGenBuffers(1, &VBO); 
	glGenVertexArrays(1, &VAO); 

	glBindVertexArray(VAO);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	
}

void RenderLoop(){
	while(!m_isClosed){
		glClearColor(0.2, 0.2, 0.5, 0.0); // Set the clear color. State setting function
		glClear(GL_COLOR_BUFFER_BIT);	// State using funtion
		Draw();
		Update(m_window);
	}
}

void Quit(){
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Update(SDL_Window * m_window){
	SDL_GL_SwapWindow(m_window);
	
	SDL_Event e;
	
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT){
			m_isClosed = true;
		}
	}
}

void Draw(){
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}