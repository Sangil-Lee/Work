#include <Python.h>

void PrintMyDef()
{
	PyObject* mydef = PyImport_ImportModule("scripts.my_def");

	if(mydef) {
		PyObject* printHello = PyObject_GetAttrString(mydef, "PrintMyDef");

		if(printHello) {
			PyObject *r = PyObject_CallFunction(printHello, NULL);

			if (r == Py_None) {
				printf("None is returned.\n");
				Py_XDECREF(r);
			}

			Py_XDECREF(printHello);
		}
		Py_XDECREF(mydef);
	}
}

int Multiply(int x, int y)
{
	PyObject* mydef = PyImport_ImportModule("scripts.my_def");
	int result;

	if(mydef) {
		PyObject* multiply = PyObject_GetAttrString(mydef, "Multiply");

		if(multiply) {
			PyObject *r = PyObject_CallFunction(multiply, "ii", x, y);

			if(r) {
				result = (int)PyInt_AS_LONG(r);
				Py_XDECREF(r);
			}
			Py_XDECREF(multiply);
		}
		Py_XDECREF(mydef);
	}

	return result;
}

void SysPath()
{
	printf("sys.path:");
	PyRun_SimpleString("import sys\nprint sys.path\nprint\n");
}

int main(int argc, char** argv)
{
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	printf("GetProgramName: %s\n\n", Py_GetProgramName());

	if (Py_IsInitialized()) {
		PySys_SetArgv(argc, argv); //error not declared function
		printf("GetPath: %s\n\n", Py_GetPath());
		SysPath();
		PrintMyDef(); // Calling python functions
		printf("%d * %d = %d\n", 10, 15, Multiply(10, 15));
		Py_Finalize();
	}
	return 0;
}

//sudo gcc -o main2 main2.cpp `python-config --includes` `python-config --libs`
