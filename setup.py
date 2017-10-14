#!/usr/bin/env python
"""

"""

from distutils.core import setup, Extension


_pbnj = Extension(
    '_pbnj',
    language='c++',
    libraries=[
        'pbnj',
    ],
    sources=[
        'pbnj_wrap.cxx',
    ],
)


setup(
    name='pbnj',
    version='0.0.1',
    description='PBNJ',
    author='Tanner Hobson',
    author_email='thobson2@vols.utk.edu',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
    ],
    packages=[
    ],
    python_requires='>=3.5',
    install_requires=[
    ],
    entry_points={
    },
    ext_modules=[
        _pbnj,
    ],
    py_modules=[
        'python/pbnj',
    ],
)
