# Przykładowy moduł Pythona napisany w C

## Kompilacja

```sh
python setup.py build_ext
```

W katalogu `build/lib...` pojawia się plik `tm_demo....so`. Skopiować go do katalogu głównego projektu.

## Test

```sh
python test_me.py
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
