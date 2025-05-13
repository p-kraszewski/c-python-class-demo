/*
 * Na podstawie
 *  * https://docs.python.org/3/extending/extending.html
 *  * https://docs.python.org/3/extending/newtypes_tutorial.html
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>
#include <stdio.h>

static PyTypeObject DemoClass;

/* **********************************************************************
 * DEFINICJA własnej klasy
 */

typedef struct {
    PyObject_HEAD /* Zawsze na początku */

        /*
         * Tutaj się zaczynają dane, które chcemy, żeby C zarządzało w tej
         * klasie. Jest tu wskaźnik, więc potrzebujemy dodać metodę do
         * deallokacji danych
         */

        char *myData;   /* Dane prywatne trzymane w obiekcie */
    size_t    myLength; /* Długość danych prywatnych */
} tm1_DemoClass;

/*
 * Dekstruktor klasy
 */
static void DemoClass_dealloc(tm1_DemoClass *self)
{
    printf("DBG DemoClass_dealloc()\n");

    /*
     * Zwolnienie danych zarządzanych przez C
     */
    if (self->myData != nullptr) {
        printf("DBG freeing user data\n");

        free(self->myData);
        self->myData   = nullptr;
        self->myLength = 0;
    }

    /*
     * Zwolnienie samego siebie x poziomu Pythona - **zawsze** musi być przy
     * własnym deallokatorze jako ostatnia operacja.
     */
    printf("DBG freeing self\n");
    Py_TYPE(self)->tp_free((PyObject *)self);
}

/*
 * Allokator nowej instancji klasy
 */
static PyObject *DemoClass_new(PyTypeObject *type, PyObject *args,
                               PyObject *kwds)
{
    printf("DBG DemoClass_new()\n");

    /* Wskaźnik na nową instancję */
    tm1_DemoClass *self;

    /* Allokujemy nową instancję klasy */
    self = (tm1_DemoClass *)type->tp_alloc(type, 0);

    if (self != NULL) {
        /* Allokacja się powiodła - ustalić stan obiektu */
        self->myData   = nullptr;
        self->myLength = 0;
    }
    return (PyObject *)self;
}

/*
 * Inicjalizator instancji (tutaj wypełnia się dane np. parametrami z
 * konstruktora)
 */
static int DemoClass_init(tm1_DemoClass *self, PyObject *args, PyObject *kwds)
{
    printf("DBG DemoClass_init()\n");

    char      *data     = nullptr;
    Py_ssize_t data_len = 0;

    /* Możliwość podania parametrów po nazwie */
    static char *kwlist[] = {"data", nullptr};

    /* Parsowanie parametrów konstruktora. Pipe `|` oznacza, że wszystkie
     * następujące po nim parametry są opcjonalne - tutaj wszystkie są
     * opcjonalne */
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|y#", kwlist, &data,
                                     &data_len)) {
        /* Błąd parsowania, np zły typ */
        printf("DBG failed arg parsing\n");
        return -1;
    }

    /*
     * Jeżeli użytkownik podał parametr, skopiuj dane do naszej klasy
     */
    if (data) {
        printf("DBG arg given, %ld bytes\n", data_len);
        /* Allokacja bufora */
        self->myData = malloc(data_len);
        if (self->myData == nullptr) {
            printf("DBG alloc failed\n");

            /* Zwróć błąd */
            return -1;
        }

        /* Skopiuj długość */
        self->myLength = data_len;

        for (int i = 0; i < data_len; i++) {
            /*
             * Kopiowanie odwróconych danych do bufora
             */
            self->myData[(data_len - 1) - i] = data[i];
        }
    }

    return 0;
}

/* Definicja składowych klasy widocznych z Pythona. Eksponujemy tu tylko
 * długość i to tylko do odczytu */
static PyMemberDef DemoClass_members[] = {
    {"len", Py_T_ULONGLONG, offsetof(tm1_DemoClass, myLength), Py_READONLY,
     "data length"},
    {nullptr} /* Koniec składowych */
};

/*
 * Definiujemy metodę w C dla klasy. Ta jest bezparametrowa.
 */
static PyObject *DemoClass_data(tm1_DemoClass *self,
                                PyObject      *Py_UNUSED(ignored))
{
    printf("DBG DemoClass_data()\n");

    if (self->myData == nullptr) {
        /* Jak nie ma danych, zwróć `None` */
        return Py_None;
    }

    /*
     * Zbudowanie nowego obiektu `bytes-like` z zawartością klasy
     */
    return Py_BuildValue("y#", self->myData, self->myLength);
}

/*
 * Definiujemy metodę w C dla klasy. Ta jest bezparametrowa.
 */
static PyObject *DemoClass_clone(tm1_DemoClass *self,
                                 PyObject      *Py_UNUSED(ignored))
{
    printf("DBG DemoClass_clone()\n");

    if (self->myLength > 0) {
        /*
         * Skonstruuj nowy obiekt z danymi bieżącego obiektu. Nawiasy wokół
         * `y#`, bo konstruktor przyjmuje Tuple parametrów, nie "goły" parametr.
         */
        PyObject *arglist = Py_BuildValue("(y#)", self->myData, self->myLength);
        return PyObject_CallObject((PyObject *)&DemoClass, arglist);
    }

    /*
     * Jak nie ma danych, wywołąj konstruktor też bez parametru
     */
    return PyObject_CallObject((PyObject *)&DemoClass, nullptr);
}

