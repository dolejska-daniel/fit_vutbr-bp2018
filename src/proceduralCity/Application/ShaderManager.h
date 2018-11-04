///
/// @file ShaderManager.h
/// @brief Obsahuje deklaraci pro třídu ShaderManager.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <string>
#include <unordered_map>


namespace Application
{
	///
	/// @brief Tato třída umožňuje jednoduchou správu shaderů na GPU.
	///
	class ShaderManager
	{
	public:
		///
		/// @brief Inicializuje novou instanci třídy.
		///
		/// @param[in,out]	vars	Proměnné programu
		///
		ShaderManager(vars::Vars& vars);
		///
		/// @brief Zruší dynamicky alokované prvky a tuto instanci.
		///
		~ShaderManager();

		///
		/// @brief Získá odkaz na aktivní program na GPU.
		///
		std::shared_ptr<ge::gl::Program> &GetActiveProgram() { return _activeProgram; }
		///
		/// @brief Získá aktivní program na GPU pouze pro čtení.
		///
		std::shared_ptr<ge::gl::Program> ReadActiveProgram() const { return _activeProgram; }

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
		/// @param[in]	programName	Název souboru s shadery
		///
		void Use(std::string const& programName);

	private:
		///
		/// @brief Aktuálně běžící program na GPU.
		///
		std::shared_ptr<ge::gl::Program> _activeProgram;
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
		void BindProgram(std::shared_ptr<ge::gl::Program> const& program);
	};
}
