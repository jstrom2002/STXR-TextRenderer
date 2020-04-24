/*STXR -- Simple Text Renderer for OpenGL 3.3 Core
* MIT License
*
* Copyright (c) 2020 JH Strom
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once
#include <filesystem>
#include <iostream>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

namespace STXR {
	struct Texture {
		GLuint id = 0;
		int width = 0;
		int height = 0;
	};

	struct Shader {
		unsigned int ID;
		Shader();

		/*
		*  \brief Setters for shader uniforms.
		*/
		void setInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
		void setVec2(const std::string& name, const glm::vec2& value) const { glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
		void setVec2(const std::string& name, float x, float y) const { glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); }
		void setVec4(const std::string& name, const glm::vec4& value) const { glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
		void setVec4(const std::string& name, float x, float y, float z, float w) { glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); }
		void setMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

		/*
		*  \brief Compile error checking, prints an error message to console if necessary.
		*/
		void checkCompileErrors(GLuint shader, std::string type);		
	};

	struct Font {
		std::string name;
		Texture tx;
		unsigned int* VAO;
		float glyphWidth, glyphHeight;
		int charSetSize;
	};

	struct Text {
		Shader* shader;
		std::vector<Font> fonts;

		Text();
		~Text();

		/*
		*  \brief Loads font .bmps into the 'fonts' vector.
		*/
		void load(
			std::vector<std::string> fontnames,
			float w = 0.10f,//default width and height of glyphs
			float h = 0.065f
		);

		void load(
			std::string fontname,
			float scale_x = 0.75,
			float scale_y = 0.75,
			float w = 0.10f,//default width and height of glyphs
			float h = 0.065f
		);

		/*
		*  \brief Draw font at x,y onto the framebuffer.
		*/
		void draw(
			int fontChoice,
			std::string str,
			double x, double y,
			glm::vec4 texcol = glm::vec4(0, 0, 0, 1),//text black by default
			double newlineSpace = 0.0,
			double spaceBetween = 0.4,
			double scale_x = 1,//it's best not to scale fonts but you can
			double scale_y = 1,
			double area_w = 9999999999,//width and height of block where text will go, for formatting. default is set to not adjust lines
			double area_h = 9999999999
		);

		void draw(
			std::string fontname, std::string str,
			float x, float y,
			glm::vec4 texcol = glm::vec4(0, 0, 0, 1),
			double newlineSpace = 0.015,
			double spaceBetween = 0.4,
			double scale_x = 1,
			double scale_y = 1,
			double area_w = 9999999999,
			double area_h = 9999999999
		);

		//get index position of save font in loaded fonts vector
		int fontNameIndex(std::string fontname);

		bool fontExists(std::string fontname);

		// Returns the dimensions of a block of text as a vec3.
		glm::vec3 calcStringDimensions(
			std::string str,
			Font ft,
			float newlineSpacing = 0
		);
	};
}