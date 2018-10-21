///
/// @file IRenderableBase.h
/// @brief Obsahuje deklaraci rozhraní Application::IRenderableBase.
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
	class IRenderableBase
	{
	public:
		///
		/// @brief 
		///
		inline std::shared_ptr<ge::gl::VertexArray> GetVA() const { return _va; }
		///
		/// @brief 
		///
		inline std::shared_ptr<ge::gl::VertexArray> CreateVA() { return _va = std::make_shared<ge::gl::VertexArray>(); }
		///
		/// @brief 
		///
		inline void BindVA() const { if (_va != nullptr) _va->bind(); }
		///
		/// @brief 
		///
		inline std::shared_ptr<ge::gl::Buffer> GetVB() const { return _vb; }
		///
		/// @brief 
		///
		inline std::shared_ptr<ge::gl::Buffer> CreateVB() { return _vb = std::make_shared<ge::gl::Buffer>(); }
		///
		/// @brief 
		///
		inline void BindVB(GLenum const&target = GL_ARRAY_BUFFER) const { if (_vb != nullptr) _vb->bind(target); }
		///
		/// @brief 
		///
		inline std::shared_ptr<ge::gl::Buffer> GetIB() const { return _ib; }
		///
		/// @brief 
		///
		inline std::shared_ptr<ge::gl::Buffer> CreateIB() { return _ib = std::make_shared<ge::gl::Buffer>(); }
		///
		/// @brief 
		///
		inline void BindIB(GLenum const&target = GL_ELEMENT_ARRAY_BUFFER) const { if (_ib != nullptr) _ib->bind(target); }

		///
		/// @brief 
		///
		inline GLenum GetDrawMode() const { return _drawMode; }
		///
		/// @brief 
		///
		inline void SetDrawMode(GLenum mode) { _drawMode = mode; }

		///
		/// @brief 
		///
		inline void Bind() const { BindVA(); BindVB(); BindIB(); }

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
		std::shared_ptr<ge::gl::Buffer> _ib;

		///
		/// @brief 
		///
		GLenum _drawMode = GL_TRIANGLES;
	};
}
