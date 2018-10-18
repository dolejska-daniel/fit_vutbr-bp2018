///
/// @file IRenderable.h
/// @brief Obsahuje deklaraci rozhraní Application::IRenderable.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <geGL/VertexArray.h>
#include <geGL/Buffer.h>


namespace Application
{
	///
	/// @brief 
	///
	class IRenderable
	{
	public:
		///
		/// @brief 
		///
		inline std::shared_ptr<ge::gl::VertexArray> GetVA() const { return _va; }
		///
		/// @brief 
		///
		inline void SetVA(std::shared_ptr<ge::gl::VertexArray> vertexArray) { _va = vertexArray; }
		///
		/// @brief 
		///
		inline std::shared_ptr<ge::gl::Buffer> GetVB() const { return _vb; }
		///
		/// @brief 
		///
		inline void SetVB(std::shared_ptr<ge::gl::Buffer> vertexBuffer) { _vb = vertexBuffer; }

		///
		/// @brief 
		///
		inline GLenum GetDrawMode() const { return _drawMode; }
		///
		/// @brief 
		///
		inline void SetDrawMode(GLenum mode) { _drawMode = mode; }

	private:
		///
		/// @brief 
		///
		std::shared_ptr<ge::gl::VertexArray> _va;
		///
		/// @brief 
		///
		std::shared_ptr<ge::gl::Buffer> _vb;

		///
		/// @brief 
		///
		GLenum _drawMode = GL_TRIANGLES;
	};
}
