///
/// @file ShaderManager.h
/// @brief Obsahuje deklaraci pro třídu ShaderManager.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <string>
#include <unordered_map>
#include <geGL/Program.h>
#include <Application/Application.h>


namespace Application
{
	///
	/// @brief Tato třída umožňuje jednoduchou správu shaderů na GPU.
	///
	class ShaderManager
	{
	public:
		///
		/// @brief Aktuálně běžící program na GPU.
		///
		std::shared_ptr<ge::gl::Program> activeProgram;

		///
		/// @brief Inicializuje novou instanci třídy.
		///
		/// @param[in,out]	_vars::Vars&	_vars	Proměnné programu
		///
		ShaderManager(vars::Vars& vars);
		///
		/// @brief Zruší dynamicky alokované prvky a tuto instanci.
		///
		~ShaderManager();

		///
		/// @brief Aktivuje soubor shaderů na GPU na základě jejich jména.
		///
		/// V případě, že soubor se shadery ještě nebyl zpracován, použije
		/// funkce pro načtení a zpracování souborů se shadery
		/// (@see Application::ShaderLoader), které následně zkompiluje,
		/// nahraje na GPU a uloží do proměnné instance.
		/// 
		/// Pokud byl soubor se shadery již zpracován, program na GPU pouze
		/// přebinduje a aktivuje. Aktualizuje proměnnou `activeProgram`.
		///
		/// @param[in]	std::string	programName	Název souboru s shadery
		///
		void Use(const std::string programName);

	private:
		///
		/// @brief Proměnné programu.
		///
		vars::Vars& _vars;
		///
		/// @brief Obsahuje zpracované programy se shadery.
		///
		std::unordered_map<std::string, std::shared_ptr<ge::gl::Program>> _programs;

		///
		/// @brief Provede bind programu na GPU a aktualizaci proměnné `activeProgram`.
		///
		void BindProgram(std::shared_ptr<ge::gl::Program> program);
	};
}
