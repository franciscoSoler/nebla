/*
 * StringUtils.h
 *
 *  Created on: 01/05/2012
 *      Author: alex
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <sstream>
#include <iostream>
#include <vector>

/**
 * Funciones auxiliares para strings
 */
class StringUtils
{
public:

	/**
	 * Parte un string delimitado por caracteres, en un vector de
	 * strings
	 * @param serialized el string serializado
	 * @param delim delimitador a utilizar
	 * @return vector<string> Vector con los items separados
	 */
	static std::vector<std::string>splitString(std::string serialized,
											char delim);

	/**
	 * Convierte un elemento de un tipo a string.
	 * Los tipos de datos que se pueden convertir estan dados por
	 * stringstream
	 */
	template <typename T>
	static std::string NumberToString ( T Number )
	{
		std::stringstream ss;
		ss << Number;
		return ss.str();
	}

	/**
	 * Convierte un string a un elemento de un tipo dado.
	 * Los tipos de datos que se pueden convertir estan dados por
	 * stringstream
	 */
	template <typename T>
	static T StringToNumber ( const std::string &Text )
	{
		std::stringstream ss(Text);
		T result;
		return ss >> result ? result : 0;
	}
};

#endif /* STRINGUTILS_H_ */
