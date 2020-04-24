/*STXR -- Simple Text Renderer
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
	std::vector<std::string> textvs() {
		std::vector<std::string> str{
			"#version 330 core\n",
			"layout(location = 0) in vec3 vertex;\n",
			"layout(location = 1) in vec2 aTexCoords;\n",
			"out vec2 TexCoords;\n",
			"uniform mat4 model;\n",
			"void main() {\n",
				"gl_Position = model * vec4(vertex.xy, 0.0, 1.0);\n",
				"TexCoords = aTexCoords;\n",
			"}\n"
		};
		return str;
	}

	std::vector<std::string> textfs() {
		std::vector<std::string> str = {
		"#version 330 core\n",
		"in vec2 TexCoords;\n",
		"out vec4 color;\n",
		"uniform sampler2D texture1;\n",
		"uniform vec4 textcolor;\n",
		"uniform vec2 textureSize;\n",
		"uniform bool smoothText = false;\n",

		"//function to average pixels in texture using gaussian weights to average them\n",
		"vec3 blurTexture(sampler2D hdrTex, vec2 texCoords, vec2 uPixelSize, int n) {\n",
			"vec3 neighbourhood = vec3(0);\n",
			"if (n == 1) {//1 pixel neighbor\n",
				"const float gauss_weight[9] =\n",
					"float[](0.000229, 0.005977, 0.060598, 0.241732, 0.382928, 0.241732, 0.060598, 0.005977, 0.000229);//sigma = 1.0\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-1, -1) * uPixelSize).xyz * gauss_weight[0];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+0, -1) * uPixelSize).xyz * gauss_weight[1];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+1, -1) * uPixelSize).xyz * gauss_weight[2];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-1, +0) * uPixelSize).xyz * gauss_weight[3];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+0, +0) * uPixelSize).xyz * gauss_weight[4];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+1, +0) * uPixelSize).xyz * gauss_weight[5];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-1, +1) * uPixelSize).xyz * gauss_weight[6];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+0, +1) * uPixelSize).xyz * gauss_weight[7];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+1, +1) * uPixelSize).xyz * gauss_weight[8];\n",
			"}/////////////////////////////////////////////////////////////////////////////////////\n",

			"if (n == 2) {//2 pixel neighbors\n",
				"const float gauss_weight[25] = float[]( //sigma = 4.0\n",
					"0.001133, 0.002316, 0.004450, 0.008033, 0.013627,\n",
					"0.021724, 0.032542, 0.045810, 0.060600, 0.075333,\n",
					"0.088001, 0.096603, 0.099654, 0.096603, 0.088001,\n",
					"0.075333, 0.060600, 0.045810, 0.032542, 0.021724,\n",
					"0.013627, 0.008033, 0.004450, 0.002316, 0.001133\n",
				");\n",

				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-2, -2) * uPixelSize).xyz * gauss_weight[0];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-1, -2) * uPixelSize).xyz * gauss_weight[1];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+0, -2) * uPixelSize).xyz * gauss_weight[2];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+1, -2) * uPixelSize).xyz * gauss_weight[3];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+2, -2) * uPixelSize).xyz * gauss_weight[4];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-2, -1) * uPixelSize).xyz * gauss_weight[5];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-1, -1) * uPixelSize).xyz * gauss_weight[6];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+0, -1) * uPixelSize).xyz * gauss_weight[7];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+1, -1) * uPixelSize).xyz * gauss_weight[8];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+2, -1) * uPixelSize).xyz * gauss_weight[9];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-2, +0) * uPixelSize).xyz * gauss_weight[10];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-1, +0) * uPixelSize).xyz * gauss_weight[11];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+0, +0) * uPixelSize).xyz * gauss_weight[12];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+1, +0) * uPixelSize).xyz * gauss_weight[13];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+2, +0) * uPixelSize).xyz * gauss_weight[14];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-2, +1) * uPixelSize).xyz * gauss_weight[15];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-1, +1) * uPixelSize).xyz * gauss_weight[16];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+0, +1) * uPixelSize).xyz * gauss_weight[17];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+1, +1) * uPixelSize).xyz * gauss_weight[18];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+2, +1) * uPixelSize).xyz * gauss_weight[19];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-2, +2) * uPixelSize).xyz * gauss_weight[20];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(-1, +2) * uPixelSize).xyz * gauss_weight[21];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+0, +2) * uPixelSize).xyz * gauss_weight[22];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+1, +2) * uPixelSize).xyz * gauss_weight[23];\n",
				"neighbourhood += texture2D(hdrTex, texCoords.xy + vec2(+2, +2) * uPixelSize).xyz * gauss_weight[24];\n",
			"}\n",
			"return neighbourhood;\n",
		"}///////////////////////////////////////////////\n",
		"void main() {\n",
			"vec4 sampled = texture(texture1, TexCoords);\n",
			"if(sampled.a <= 0.1){discard;}//discard low alpha values\n",
			"if (!smoothText) {\n",
				"if (sampled.r <= 0.3) { discard; }//no filtering\n",
			"}\n",
			"else {\n",
				"if (blurTexture(texture1, TexCoords, vec2(1.0 / textureSize.x, 1.0 / textureSize.y), 2).r <= 0.3) { discard; }//neighbor filtering\n",
			"}\n",
			"color = textcolor;\n",
		"}\n"
		};
		return str;
	}

	struct Texture {
		GLuint id = 0;
		int width = 0;
		int height = 0;
	};

	int charCount(std::string str, char toCount) {
		int count = 0;
		std::string str2 = " ";
		str2[0] = toCount;
		while (str.find(str2) != std::string::npos) {
			count++;
			str = str.substr(str.find(str2) + 1);
		}
		return count;
	}

	int numWords(std::string str) {
		if (str.size() <= 0) { return 0; }
		return charCount(str, ' ') + 1;
	}

	int numLines(std::string str) {
		if (str.size() <= 0) { return 0; }
		return charCount(str, '\n') + 1;
	}

	std::vector<std::string> tokenize(std::string toTokenize, std::string token) {
		std::vector<std::string> tokenizedStrings;
		std::string str;
		while (toTokenize.find(token) != std::string::npos) {
			str = toTokenize.substr(0, toTokenize.find(token));
			if (str.length() > 0) { tokenizedStrings.push_back(str); }
			toTokenize = toTokenize.substr(toTokenize.find(token) + token.length());
		}
		if (toTokenize.length() > 0) { tokenizedStrings.push_back(toTokenize); toTokenize = ""; }
		return tokenizedStrings;
	}

	int maxCharsPerLine(std::string str) {
		int lines = -1;
		while (str.find("\n") != std::string::npos) {
			if (str.find("\n") > lines) { lines = str.find("\n"); }
			str = str.substr(str.find("\n") + 1);
		}
		if (str.length() > lines) { lines = str.length(); }
		return lines;
	}

	Texture loadTexture(char const* path, bool flip=false) {
		Texture tx;
		glGenTextures(1, &tx.id);

		if(flip)stbi_set_flip_vertically_on_load(true);
		int width, height, nrComponents;
		unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 4);
		if (flip)stbi_set_flip_vertically_on_load(false);
		if (data == nullptr) { return tx; }
		if (data) {
			GLenum internalFormat = GL_RGBA;
			GLenum dataFormat = GL_RGBA;

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glBindTexture(GL_TEXTURE_2D, tx.id);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);		
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//texture filter settings -- must be trilinear or text won't look smooth.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			tx.height = height;
			tx.width = width;
		}
		else {
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		return tx;
	}

	std::string getNextWord(std::string str) {
		if (str.find(" ") != std::string::npos) {
			return str.substr(0, str.find(" ") + 1);
		}
		return str;
	}

	std::string getNextLine(std::string str) {
		if (str.find("\n") != std::string::npos) {
			return str.substr(0, str.find("\n"));
		}
		return str;
	}

	class Shader {
	public:
		unsigned int ID;
		Shader() {
			std::vector<std::string> vertexPath = textvs();
			std::vector<std::string> fragmentPath = textfs();
			std::string vertexCode = "";
			for (int i = 0; i < vertexPath.size(); ++i) {
				vertexCode.append(vertexPath[i]);
			}
			std::string fragmentCode = "";	
			for (int i = 0; i < fragmentPath.size(); ++i) {
				fragmentCode.append(fragmentPath[i]);
			}
			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();
			// 2. compile shaders
			unsigned int vertex, fragment;
			int success;
			char infoLog[512];
			// vertex shader
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vShaderCode, NULL);
			glCompileShader(vertex);
			checkCompileErrors(vertex, "VERTEX");
			// fragment Shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fShaderCode, NULL);
			glCompileShader(fragment);
			checkCompileErrors(fragment, "FRAGMENT");
			// shader Program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			checkCompileErrors(ID, "PROGRAM");
			// delete the shaders as they're linked into our program now and no longer necessery
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}

		void use() { glUseProgram(ID); }
		void setBool(const std::string& name, bool value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
		void setInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
		void setFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }
		void setVec2(const std::string& name, const glm::vec2& value) const { glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
		void setVec2(const std::string& name, float x, float y) const { glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); }
		void setVec3(const std::string& name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
		void setVec3(const std::string& name, float x, float y, float z) const { glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); }
		void setVec4(const std::string& name, const glm::vec4& value) const { glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
		void setVec4(const std::string& name, float x, float y, float z, float w) { glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); }
		void setMat2(const std::string& name, const glm::mat2& mat) const { glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
		void setMat3(const std::string& name, const glm::mat3& mat) const { glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
		void setMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	protected:
		void checkCompileErrors(GLuint shader, std::string type) {
			GLint success;
			GLchar infoLog[1024];
			if (type != "PROGRAM") {
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " <<
						type << "\n" << infoLog << "\n";
				}
			}
			else {
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " <<
						type << "\n" << infoLog << "\n";
				}
			}
		}
	};

	struct TextItem {//used to hold all the variables required for drawing a text string
		std::string fontname = "";
		std::string textstring = "";
		glm::vec3 position = glm::vec3(0);
		glm::vec4 color = glm::vec4(0, 0, 0, 1);
		float newlineSpace = 0.0f;
		float spaceBetween = 0.4f;
		glm::vec3 scale = glm::vec3(1);
	};

	class TextSettings {
	public:
		int fontChoice = 0;
		std::string str = "";
		float x = 0;//x,y are the NDC screen coordinates where the text will begin to appear 
		float y = 0;
		glm::vec4 texcol = glm::vec4(0, 0, 0, 1);//text is opaque black by default
		float newlineSpace = 0.0;
		float spaceBetween = 0.45;
		float scale_x = 1;//it's best not to scale fonts but you can
		float scale_y = 1;
		float area_w = 9999999999;//width and height of block where text will go; for formatting. default is set to not adjust lines
		float area_h = 9999999999;
	};
	struct Font {
		std::string name;
		Texture tx;
		unsigned int* VAO;
		float glyphWidth, glyphHeight;
		int charSetSize;
	};

	class Text {
	public:
		Shader* shader;
		std::vector<Font> fonts;

		Text() {}
		~Text() {
			for (int i = 0; i < fonts.size(); ++i) {
				glDeleteVertexArrays(fonts[i].charSetSize, fonts[i].VAO);
				glDeleteTextures(1, &fonts[i].tx.id);
			}
		}

		void load(//loads font .bmps into the 'fonts' vector -- ALL load functions go through this one
			std::vector<std::string> fontnames,
			float w = 0.10f,//default width and height of glyphs
			float h = 0.065f
		) {
			if (shader == nullptr) {
				shader = new Shader();
				shader->use();
				shader->setMat4("model", glm::mat4(1.0f));
				shader->setInt("texture1", 0);
			}

			for (int n = 0; n < fontnames.size(); n++) {
				Font ft;
				ft.name = fontnames[n].substr(fontnames[n].rfind("/") + 1, fontnames[n].length());//get file name from path
				while (ft.name.find(".bmp") != std::string::npos) {//remove file extension from the end of file name
					ft.name = ft.name.substr(0, ft.name.length() - 4);
				}
				ft.charSetSize = 126 - 32;//default -- load only english non-unicode chars (ie nothing above 126 on the ASCII table)
				ft.VAO = new unsigned int[ft.charSetSize];
				glGenVertexArrays(ft.charSetSize, ft.VAO);

				ft.tx = loadTexture(fontnames[n].c_str());
				int tx_x, tx_y;
				tx_x = sqrt(ft.tx.width);
				tx_y = ft.tx.height / tx_x;//allows textures that are non-square (ie height < width) to pack chars in
				ft.glyphWidth = w;
				ft.glyphHeight = h;

				float cx, cy,//variables for glyph creation
					u1, u2, v1, v2;
				float zdepth = 0;

				for (int i = 0; i < ft.charSetSize; i++) {
					cx = float(i % tx_x) / ((float)tx_x);
					cy = float(floor(i / tx_x)) / ((float)tx_y);

					u1 = cx;
					u2 = cx + (1.0 / tx_x);
					v1 = -(1 - cy);
					v2 = -(1 - cy - (1.0 / tx_y));
					float glyph_h = ft.glyphWidth;
					float glyph_w = ft.glyphHeight;

					float quadVertices[] = {
						// positions           // texCoords
						 0.0f, glyph_h,  zdepth,    u1,  v1,
						 0.0f,     0.0f,  zdepth,    u1,  v2,
						glyph_w,  0.0f,  zdepth,    u2,  v2,

						 0.0f,    glyph_h,  zdepth, u1,  v1,
						glyph_w, 0.0f,      zdepth, u2,  v2,
						glyph_w, glyph_h,  zdepth, u2,  v1
					};
					// setup plane VAO
					unsigned int quadVBO;
					glGenBuffers(1, &quadVBO);
					glBindVertexArray(ft.VAO[i]);
					glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				}
				fonts.push_back(ft);
			}
		}

		void Draw(//draw font at x,y -- ALL Draw() functions go through this one
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
		) {
			if (fontChoice <= -1) { return; }

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fonts[fontChoice].tx.id);

			//setup shader and variables
			shader->use();
			shader->setVec4("textcolor", texcol);
			shader->setVec2("textureSize", glm::vec2(fonts[fontChoice].tx.width, fonts[fontChoice].tx.height));
			float pos = x;
			float spacing = 0;

			//apply behaviors
			//for (int i = 0; i < onTime.size(); ++i) {onTime[i](this);}

			//draw loop
			std::string linestr = "";
			std::string wordstr = "";
			int i_max, j_max;
			i_max = numLines(str);
			for (int i = 0; i < i_max; ++i) {

				linestr = getNextLine(str);
				if (str.find("\n") != std::string::npos) { str = str.substr(str.find("\n") + 1); }
				else { str = ""; }

				j_max = numWords(linestr);
				for (int j = 0; j < j_max; ++j) {

					wordstr = getNextWord(linestr);
					if (linestr.find(" ") != std::string::npos) { linestr = linestr.substr(linestr.find(" ") + 1); }
					else { linestr = ""; }

					//if text is out of area, insert new line
					if (pos >=
						area_w - pos -
						(wordstr.size() * (fonts[fontChoice].glyphWidth * spaceBetween))
						) {
						spacing += fonts[fontChoice].glyphHeight + newlineSpace;
						pos = x;
					}

					for (int n = 0; n < wordstr.size(); ++n) {
						shader->setMat4("model",
							glm::scale(
								glm::translate(glm::mat4(1.0), glm::vec3(pos, y - spacing, 0.0)),//needs adjustment
								glm::vec3(scale_x, scale_y, 1)
							)
						);
						glBindVertexArray(fonts[fontChoice].VAO[wordstr[n] - 32]);
						glDrawArrays(GL_TRIANGLES, 0, 6);
						pos += fonts[fontChoice].glyphWidth * spaceBetween;
						glBindVertexArray(0);
					}
				}
				//add space to next line
				spacing += fonts[fontChoice].glyphHeight + newlineSpace;
				pos = x;
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Draw(TextSettings ts) {
			Draw(
				ts.fontChoice,
				ts.str,
				ts.x,
				ts.y,
				ts.texcol,
				ts.newlineSpace,
				ts.spaceBetween,
				ts.scale_x,
				ts.scale_y,
				ts.area_w,
				ts.area_h
			);
		}

		void load(//loads only one single font
			std::string fontname,
			float scale_x = 0.75,
			float scale_y = 0.75,
			float w = 0.10f,//default width and height of glyphs
			float h = 0.065f
		) {
			w *= scale_x;
			h *= scale_y;
			if (checkIfFontExists(fontname)) { return; }//dont load same font twice
			std::vector<std::string> fns;
			fns.push_back(fontname);
			load(fns, w, h);//call main 'load()' function
		}

		void load(//loads all fonts
			float scale_x = 0.75,
			float scale_y = 0.75,
			float w = 0.10f,//default width and height of glyphs
			float h = 0.065f
		) {
			w *= scale_x;
			h *= scale_y;
			std::vector<std::string> fns;
			load(fns, w, h);//call main 'load()' function
		}

		void Draw(std::string fontname, std::string str,
			float x, float y,//secondary Draw() method for letting system choose font
			glm::vec4 texcol = glm::vec4(0, 0, 0, 1),
			double newlineSpace = 0.015,
			double spaceBetween = 0.4,
			double scale_x = 1,
			double scale_y = 1,
			double area_w = 9999999999,
			double area_h = 9999999999
		) {
			if (!checkIfFontExists(fontname)) {//load font if necessary 
				fontname = "GUI/fonts/" + fontname + ".bmp";
				load(fontname);
			}
			Draw(fontNameIndex(fontname), str, x, y, texcol, newlineSpace, 
				spaceBetween, scale_x, scale_y, area_w, area_h);
		}

		void Draw(std::string fontname, std::string str, glm::vec3 pos,
			glm::vec4 texcol = glm::vec4(0, 0, 0, 1),//text black by default
			double newlineSpace = 0.015,
			double spaceBetween = 0.4,
			glm::vec3 scale = glm::vec3(1),
			double area_w = 9999999999,
			double area_h = 9999999999
		) {
			if (!checkIfFontExists(fontname)) {//load font if necessary 
				fontname = "GUI/fonts/" + fontname + ".bmp";
				load(fontname);
			}
			Draw(fontNameIndex(fontname), str, pos.x, pos.y, texcol, newlineSpace, 
				spaceBetween, scale.x, scale.y, area_w, area_h);
		}

		void Draw(
			TextItem txitm,
			float area_w = 9999999999,//width and height of block where text will go, for formatting. default is set to not adjust lines
			float area_h = 9999999999
		) {
			Draw(txitm.fontname, txitm.textstring, txitm.position,
				txitm.color, txitm.newlineSpace, txitm.spaceBetween, txitm.scale, area_w, area_h);
		}

		int fontNameIndex(std::string fontname) {//get index position of save font in loaded fonts vector		
			for (int i = 0; i < fonts.size(); ++i) {
				if (fonts[i].name == fontname) {
					return i;
				}
			}
			return -1;
		}

		bool checkIfFontExists(std::string fontname) {
			for (int i = 0; i < fonts.size(); ++i) {
				if (fonts[i].name == fontname) {
					return true;
				}
			}
			return false;
		}

		glm::vec3 calcStringDimensions(//returns the dimensions of a block of text as a vec3
			std::string str,
			Font ft,
			float newlineSpacing = 0
		) {
			return glm::vec3(
				maxCharsPerLine(str) * ft.glyphWidth,
				numLines(str) * ft.glyphHeight + newlineSpacing * (numLines(str) - 1),
				0
			);
		}

		glm::vec3 calcStringDimensions(TextItem txt) {//returns the dimensions of a block of text as a vec3
			if (fontNameIndex(txt.fontname) < 0) { this->load(txt.fontname); }//do a check for existance of the font
			return calcStringDimensions(txt.textstring, this->fonts[fontNameIndex(txt.fontname)], txt.newlineSpace);
		}
	};
}
