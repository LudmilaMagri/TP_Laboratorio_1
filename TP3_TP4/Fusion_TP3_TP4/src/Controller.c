#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "Employee.h"
#include "parser.h"
#include "utn.h"

/** \brief Carga los datos de los empleados desde el archivo data.csv (modo texto).
 *
 * \param path char*
 * \param pArrayListEmployee LinkedList*
 * \return int
 *
 */
LinkedList* ll_filtrar(LinkedList* pArrayListEmployee, int(*funcionCriterio)(void*))
{
	LinkedList* listaFiltrada = NULL;
	int len;
	void* pE;
	int i;
	if(pArrayListEmployee!=  NULL)
	{
		listaFiltrada = ll_newLinkedList(); //solo con mi opcion
		len= ll_len(pArrayListEmployee);
		for(i=0; i<len; i++)
		{
			pE = (Employee*)ll_get(pArrayListEmployee, i);
			if (funcionCriterio(pE)==1)
			{
				ll_add(listaFiltrada, pE);
			}
		}
	}
return listaFiltrada;
}

int aplicarAumento (LinkedList* listaFiltrada)
{
	int retorno = -1;
	Employee* pE;
	int flag;
	int i;
	int lenLista;
	float sueldoAux;

	lenLista = ll_len(listaFiltrada);
	for(i=0; i<lenLista; i++)
	{
		pE = (Employee*)ll_get(listaFiltrada, i); //empleados solo de la lista filtrada
		employee_getSueldo(pE, &flag);
		sueldoAux = sueldoAux*(1.1);
		//sueldoAux = sueldoAux + (sueldoAux*0.1);
		employee_setSueldo(pE, sueldoAux);
		retorno = 0;
	}
	return retorno;
}
static int funcionCriterioFiltrarHoras(void* pGenerico)
{
	Employee* pE;
	pE = (Employee*) pGenerico;
	int horasTrabajadas;
	int flag;

	horasTrabajadas= employee_getHorasTrabajadas(pE, &flag);
	if(horasTrabajadas>100)
	{
		return 1;
	}
	return 0;
}
int controller_aplicarAumento (LinkedList* pArrayListEmployee)
{
	//aumentar sueldo un 10% a los que trabajan mas de 100 horas
	int retorno=-1;
	LinkedList* listaFiltrada;

	if(pArrayListEmployee!=  NULL)
	{
		listaFiltrada=ll_filtrar(pArrayListEmployee, funcionCriterioFiltrarHoras);
		aplicarAumento(listaFiltrada);
		ll_deleteLinkedList(listaFiltrada);
	}

	return retorno;
}
int controller_loadFromText(char* path , LinkedList* pArrayListEmployee, int* pIdMax)
{

	int retorno = -1;
	FILE* fpArchivo;
	if(path != NULL &&  pArrayListEmployee!=NULL)
	{
		fpArchivo = fopen(path, "r");
		if(fpArchivo!=NULL)
		{
			*pIdMax = parser_EmployeeFromText(fpArchivo, pArrayListEmployee);
			employee_valorInicialId(*pIdMax);
			retorno = 0;
			printf("\nDatos cargados correctamente en modo texto\n");
		}
		fclose(fpArchivo);
	}

    return retorno;
}

/** \brief Carga los datos de los empleados desde el archivo data.csv (modo binario).
 *
 * \param path char*
 * \param pArrayListEmployee LinkedList*
 * \return int
 *
 */
int controller_loadFromBinary(char* path , LinkedList* pArrayListEmployee, int* pIdMax)
{
	int retorno= -1;
	FILE* pFile=NULL;
	if(path != NULL && pArrayListEmployee!=NULL)
	{
		pFile = fopen(path,"rb");
		if(pFile!=NULL)
		{
			*pIdMax = parser_EmployeeFromBinary(pFile, pArrayListEmployee);
			employee_valorInicialId(*pIdMax);
			printf("\nDatos cargados correctamente en modo binario\n");
			retorno = 0;
		}
		else
		{
			printf("\nError, no se ha podido cargar los datos\n");
		}
		fclose(pFile);
	}
    return retorno;
}

