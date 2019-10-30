#include "RenderUtil.h"
#include <iostream>
#include <glm/gtx/vector_angle.hpp>
#include <mutex>
//#include "Model.h"
//#include "Shader.h"

namespace Reality
{
	RenderUtil::RenderUtil()
	{
		window = std::make_unique<Window>();
		window->glfwWindow = InitWindow(window->width, window->height, "RealityEngine-v1.0");
		SetupPrimitiveShader();
		SetupSpherePrimitive();
		SetUpCubePrimitive();
		SetUpLinePrimitive();
		SetupTextRender();
	}


	GLFWwindow* RenderUtil::InitWindow(int width, int height, const char* title)
	{
		// glfw: initialize and configure
			// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		// glfw window creation
		// --------------------
		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return NULL;
		}
		glfwMakeContextCurrent(window);

		// tell GLFW to capture our mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return NULL;
		}

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
		return window;
	}

	Model* RenderUtil::LoadModel(const std::string& model)
	{
		Model* newModel = new Model(model);
		modelCache[model] = newModel;
		return newModel;
	}

	void RenderUtil::LoadShader(const std::string& vertex, const std::string& fragment)
	{
		shaderCache[vertex + fragment] = new Shader(vertex.c_str(), fragment.c_str());
	}

	void RenderUtil::DrawModel(const std::string& model, const std::string& vertex, const std::string& fragment)
	{
		float time = glfwGetTime();
		if (modelCache.find(model) != modelCache.end())
		{
			Model* modelToDraw = modelCache[model];
			if (shaderCache.find(vertex + fragment) != shaderCache.end())
			{
				modelToDraw->Draw(*shaderCache[vertex + fragment]);
			}
			else
			{
				modelToDraw->Draw(primitiveShader);
			}
			verts += modelToDraw->verts;
			triangles += modelToDraw->verts / 3;
			drawCalls += modelToDraw->meshes.size();
		}
		renderDeltaTime += glfwGetTime() - time;
	}

	void RenderUtil::ClearDisplay(GLFWwindow* window)
	{
		glClearColor(0.0f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		verts = 0;
		triangles = 0;
		lines = 0;
		drawCalls = 0;
		renderDeltaTime = 0;
	}

	void RenderUtil::SwapBuffers(GLFWwindow* window)
	{
		glfwSwapBuffers(window);
	}

	void RenderUtil::SetUpCubePrimitive()
	{
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(cubeVAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void RenderUtil::SetupSpherePrimitive()
	{
		std::vector<float> VNT;
		std::vector<int> indices;

		float radius = 1;
		float PI = 3.141592f;
		int sectorCount = 72;
		int stackCount = 24;

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				VNT.push_back(x);
				VNT.push_back(y);
				VNT.push_back(z);

				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				VNT.push_back(nx);
				VNT.push_back(ny);
				VNT.push_back(nz);

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				VNT.push_back(s);
				VNT.push_back(t);
			}
		}

		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}



		glGenBuffers(1, &sphereVBO);
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);           // for vertex data
		glBufferData(GL_ARRAY_BUFFER,                   // target
			VNT.size() * sizeof(float),			// data size, # of bytes
			&VNT[0],								// ptr to vertex data
			GL_STATIC_DRAW);							// usage

		// copy index data to VBO
		glGenBuffers(1, &sphereIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIBO);   // for index data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,				// target
			indices.size() * sizeof(int),					// data size, # of bytes
			&indices[0],									// ptr to index data
			GL_STATIC_DRAW);								// usage

		numIndices = indices.size();

		glGenVertexArrays(1, &sphereVAO);
		glBindVertexArray(sphereVAO);

		// bind VBOs
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIBO);

		// activate attrib arrays
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// set attrib arrays with stride and offset
		int stride = 8 * sizeof(float) ;     // should be 32 bytes
		glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, (void*)(sizeof(float) * 6));

	}

	void RenderUtil::DrawSphere(const glm::vec3& position, const float& radius, const Color& color)
	{
		glBindVertexArray(sphereVAO);
		primitiveShader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window->width / (float)window->height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		primitiveShader.setMat4("projection", projection);
		primitiveShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(radius, radius, radius));
		primitiveShader.setMat4("model", model);

		primitiveShader.setVec3("col", glm::vec3(color.r, color.g, color.b));

		// bind VBOs
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT,(void*)0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	void RenderUtil::DrawBuoyancySphere(const glm::vec3& position, const float& radius, const float& liquidHeight, 
		const Color& colorAbove, const Color& colorBelow)
	{
		glBindVertexArray(sphereVAO);
		buoyancyShader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window->width / (float)window->height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		buoyancyShader.setMat4("projection", projection);
		buoyancyShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(radius, radius, radius));
		buoyancyShader.setMat4("model", model);

		buoyancyShader.setFloat("liquidHeight", liquidHeight);
		buoyancyShader.setVec4("colAbove", glm::vec4(colorAbove.r, colorAbove.g, colorAbove.b, colorAbove.a));
		buoyancyShader.setVec4("colBelow", glm::vec4(colorBelow.r, colorBelow.g, colorBelow.b, colorBelow.a));

		// bind VBOs
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}


	void RenderUtil::SetUpLinePrimitive()
	{
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			// positions         
			 0.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, 0.0f
		};

		glGenVertexArrays(1, &lineVAO);
		glGenBuffers(1, &lineVBO);

		glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(lineVAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void RenderUtil::SetupPrimitiveShader()
	{
		primitiveShader = Shader("Shaders/vertexDefault.vs", "Shaders/FragmentConstant.fs");
		primitiveTransparentShader = Shader("Shaders/vertexDefault.vs", "Shaders/FragmentTransparent.fs");
		primitiveShaderBasic = Shader("Shaders/SimpleVertex.vs", "Shaders/FragmentConstant.fs");
		textShader = Shader("Shaders/text.vs", "Shaders/text.fs");
		buoyancyShader = Shader("Shaders/VertexBuoyancy.vs", "Shaders/FragmentBuoyancy.fs");
	}

	void RenderUtil::SetupTextRender()
	{
		// Setup the shader
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(window->width), 0.0f, static_cast<GLfloat>(window->height));
		textShader.use();
		glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// FreeType
		FT_Library ft;
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

		// Load font as face
		FT_Face face;
		if (FT_New_Face(ft, "Fonts/Arial.ttf", 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Load first 128 characters of ASCII set
		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		// Destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);


		// Configure VAO/VBO for texture quads
		glGenVertexArrays(1, &textVAO);
		glGenBuffers(1, &textVBO);
		glBindVertexArray(textVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void RenderUtil::RenderText(std::string text, float x, float y, float scale, Color color)
	{
		float time = glfwGetTime();
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Activate corresponding render state	
		textShader.use();
		glUniform3f(glGetUniformLocation(textShader.ID, "textColor"), color.r, color.g, color.b);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(textVAO);

		// Iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			GLfloat xpos = x + ch.Bearing.x * scale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			// Update VBO for each character
			GLfloat vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },

				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
			};
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, textVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			verts += 6;
			triangles += 2;
			drawCalls++;
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		renderDeltaTime += glfwGetTime() - time;
	}

	void RenderUtil::DrawCube(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const Color& color)
	{
		float time = glfwGetTime();
		glBindVertexArray(cubeVAO);
		primitiveTransparentShader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window->width / (float)window->height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		primitiveTransparentShader.setMat4("projection", projection);
		primitiveTransparentShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		primitiveTransparentShader.setMat4("model", model);

		primitiveTransparentShader.setVec4("col", glm::vec4(color.r, color.g, color.b, color.a));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		verts += 36;
		triangles += 12;
		drawCalls++;
		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderDeltaTime += glfwGetTime() - time;
	}

	void RenderUtil::DrawLine(const glm::vec3& start, const glm::vec3& end, const Color& color)
	{
		float time = glfwGetTime();
		glBindVertexArray(lineVAO);
		primitiveShaderBasic.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window->width / (float)window->height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		primitiveShaderBasic.setMat4("projection", projection);
		primitiveShaderBasic.setMat4("view", view);

		glm::vec3 position = (start + end) * 0.5f;
		float scale = glm::length(start - end);
		glm::vec3 axis = glm::cross(glm::vec3(1, 0, 0), end - start);
		axis = glm::normalize(axis);
		float proj = glm::dot(end - start, glm::vec3(1, 0, 0));
		/*glm::vec3 projVec = proj * glm::vec3(1, 0, 0);
		glm::vec3 perpVec = end - start - proj;
		float perp = 
		float angle = atan2(glm::length(perp), glm::length(proj));*/
		// world transformation
		//float angle = glm::ori
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, start);
		model = glm::rotate(model, glm::orientedAngle(glm::vec3(1, 0, 0), glm::normalize(end - start), axis), axis);
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		primitiveShaderBasic.setMat4("model", model);

		primitiveShaderBasic.setVec3("col", glm::vec3(color.r, color.g, color.b));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_LINES, 0, 2);
		verts += 2;
		lines += 1;
		drawCalls++;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderDeltaTime += glfwGetTime() - time;
	}
}
