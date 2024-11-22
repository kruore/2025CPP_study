#include "employee.h"
#include "DataBase.h"

int main()
{
	DataBase DB;

	employee* newData= new employee();
	DB.data.push_back(newData);
}
