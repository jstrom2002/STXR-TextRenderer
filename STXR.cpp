#include "STXR.h"

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

		"//Function to average pixels in texture using gaussian weights to average them.\n",
		"//This can smooth character edges but also can exaggerate stray values.\n",
		"//Use wisely.\n",
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

	Shader::Shader() {
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

	void Shader::checkCompileErrors(GLuint shader, std::string type) {
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


		Text::Text() {}
		Text::~Text() {
			for (int i = 0; i < fonts.size(); ++i) {
				glDeleteVertexArrays(fonts[i].charSetSize, fonts[i].VAO);
				glDeleteTextures(1, &fonts[i].tx.id);
			}
		}

		void Text::load(//loads font .bmps into the 'fonts' vector -- ALL load functions go through this one
			std::vector<std::string> fontnames,
			float w,
			float h
		) {
			if (shader == nullptr) {
				shader = new Shader();
				glUseProgram(shader->ID);
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
		void Text::draw(
			int fontChoice,
			std::string str,
			double x, double y,
			glm::vec4 texcol,
			double newlineSpace,
			double spaceBetween,
			double scale_x,
			double scale_y,
			double area_w,
			double area_h
		) {
			if (fontChoice <= -1) { return; }

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fonts[fontChoice].tx.id);

			//setup shader and variables
			glUseProgram(shader->ID);
			shader->setVec4("textcolor", texcol);
			shader->setVec2("textureSize", glm::vec2(fonts[fontChoice].tx.width, fonts[fontChoice].tx.height));
			float pos = x;
			float spacing = 0;

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

		void Text::load(
			std::string fontname,
			float scale_x,
			float scale_y,
			float w,
			float h
		) {
			w *= scale_x;
			h *= scale_y;
			if (fontExists(fontname)) {
				return; 
			}
			std::vector<std::string> fns;
			fns.push_back(fontname);
			load(fns, w, h);
		}

		void Text::draw(
			std::string fontname, std::string str,
			float x, float y,
			glm::vec4 texcol,
			double newlineSpace,
			double spaceBetween,
			double scale_x,
			double scale_y,
			double area_w,
			double area_h
		) {
			if (!fontExists(fontname)) {
				fontname = "GUI/fonts/" + fontname + ".bmp";
				load(fontname);
			}
			draw(fontNameIndex(fontname), str, x, y, texcol, newlineSpace, 
				spaceBetween, scale_x, scale_y, area_w, area_h);
		}

		int Text::fontNameIndex(std::string fontname) {	
			for (int i = 0; i < fonts.size(); ++i) {
				if (fonts[i].name == fontname) {
					return i;
				}
			}
			return -1;
		}

		bool Text::fontExists(std::string fontname) {
			for (int i = 0; i < fonts.size(); ++i) {
				if (fonts[i].name == fontname) {
					return true;
				}
			}
			return false;
		}

		glm::vec3 Text::calcStringDimensions(std::string str,Font ft,float newlineSpacing) {
			return glm::vec3(maxCharsPerLine(str) * ft.glyphWidth,
				numLines(str) * ft.glyphHeight + newlineSpacing * (numLines(str) - 1),0);
		}
}
