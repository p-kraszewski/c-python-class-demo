from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="tm1_demo",
            sources=["tm1_demo.c"],
        ),
    ]
)