/** \brief Alta de empleados
 *
 * \param path char*
 * \param pArrayListEmployee LinkedList*
 * \return int
 *
 */
int controller_addEmployee(LinkedList* pArrayListEmployee)
{
	int retorno = -1;
	Employee* pE;
	int newId;
	char nombre [5000];
	float sueldo;
	int horas;

	if(pArrayListEmployee!= NULL)
	{
		pE = employee_new();
		if(utn_getNombre(nombre, 5000, "\nIngrese un nombre\n", "\nError\n", 3)==0 &&
			utn_getNumeroFlotante(&sueldo, "\nIngrese el sueldo\n", "\nError\n", 0, 1000000, 3)==0 &&
			utn_getNumeroInt(&horas, "\nIngrese las horas trabajadas\n", "\nError\n", 0, 50000, 3)==0 &&
			pE != NULL)
		{
			newId=employee_generarId();
			printf("\nID NUEVO: %d\n", newId);
			if (employee_setId(pE, newId)==0 && employee_setNombre(pE, nombre)==0 &&
				employee_setHorasTrabajadas(pE, horas)==0 && employee_setSueldo(pE, sueldo)==0)
			if(ll_add(pArrayListEmployee, pE)==0)
			{
				printf("\nEmpleado agregado exitosamente\n");
				retorno = 0;
			}
		}
	}
    return retorno;
}

/** \brief Modificar datos de empleado
 *
 * \param path char*
 * \param pArrayListEmployee LinkedList*
 * \return int
 *
 */

static Employee* buscarPorId (LinkedList* pArrayListEmployee, int idABuscar)
{
	int idEncontrado;
	int flag;
	int i;
	Employee* pEmpEdit;
	Employee* pRet=NULL;

	if(pArrayListEmployee!=  NULL)
	{
		int lenLista = ll_len(pArrayListEmployee);
		for(i=0; i<lenLista; i++)
		{
			pEmpEdit = (Employee*)ll_get(pArrayListEmployee, i);
			idEncontrado = employee_getId(pEmpEdit, &flag);
			if(idABuscar== idEncontrado)
			{
				pRet = pEmpEdit;
				break;
			}
		}
	}
	return pRet;
}
int controller_editEmployee(LinkedList* pArrayListEmployee)
{
	int retorno = -1;
	int idPedido;
	Employee* pEmpEdit;
	char nombre [5000];
	float sueldo;
	int horas;
	if(pArrayListEmployee!=  NULL)
	{
		if(utn_getNumeroInt(&idPedido, "\nIngrese el id que desea modificar\n", "\nError\n", 0, 99999999, 3)==0)
		{
			pEmpEdit=buscarPorId(pArrayListEmployee, idPedido);
			if(pEmpEdit!= NULL)
			{
				if(utn_getNombre(nombre, 5000, "\nIngrese un nombre\n", "\nError\n", 3)==0 &&
					utn_getNumeroFlotante(&sueldo, "\nIngrese el sueldo\n", "\nError\n", 0, 1000000, 3)==0 &&
					utn_getNumeroInt(&horas, "\nIngrese las horas trabajadas\n", "\nError\n", 0, 50000, 3)==0)
				{
					if(employee_setNombre(pEmpEdit, nombre)==0 && employee_setSueldo(pEmpEdit, sueldo)== 0 &&
						employee_setHorasTrabajadas(pEmpEdit, horas)==0)
					{
						printf("\nEmpleado modificado exitosamente\n");
						retorno = 0;
					}
				}
			}
			else
			{
				printf("\nError, id no encontrado\n");
			}
		}
	}
    return retorno;
}

/** \brief Baja de empleado
 *
 * \param path char*
 * \param pArrayListEmployee LinkedList*
 * \return int
 *
 */
