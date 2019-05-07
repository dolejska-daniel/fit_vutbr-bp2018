///
/// @file Curve.h
/// @brief
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <geGL/Texture.h>
#include <FreeImagePlus.h>
#include <geGL/StaticCalls.h>
#include <Terrain/HeightMap.h>


using namespace glm;
using namespace ge::gl;

namespace Utils
{
	//--------------------------------------------------------------------
	//	Převzato z: https://learnopengl.com/Advanced-OpenGL/Cubemaps
	//
	static float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	//--------------------------------------------------------------------

	static std::shared_ptr<ge::gl::Texture> create_texture2D(const GLsizei width, const GLsizei height, int max_mipmap_level, const GLvoid *data, const GLenum format = GL_RGBA, const GLenum type = GL_UNSIGNED_BYTE)
	{
		static GLfloat largest_supported_anisotropy = 0;
		if (largest_supported_anisotropy == 0)
			ge::gl::glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);

		const auto texture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGB8, max_mipmap_level, width, height);
		texture->texParameterfv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
		texture->setData2D(data, format, type);
		if (max_mipmap_level > 1)
		{
			texture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			texture->generateMipmap();
		}

		return texture;
	}

	static std::shared_ptr<ge::gl::Texture> create_texture2D(const GLsizei width, const GLsizei height, const GLvoid *data, const GLenum format = GL_RGBA, const GLenum type = GL_UNSIGNED_BYTE)
	{
		const auto max_mipmap_level = int(glm::ceil(glm::log2(float(width))));
		return create_texture2D(width, height, max_mipmap_level, data, format, type);
	}

	static void load_image_from_file(fipImage *image, const std::string& filepath, GLenum *format, GLenum *type)
	{
		image->load(filepath.c_str());
		*format = GL_RGB;
		*type = GL_UNSIGNED_BYTE;

		if (image->getImageType() == FIT_BITMAP) {
			switch (image->getBitsPerPixel())
			{
			case 24:
				*format = GL_BGR; break;
			case 32:
				*format = GL_BGRA; break;
			default:
				std::cerr << "Invalid BPP for loaded image" << std::endl;
			}
			*type = GL_UNSIGNED_BYTE;
		}
		if (image->getImageType() == FIT_RGBAF) {
			switch (image->getBitsPerPixel())
			{
			case 32 * 4:
				*format = GL_RGBA; break;
			case 32 * 3:
				*format = GL_RGB; break;
			default:
				std::cerr << "Invalid BPP for loaded image" << std::endl;
			}
			*type = GL_FLOAT;
		}
	}

	static FIBITMAP *create_image_from_raw(const unsigned width, const unsigned height, byte *data)
	{
		return FreeImage_ConvertFromRawBits(data, width, height, 3 * width, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	}

	static bool save_image_to_file(const std::string& filename, FIBITMAP *image, const FREE_IMAGE_FORMAT format = FIF_PNG)
	{
		const auto saved = FreeImage_Save(format, image, filename.c_str());
		return saved;
	}

	static bool save_image_to_file(const std::string& filename, const unsigned width, const unsigned height, byte *data, const FREE_IMAGE_FORMAT format = FIF_PNG)
	{
		const auto image = create_image_from_raw(width, height, data);
		return save_image_to_file(filename, image, format);
	}

	static byte *dump_framebuffer(const unsigned width, const unsigned height)
	{
		const auto data = new byte[3 * width * height];
		glReadBuffer(GL_BACK);
		glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, data);
		return data;
	}

	static void dump_framebuffer(const unsigned width, const unsigned height, byte *data)
	{
		glReadBuffer(GL_BACK);
		glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, data);
	}

	static byte *dump_terrain_noise(Terrain::HeightMap *height_map, const unsigned width, const unsigned height, const float scale)
	{
		const auto noise = new byte[3 * width * height];
		for (auto noise_y = 0u; noise_y < height; ++noise_y)
		{
			for (auto noise_x = 0u; noise_x < width; ++noise_x)
			{
				auto noise_p = vec2{ noise_x - .5f * width, noise_y - .5f * width };
				noise_p *= scale;

				const auto noise_h = height_map->GenerateNoise(noise_p.x, noise_p.y);
				const auto noise_c = byte(height_map->approximate_sample(noise_h) * 255);

				/*
				auto noise_ph = noise_h;
				height_map->PreprocessData(noise_ph);
				const auto noise_pc = byte(noise_ph * 254);
				*/

				const auto noise_index = 3 * noise_y * width + 3 * noise_x;
				noise[noise_index] =
					noise[noise_index + 1] =
					noise[noise_index + 2] = noise_c;
			}
		}
		return noise;
	}

	static unsigned load_cubemap(const std::vector<std::string>& faces)
	{
		//--------------------------------------------------------------------
		//	Převzato a upraveno z: https://learnopengl.com/Advanced-OpenGL/Cubemaps
		//
		unsigned textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		for (GLuint i = 0; i < faces.size(); i++)
		{
			fipImage face;
			GLenum format, type;
			load_image_from_file(&face, faces[i], &format, &type);
			face.flipHorizontal();
			face.flipVertical();

			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB8, face.getWidth(), face.getHeight(), 0, format, type, face.accessPixels()
			);

			face.clear();
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
		//--------------------------------------------------------------------
	}

	static std::shared_ptr<ge::gl::Texture> load_texture_from_file(const std::string& filepath)
	{
		fipImage image;
		GLenum format, type;
		load_image_from_file(&image, filepath, &format, &type);

		auto texture = create_texture2D(image.getWidth(), image.getHeight(), image.accessPixels(), format, type);

		image.clear();
		return texture;
	}

	template<typename T>
	static void merge_vectors(std::vector<T>& vec1, const std::vector<T>& vec2)
	{
		vec1.insert(std::end(vec1), std::begin(vec2), std::end(vec2));
	}

	static std::vector<glm::vec3> create_block_base(const std::vector<glm::vec3>& p, const float padding = 0.f, const float height = 0.f, float al = 0.f, float bl = 0.f)
	{
		if (al == 0.f)
			al = glm::length(p[1] - p[0]) * (1.f - padding);
		if (bl == 0.f)
			bl = glm::length(p[3] - p[0]) * (1.f - padding);
		const auto vl = glm::length(p[2] - p[0]);

		const auto a = glm::normalize(p[1] - p[0]);
		const auto b = glm::normalize(p[3] - p[0]);
		const auto v = glm::normalize(a * al + b * bl);
		const auto o = v * padding / 2.f * vl;

		std::vector<glm::vec3> points
		{
			p[0] + o,
			p[0] + o + a * al,
			p[0] + o + v * sqrt(al * al + bl * bl),
			p[0] + o + b * bl,
		};
		if (height != 0)
		{
			for (auto& point : points)
				point.y = height;
		}

		return points;
	}

	static std::vector<glm::vec3> move_vecs(const std::vector<glm::vec3>& v, const glm::vec3& dir, float len = 1)
	{
		auto o(v);
		for (auto& vec : o)
			vec += dir * len;
		return o;
	}

	static glm::vec3 random_dir_vec()
	{
		auto dir_rand = rand() % 2000 / 1000.f;
		dir_rand -= 1;

		return glm::normalize(glm::vec3{ glm::cos(dir_rand), 0, glm::sin(dir_rand) });
	}

	static float randomf(float from = 0.f, float to = 1.f)
	{
		from *= 1000.f;
		to *= 1000.f;
		auto r = rand() % int(to - from) / 1000.f;
		r += from;

		return r;
	}

	static float randomi(int from = 0, int to = 1)
	{
		to += 1;
		auto r = rand() % (to - from) / 1000;
		r += from;
		r -= 1;

		return r;
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	static float vec_angle(const glm::vec<L, T, Q>& u, const glm::vec<L, T, Q>& v)
	{
		return glm::angle(u, v) * 180.f / glm::pi<float>();
	}

	static glm::vec3 vec4to3(const glm::vec4& v)
	{
		return { v.x, v.y, v.z };
	}

	static glm::vec2 vec3to2(const glm::vec3& v)
	{
		return { v.x, v.z };
	}

	static glm::vec3 vec2to3(const glm::vec2& v)
	{
		return { v.x, 0, v.y };
	}

	static glm::vec4 vec3to4(const glm::vec3& v)
	{
		return { v.x, v.y, v.z, 0 };
	}

	const glm::vec3 color_white = { 1, 1, 1 };

	const glm::vec3 color_red = { 1, 0, 0 };

	const glm::vec3 color_green = { 0, 1, 0 };

	const glm::vec3 color_blue = { 0, 0, 1 };

	const glm::vec3 color_black = { 0, 0, 0 };

	static glm::vec3 color_rgb(const unsigned r, const unsigned g, const unsigned b)
	{
		return {
			r * (1.f / 255.f),
			g * (1.f / 255.f),
			b * (1.f / 255.f),
		};
	}

	static glm::vec4 color_rgba(const unsigned r, const unsigned g, const unsigned b, const float a)
	{
		return {
			r * (1.f / 255.f),
			g * (1.f / 255.f),
			b * (1.f / 255.f),
			a,
		};
	}

	static glm::vec3 right_vector(const glm::vec3& vec)
	{
		return {
			-vec.z,
			vec.y,
			vec.x,
		};
	}

	static glm::vec3 left_vector(const glm::vec3& vec)
	{
		return {
			vec.z,
			vec.y,
			-vec.x,
		};
	}

	static std::ifstream read_file(const std::string& filepath)
	{
		return std::ifstream(filepath.c_str());
	}

	static std::ofstream write_file(const std::string& filepath)
	{
		return std::ofstream(filepath.c_str());
	}

	static std::fstream open_file(const std::string& filepath)
	{
		return std::fstream(filepath.c_str());
	}

	static glm::vec2 tangent_intersection(const glm::vec2 a, const glm::vec2 b, const glm::vec2 v)
	{
		const auto u = (v.x - a.x) * (b.x - a.x) + (v.y - a.y) * (b.y - a.y)
			/ glm::pow(glm::length(b - a), 2.f);

		return {
			a.x + u * (b.x - a.x),
			a.y + u * (b.y - a.y)
		};
	}
}
