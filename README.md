# Przykładowy moduł Pythona napisany w C

## Kompilacja

```sh
python setup.py build_ext
```

W katalogu `build/lib...` pojawia się plik `tm_demo....so`. Skopiować go do katalogu głównego projektu.

## Alternatywna kompilacja dla Ubuntu

* Zainstalować `cmake`, `build-essential`, `python3-dev`
* W katalogu repozytorium założyc katalog `build`
* Wejść do katalogu `build`
* Wydać polecenie `cmake ..`
* Wydać polecenie `make`
* Wydać polecenie `cp *.so ..`

```sh
user@user-virtualbox:~/Pobrane/c-python-class-demo$

user@user-virtualbox:~/Pobrane/c-python-class-demo$ mkdir build

user@user-virtualbox:~/Pobrane/c-python-class-demo$ cd build

user@user-virtualbox:~/Pobrane/c-python-class-demo/build$ cmake ..

-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found Python: /usr/bin/python3 (found version "3.12.3") found components: Interpreter Development Development.Module Development.Embed
-- Configuring done (0.4s)
-- Generating done (0.0s)
-- Build files have been written to: /home/user/Pobrane/c-python-class-demo/build

user@user-virtualbox:~/Pobrane/c-python-class-demo/build$ make

[ 50%] Building C object CMakeFiles/tm1_demo.dir/tm1_demo.c.o
[100%] Linking C shared library tm1_demo.so
[100%] Built target tm1_demo

user@user-virtualbox:~/Pobrane/c-python-class-demo/build$ cp *.so ..
```

## Test

```sh
python3 test_me.py
```

Powinien się uruchomić test z poniższym wynikiem:

```txt
DBG PyInit_tm1_demo()
DBG: tm1demo_func()
Rev data= b'tseT'
DBG DemoClass_new()
DBG DemoClass_init()
DBG arg given, 10 bytes
K len= 10
DBG DemoClass_data()
K data= b'tset rehtO'
DBG DemoClass_new()
DBG DemoClass_init()
L len= 0
DBG DemoClass_data()
L data= None
DBG DemoClass_clone()
DBG DemoClass_new()
DBG DemoClass_init()
DBG arg given, 10 bytes
M len= 10
DBG DemoClass_data()
M data= b'Other test'
DBG DemoClass_dealloc()
DBG freeing user data
DBG freeing self
DBG DemoClass_dealloc()
DBG freeing self
DBG DemoClass_dealloc()
DBG freeing user data
DBG freeing self
```

