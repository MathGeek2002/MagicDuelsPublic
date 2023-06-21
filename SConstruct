#!/usr/bin/env python

import platform
import configparser
import os

config = configparser.ConfigParser()
config.read('scons.ini')

system = platform.system()
print('Using SCons version for ' + system)

my_libs = ['GL', 'GLU', 'GLEW', 'SDL2', 'freeimage', 'assimp', 'dl', 'SDL2_mixer', 'freetype','gomp']
my_libpath = []
my_incpath = [config['paths']['freetype']]
my_ccflags = ['-Wall', '-std=c++17', '-O3', '-fopenmp']

env = Environment(
    CCFLAGS=my_ccflags
)

if system == 'Windows':
    my_libs = ['mingw32', 'opengl32', 'glu32', 'glew32', 'SDL2', 'freeimage', 'SDL2_mixer', 'freetype','gomp']
    my_tools = ['mingw']
    env = Environment(
        CCFLAGS=my_ccflags, tools=my_tools
    )
    os.system("windres resources.rc resources.o")

if system == 'Windows':
    env.core_sources = ["resources.o"]
else:
    env.core_sources = []

Export('env', 'config')

SConscript(['core/SCons', 'thirdparty/SCons'])

env.Program(
    target='Magic Duels',
    source=env.core_sources + env.Object(
        ['main.cpp'],
        CPPPATH=my_incpath
    ),
    LIBS=my_libs,
    LIBPATH=my_libpath
)
