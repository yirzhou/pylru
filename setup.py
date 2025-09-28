#!/usr/bin/env python3

import os

import pybind11
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import find_packages, setup

# Define the extension module
ext_modules = [
    Pybind11Extension(
        "pylru",
        [
            "src/pylru.cpp",
        ],
        include_dirs=[
            # Path to pybind11 headers
            pybind11.get_cmake_dir() + "/../../../include",
            # Path to our C++ headers
            "include",
        ],
        language='c++',
        cxx_std=17,
    ),
]

# Read the contents of README file
this_directory = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(this_directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(
    name="pylru",
    version="1.0.0",
    author="Your Name",
    author_email="your.email@example.com",
    description="A high-performance LRU cache implementation in C++ with Python bindings",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/yourusername/pylru",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
    install_requires=[
        "pybind11>=2.6.0",
    ],
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: C++",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Topic :: System :: Systems Administration",
    ],
)