int controller_removeEmployee(LinkedList* pArrayListEmployee)
{
	int retorno = -1;
	int idPedido;
	Employee* pEmpDelete;
	int indexDelete;

	if(pArrayListEmployee!=  NULL)
	{
		if(utn_getNumeroInt(&idPedido, "\nIngrese el id que desea eliminar\n", "\nError\n", 0, 99999999, 3)==0)
		{
			pEmpDelete=buscarPorId(pArrayListEmployee, idPedido);
			if(pEmpDelete!= NULL)
			{
				indexDelete = ll_indexOf(pArrayListEmployee, pEmpDelete);
				employee_delete2(pEmpDelete);
				if (ll_remove(pArrayListEmployee, indexDelete)==0)
				{
					printf("\nEmpleado eliminado exitosamente\n");
					retorno =0;
				}
			}
			else
				printf("\nNo se encontro el id ingresado\n");
		}
	}
    return retorno;
}


/** \brief Listar empleados
 *
 * \param path char*
 * \param pArrayListEmployee LinkedList*
 * \return int
 *
 */
int controller_ListEmployee(LinkedList* pArrayListEmployee)
{
	int retorno =-1;
	int i;
	Employee* pE;
	char id[5000];
	int flag;
	char* pNombre;
	float sueldoAux;
	char horasAux[5000];

	if (pArrayListEmployee!= NULL)
	{
		for(i=0; i<ll_len(pArrayListEmployee); i++)
		{
			pE= (Employee*)ll_get(pArrayListEmployee, i);
			if (pE!=NULL)
			{
				employee_getIdTxt(pE, id);
				pNombre = employee_getNombre(pE, &flag);
				sueldoAux = employee_getSueldo(pE, &flag);
				employee_getHorasTrabajadasTxt(pE, horasAux);
				printf("\n ID: %s - NOMBRE: %s - SUELDO: $%.2f - HORAS: %s\n", id, pNombre, sueldoAux,horasAux );
				retorno = 0;
			}
		}
	}
    return retorno;
}

/** \brief Ordenar empleados
 *
 * \param path char*
 * \param pArrayListEmployee LinkedList*
 * \return int
 *
 */
int controller_sortEmployee(LinkedList* pArrayListEmployee)
{
	int retorno = -1;
	int opcion;
	if(pArrayListEmployee!= NULL)
	{
		do
		{
			if(!utn_getNumeroInt(&opcion,
					"\n\t---------------------------------------------------------------"
					"\n\t\t***** MENU DE ORDENAMIENTO*****							\n"
					"\n\t*	1. Ordenar por nombre.\t\t\t*"
					"\n\t*	2. Ordenar por ID.\t\t\t*"
					"\n\t*	3. Ordenar por sueldo.\t\t\t*"
					"\n\t*	4. Ordenar por horas trabajadas.\t*"
					"\n\t*	5. Salir.\t\t\t\t*"
					"\n\t---------------------------------------------------------------\n\n",
							"Opcion incorrecta", 0, 6, 3))
			{
				switch(opcion)
				{
					case 1:
						if(ll_sort(pArrayListEmployee, employee_funcionCriterioNombre, 1)==0)
							printf("\nDatos ordenados exitosamente por nombre\n");
						break;
					case 2:
						if(ll_sort(pArrayListEmployee, employee_funcionCriterioId, 1)==0)
							printf("\nDatos ordenados exitosamente por ID\n");
						break;
					case 3:
						if(ll_sort(pArrayListEmployee, employee_funcionCriterioSueldo, 1)==0)
							printf("\nDatos ordenados exitosamente por sueldo\n");
						break;
					case 4:
						if(ll_sort(pArrayListEmployee, employee_funcionCriterioHoras, 1)==0)
							printf("\nDatos ordenados exitosamente por horas trabajadas\n");
						break;
				}

			}

		}while(opcion !=5);
			retorno = 0;

	}
//	ll_sort(pArrayListEmployee, funcionCriterio(a, b), order)
    return retorno;
}

/** \brief Guarda los datos de los empleados en el archivo data.csv (modo texto).
 *
 * \param path char*
 * \param pArrayListEmployee LinkedList*
 * \return int
 *
 */

