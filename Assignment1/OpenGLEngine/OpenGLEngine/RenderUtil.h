#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <unordered_map>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Window.h"
#include "Color.h"
class Model;
class Shader;
namespace Reality
{
	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};

	class RenderUtil
	{
	public:
		RenderUtil();
		/*Initialization*/
		GLFWwindow* InitWindow(int width, int height, const char* title);

		/*Loaders*/
		Model* LoadModel(const std::string& model);
		void LoadShader(const std::string& vertex, const std::string& fragment);

		/*Draw*/
		void DrawModel(const std::string& modelPath, const std::string& vertex, const std::string& fragment);
		inline Shader& GetShader(const std::string& vertexShader, const std::string& fragmentShader) 
		{ 
			return shaderCache.find(vertexShader + fragmentShader) != shaderCache.end() ? *shaderCache.at(vertexShader + fragmentShader) : primitiveShader; 
		}

		/*Draw Primitives*/
		void DrawSphere(const glm::vec3& position = glm::vec3(0, 0, 0), const float& radius = 1, const Color& color = Color(0, 1, 0, 1));
		void DrawBuoyancySphere(const glm::vec3& position = glm::vec3(0, 0, 0), const float& radius = 1, const float& liquidHeight=0, const Color& colorAbove = Color(0, 1, 0, 1), const Color& colorBelow = Color(0, 1, 0, 1));
		void DrawCube(const glm::vec3& position = glm::vec3(0,0,0), const glm::vec3& scale = glm::vec3(1, 1, 1), const glm::vec3& rotation = glm::vec3(0, 0, 0), const Color& color = Color(0,1,0,1));
		void DrawLine(const glm::vec3& start, const glm::vec3& end, const Color& color = Color(0, 1, 0, 1));

		/*TextRender*/
		void RenderText(std::string text, float x, float y, float scale, Color color);

		/*Window Utils*/
		void ClearDisplay(GLFWwindow* window);
		void SwapBuffers(GLFWwindow* window);

		/*Camera And Window*/
		Camera camera;
		std::unique_ptr<Window> window;

		inline int GetVerts() { return verts; }
		inline int GetTris() { return triangles; }
		inline int GetLines() { return lines; }
		inline int GetDrawCalls() { return drawCalls; }
		inline float GetRenderDelta() { return renderDeltaTime; }


	private:
		/*Caches*/
		std::map<GLchar, Character> Characters;
		std::unordered_map<std::string, Model*> modelCache;
		std::unordered_map<std::string, Shader*> shaderCache;

		/*Primitive Buffers and Shaders*/
		unsigned int sphereVBO, sphereVAO, sphereIBO, numIndices;
		unsigned int cubeVBO, cubeVAO;
		unsigned int lineVBO, lineVAO;
		GLuint textVAO, textVBO;
		Shader primitiveShader;
		Shader primitiveShaderBasic;
		Shader primitiveTransparentShader;
		Shader buoyancyShader;
		Shader textShader;

		/*Set Up Primitives*/
		void SetupPrimitives();
		void SetupSpherePrimitive();
		void SetUpCubePrimitive();
		void SetUpLinePrimitive();
		void SetupPrimitiveShader();
		void SetupTextRender();

		/*Debug*/
		int verts;
		int triangles;
		int lines;
		int drawCalls;
		float renderDeltaTime;
	};
}

