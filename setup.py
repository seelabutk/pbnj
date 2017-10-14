#!/usr/bin/env python
"""

"""

from distutils.core import setup, Extension
from os import environ


# Needed so that the extension can always find the modules, even if they aren't
# on your normal load path. e.g. /opt/embree/lib usually isn't in your
# ldconfig, so this hard codes /opt/embree/lib into your extension, so that it
# knows where to look for the libraries.
def patch_ld_run_path():
    if 'LD_RUN_PATH' in environ is not None:
        ld_run_path += environ['LD_RUN_PATH'] + ':'
    else:
        ld_run_path = ''

    ld_run_path += ':'.join([
        '/opt/embree/lib',
        '/usr/local/lib',
        '/usr/local/lib64',
    ])

    environ['LD_RUN_PATH'] = ld_run_path


patch_ld_run_path()

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
    python_requires='>=3.5',
    entry_points={
    },
    ext_modules=[
        _pbnj,
    ],
)
