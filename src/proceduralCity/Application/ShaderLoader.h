///
/// @file ShaderLoader.h
/// @brief Obsahuje deklaraci namespace Apllication::ShaderLoader.
///
/// @author Daniel Dolejška <xdolej08@stud.fit.vutbr.cz>
///
#pragma once
#include <string>
#include <unordered_map>
#include <Application/Application.h>


namespace Application
{
	///
	/// @brief Obsahuje funkce nezbytné pro práci se zdrojovými soubory shaderů.
	///
	namespace ShaderLoader
	{
		///
		/// @brief Otevře nový souborový stream pro poskytnutou cestu.
		///
		/// @param[in]	filepath	Cesta k `.shader` souboru
		///
		/// @returns	Souborový stream
		///
		std::ifstream OpenFile(std::string const& filepath);
		///
		/// @brief Uzavře poskytnutý souborový stream.
		///
		/// @param[in,out]	file	Otevřený souborový stream
		///
		void CloseFile(std::ifstream& file);
		///
		/// @brief Získá zdrojový kód ze souboru daného jména.
		///
		/// Pokusí se otevřít a načíst zdrojový kód shaderů ze souboru daného
		/// jména z aktuální `resources` složky a podsložky `shaders` (
		///	`{RESOURCES}/shaders/{filename}.shader`).
		///
		/// @param[in]	filename	Název `.shader` souboru
		///
		/// @returns	Zdrojový kód z daného souboru
		///
		ShaderSources GetShaderSources(std::string const& filename);

		///
		/// @brief Seznam podporovaných typů.
		///
		///	Obsahuje enum typ shaderu (@see ShaderLoader::ShaderType) mapovaný
		/// na svou textovou reprezentaci (`string` => `ShaderType`).
		///
		extern const std::unordered_map<std::string, ShaderType> Types;
		///
		/// @brief Seznam zpracovaných souborů.
		///
		///	Obsahuje zpracovaný zdrojový kód shaderů (@see ShaderLoader::ShaderSources)
		///	mapovaný na jméno souboru, ze kterého byl kód načten (`string` =>
		///	`ShaderSources`).
		///
		extern std::unordered_map<std::string, ShaderSources> Files;
	}
}
