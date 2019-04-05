#include <Python.h>
#include <stdio.h>

typedef unsigned char uchar;

int patchSize = 4;
uchar *targetPatch;


void myClass() {
	PyObject* mydef = PyImport_ImportModule("scripts.my_net");
	int result;

	if (mydef) 
	{
		PyObject* myClass = PyObject_GetAttrString(mydef, "PatchNetwork");

		if (myClass) 
		{
			/* instance = MyClass() */
			PyObject *instance = PyObject_CallObject(myClass, NULL);

			if (instance) {
				PyObject *returnSum = PyObject_CallMethod(instance, "Sum",
						NULL);
				if (returnSum) {
					double result = PyFloat_AS_DOUBLE(returnSum); //find using Search . Type this "PyFloat_"
					Py_XDECREF(returnSum);
					printf("sum = %f\n", result);
				} else
					printf("No Sum Method\n");

				// string(s) , integer(i) , float(f)
				// https://docs.python.org/2/c-api/arg.html#c.Py_BuildValue
				PyObject *returnAvr = PyObject_CallMethod(instance, "Avr", "i",
						4);
				if (returnAvr) {
					double result = PyFloat_AS_DOUBLE(returnAvr); //find using Search . Type this "PyFloat_"
					Py_XDECREF(returnAvr);
					printf("Avr = %f\n", result);
				} else
					printf("No Avr Method\n");

				printf("\n\n");

				targetPatch = (uchar*) malloc(patchSize * sizeof(uchar));
				for (int i = 0; i < patchSize; i++)
					targetPatch[i] = i + 10;

				PyObject *nParam = PyTuple_New(patchSize);
				for (long i = 0; i < patchSize; i++) {
					PyObject *item = PyInt_FromLong(targetPatch[i]);
					PyTuple_SetItem(nParam, i, item);
				}

				PyObject *returnPredict = PyObject_CallMethod(instance,
						"Predict", "(O)", nParam);
				if (returnPredict) {

					int* result = reinterpret_cast<int*>(returnPredict); //find using Search . Type this "PyFloat_"
					int m_numCols = PyObject_Length(returnPredict);
					printf("size %d\n", m_numCols);
					for (int j=0; j<m_numCols; j++) {
						PyObject *piItem = PySequence_GetItem(returnPredict, j);

						int isInt=PyInt_Check(piItem);
						int isIntExact=PyInt_CheckExact(piItem);
						printf("isInt %d isIntExact %d\n",isInt,isIntExact);

						double value = PyInt_AsSsize_t(piItem);
						printf("Predict [%d] %f \n", j, value);
					}
					Py_XDECREF(returnPredict);
				} else
					printf("No Predict Method\n");
			}
		}
	}
}

int main(int argc, char** argv) 
{
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	printf("GetProgramName: %s\n\n", Py_GetProgramName());

	if (Py_IsInitialized()) {
		PySys_SetArgv(argc, argv);
		printf("GetPath: %s\n\n", Py_GetPath());
		myClass();
		Py_Finalize();
	}
	return 0;
}

//sudo gcc -o ex4 ex_4.cpp -Wno-write-strings `python-config --includes` `python-config --libs`