/*
 * Tablica metod naszej klasy
 */
static PyMethodDef DemoClass_methods[] = {
    {"data", (PyCFunction)DemoClass_data, METH_NOARGS, "Return data"},
    {"clone", (PyCFunction)DemoClass_clone, METH_NOARGS, "Clone class"},
    {nullptr} /* Koniec */
};

static PyTypeObject DemoClass = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0).tp_name = "tm1_demo.DemoClass",
    .tp_doc       = PyDoc_STR("DemoClass objects"),
    .tp_basicsize = sizeof(tm1_DemoClass),
    .tp_itemsize  = 0,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new       = DemoClass_new,
    .tp_init      = (initproc)DemoClass_init,
    .tp_dealloc   = (destructor)DemoClass_dealloc,
    .tp_members   = DemoClass_members,
    .tp_methods   = DemoClass_methods,
};

/* **********************************************************************
 * DEFINICJA zwykłych funkcji
 */

/*
 * Definicja funkcji C widocznej z Pythona. Zawsze ma takie same parametry i
 * zwracaną wartość.
 *
 * Ta funkcja odwraca binarny obiekt
 */
static PyObject *tm1demo_func(PyObject *self, PyObject *args)
{
    Py_buffer data;

    printf("DBG: tm1demo_func()\n");

    /*
     * Argumenty funkcji z Pythona przekazywane są jako krotka (Tuple), ta
     * funkcja rozpakowuje. Znacznik `y*` jest specjalnie przeznaczony dla
     * obiektów binarnych (`bytes-like object`).
     *
     * Obiekt `data` trzeba zwolnić na końcu funkcji
     */
    if (!PyArg_ParseTuple(args, "y*", &data)) {
        return nullptr;
    }

    /*
     * Zmienne tymczasowe
     */
    Py_ssize_t  len     = data.len;
    char const *dataPtr = data.buf;

    /*
     * Zrobienie bufora na wynik
     */
    char *rev_data = malloc(len);
    if (rev_data == nullptr) {
        /*
         * Brakło pamięci — zwolnij `data` i zwróć błąd
         */
        PyBuffer_Release(&data);
        return nullptr;
    }

    for (int i = 0; i < len; i++) {
        /*
         * Kopiowanie odwróconych danych do bufora
         */
        rev_data[(len - 1) - i] = dataPtr[i];
    }

    /*
     * Zwolnienie niepotrzebnego data
     */
    dataPtr = nullptr;
    PyBuffer_Release(&data);

    /*
     * Zbudowanie nowego obiektu `bytes-like` z odwróconych danych i długości
     */
    PyObject *result = Py_BuildValue("y#", rev_data, len);

    /*
     * Zwolnienie bufora tymczasowego - `Py_BuildValue` kopiuje zawartość
     */
    free(rev_data);

    /*
     * Zwrócenie skonstruowanej zawartości
     */
    return result;
}

/*
 * Lista metod zdefiniowanych w module
 */
static PyMethodDef TM1DemoMethods[] = {
    /*
     * Pola w takiej kolejności
     * { "nazwa funkcji dla Pythona", funkcja_w_c, flagi, "Dokumentacja funkcji"
     * },
     */
    {"demo_func", tm1demo_func, METH_VARARGS, "Demo function."},
    /*
     * Znacznik końca tablicy
     */
    {nullptr, nullptr, 0, nullptr} /* Sentinel */
};

/*
 * Definicja modułu dla Pythona
 */
static struct PyModuleDef tm1DemoModule = {
    .m_base    = PyModuleDef_HEAD_INIT, /* Zawsze ta stała */
    .m_name    = "tm1_demo",            /* Nazwa modułu dla Pythona */
    .m_size    = -1,                    /* Automatyczne zarządzanie pamięcią */
    .m_methods = TM1DemoMethods         /* Tablica zdefiniowanych metod */
};

/*
 * Jedyna eksportowana funkcja (nie `static`).
 *
 * **MUSI** nazywać się `PyInit_NAZWAMODUŁU`.
 *
 * Ta sama `NAZWAMODUŁU` musi być użyta:
 *   * w nazwie tej funkcji
 *   * w `.m_name` powyżej,
 *   * w pliku `setup.py`
 *   * w pliku `pyproject.toml`
 */
PyMODINIT_FUNC PyInit_tm1_demo(void)
{
    printf("DBG PyInit_tm1_demo()\n");
    PyObject *m;

    /* Aktywacja klasy */
    if (PyType_Ready(&DemoClass) < 0) return nullptr;

    /*
     * Stworzenie instancji modułu
     */
    m = PyModule_Create(&tm1DemoModule);
    if (m == nullptr) {
        return nullptr;
    }

    /*
     * Dopięcie klasy do modułu
     */
    if (PyModule_AddObjectRef(m, "DemoClass", (PyObject *)&DemoClass) < 0) {
        Py_DECREF(m);
        return nullptr;
    }

    /*
     * Podpięcie modułu pod interpreter
     */
    return m;
}
