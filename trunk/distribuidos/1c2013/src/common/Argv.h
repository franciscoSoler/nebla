/*
 * Argv.h
 *
 *  Created on: 29/04/2012
 *      Author: alex
 */

#ifndef ARGV_H_
#define ARGV_H_

#include <iostream>
#include <vector>
#include "Object.h"

/**
 * Se encarga de manejar argumentos para pasar por linea de
 * comandos
 */
class Argv: public Object
{
typedef std::vector<std::string> ArgumentList;
public:
	typedef enum {PUSH_BACK=0,PUSH_FRONT} ElemPosition;
	/**
	 * Construct
	 */
	Argv();

	/**
	 * Destruct
	 */
	virtual ~Argv();

	/**
	 * Agrega un argumento
	 * @param arg nuevo argumento a agregar
	 */
	void pushArg(std::string arg, ElemPosition pos =PUSH_BACK);

	/**
	 * Agrega un argumento
	 * @param arg nuevo argumento a agregar
	 */
	void pushArg(int arg, ElemPosition pos =PUSH_BACK);

	/**
	 * Permite ingresar todos los parametros en un string,
	 * en el cual los parametros estan separados por espacios.
	 * Reemplaza los parametros ya existentes.
	 *  @param args string con todos los parametros a usar.
	 */
	void setSerializedArgs(std::string args);

	/**
	 * Vacia los elementos ya existentes.
	 */
	void clear();

	/**
	 * Obtiene el numero de argumentos contenidos
	 * @returns int numero de argumentos
	 */
	int getArgc();

	/**
	 * Toma los argumentos desde argv, y los inserta
	 * en el vector interno del objeto
	 * @param argv vector de argumentos, con el ultimo elemento
	 * 			   NULL
	 */
	void setArgv(char*const* argv);

	/**
	 * Devuelve los argumentos en un formato mediante
	 * el cual se los puedo pasar al metodo execv()
	 */
	char*const* getArgv();

	/**
	 * Serializa todos los argumentos a un string, separados
	 * por espacios
	 * @returns string String con los argumentos serializados
	 */
	std::string serializeArgs();

	/**
	 * Obtiene un string con el n-esimo arg
	 * @param n numero de arg (arranca en 0)
	 * @returns string contenido del argumento
	 * @throws exception si se le pasa un indice fuera de rango
	 */
	std::string getArg(unsigned int n);
private:
	ArgumentList m_Arguments;
	std::vector<const char*> m_Argv;
};

#endif /* ARGV_H_ */
