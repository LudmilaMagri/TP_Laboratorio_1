#ifndef CONTROLLER_H_
#define CONTROLLER_H_


int controller_loadFromText(char* path , LinkedList* pArrayListEmployee, int* pIdMax);
int controller_loadFromBinary(char* path , LinkedList* pArrayListEmployee, int* pIdMax);
int controller_addEmployee(LinkedList* pArrayListEmployee);
int controller_editEmployee(LinkedList* pArrayListEmployee);
int controller_removeEmployee(LinkedList* pArrayListEmployee);
int controller_ListEmployee(LinkedList* pArrayListEmployee);
int controller_sortEmployee(LinkedList* pArrayListEmployee);
int controller_saveAsText(char* path , LinkedList* pArrayListEmployee);
int controller_saveAsBinary(char* path , LinkedList* pArrayListEmployee);
int controller_cloneList (LinkedList* pArrayListEmployee, char* pFile);
int controller_isEmptyList(LinkedList* pArrayListEmployee);


#endif /* CONTROLLER_H_ */