int controller_saveAsText(char* path , LinkedList* pArrayListEmployee)
{
	int retorno = -1;
	int i;
	FILE* pFile;
	Employee* pE;
	int flag, idAux, horasAux;
	float sueldoAux;
	char* nombreAux;

	if (path!= NULL && pArrayListEmployee != NULL)
	{
		pFile = fopen(path, "w");
		if(pFile != NULL)
		{
			retorno = 0;
			for(i=0; i<ll_len(pArrayListEmployee); i++)
			{
				pE =(Employee*)ll_get(pArrayListEmployee, i);
				if(pE!=NULL)
				{
						idAux=employee_getId(pE, &flag);
						horasAux=employee_getHorasTrabajadas(pE, &flag);
						nombreAux= employee_getNombre(pE, &flag);
						sueldoAux = employee_getSueldo(pE, &flag);
						fprintf(pFile, "%d, %s, %d, %2.f\n", idAux, nombreAux, horasAux, sueldoAux);
				}
			}
			printf("\nGuardado exitosamente en modo texto\n");
			fclose(pFile);
		}
	}
    return retorno;
}


/** \brief Guarda los datos de los empleados en el archivo data.csv (modo binario).
 *
 * \param path char*
 * \param pArrayListEmployee LinkedList*
 * \return int
 *
 */

int controller_saveAsBinary(char* path , LinkedList* pArrayListEmployee)
{
	FILE* pFile;
	Employee* pE;
	int i;
	int retorno =-1;

	pFile = fopen(path,"wb");
	if(pFile!=NULL)
	{
		for(i=0; i<ll_len(pArrayListEmployee); i++)
		{
			pE = ll_get(pArrayListEmployee, i);
			if(pE!=NULL)
			{
				fwrite(pE,sizeof(Employee),1,pFile);
				retorno =0;
			}
		}
		fclose(pFile);
		printf("\nDatos guardados exitosamente en modo binario\n");
	}
    return retorno;
}
/** \brief Crea una copia de la lista comparando si estan todos los valores
 *
 * \param pList LinkedList* Puntero a la lista
 * \return LinkedList* Retorna  (-1) Error
                                (0) Si ok
*/

int controller_cloneList (LinkedList* pArrayListEmployee, char* pFile)
{
	int retorno = -1;
	int respuesta;
	LinkedList* pL;

	if (pArrayListEmployee!=NULL)
	{
		if (utn_getNumeroInt(&respuesta, "\nEsta seguro de hacer una copia de la lista?\nSI:1\nNO:0", "\nError\n", 0, 3, 3)==0)
		{
			if(respuesta==1)
			{
				pL =ll_clone(pArrayListEmployee);
				if (pL!=NULL)
				{
					if (ll_containsAll(pL, pArrayListEmployee)==1)
					{
						controller_ListEmployee(pL);
						printf("\nLista creada exitosamente\n");
						controller_saveAsText(pFile, pL);
						retorno = 0;
					}
					else
					{
						printf("\nError, no se pudo guardar la lista");
						ll_deleteLinkedList(pL);
					}
				}
			}
		}
	}
	return retorno;
}
/** \brief Verifica si la lista esta vacia o no
 *
 * \param pList LinkedList* Puntero a la lista
 * \return LinkedList* Retorna  (-1) Error
                                (0) Si ok
*/

int controller_isEmptyList(LinkedList* pArrayListEmployee)
{
	int retorno = -1;
	int respuesta;
	if (pArrayListEmployee!=NULL)
	{
		retorno = 0;
		if (ll_isEmpty(pArrayListEmployee)==1)
		{
			printf("\nLa lista de empleados esta vacia\n");
		}
		else if (ll_isEmpty(pArrayListEmployee)==0)
		{
			printf("\nLa lista de empleados se encuentra cargada\n\n");
			if (utn_getNumeroInt(&respuesta, "\nDesea ver el contenido de la lista?\nSI:1\nNO:0\n", "\nError\n", 0, 3, 3)==0)
			{
				if(respuesta==1)
				{
					controller_ListEmployee(pArrayListEmployee);
				}
			}
		}
	}
	return retorno;
}
