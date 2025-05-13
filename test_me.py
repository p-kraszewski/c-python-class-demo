#!/usr/bin/python3

# Import naszej biblioteki binarnej
import tm1_demo

# Wywołanie funkcji
ans = tm1_demo.demo_func(b"Test")

# Powinno wyświetlić b'tseT'
print("Rev data=", ans)

k = tm1_demo.DemoClass(data=b'Other test')

# Składowa len
print("K len=", k.len)

# Pobranie danych
print("K data=", k.data())

# Konstruktor bez parametru
l = tm1_demo.DemoClass()

# Składowa len
print("L len=", l.len)

# Pobranie danych
print("L data=", l.data())

# Kopia danych - konstruktor znowu obraca łańcuch
m = k.clone()

# Składowa len
print("M len=", m.len)

# Pobranie danych
print("M data=", m.data())
