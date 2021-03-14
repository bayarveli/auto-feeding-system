import os
import subprocess
import scons.SCons.Tool.cc as cc
import scons.SCons.Builder as builder

env = Environment()

def _echospawn(sh, escape, cmd, args, env):
    return subprocess.call(args)

# / ile yazılan flagleri - ye dönüştürüyor.
cc.generate(env)

# avr-gcc is not recognized hatasını kaldırıyor.
env['SPAWN']  = _echospawn
env['ESCAPE'] = lambda x:x
env['CC'] = 'avr-gcc'
env['OBJCOPY'] = 'avr-objcopy'
env['AR'] = 'avr-ar'
env['CCFLAGS'] = []
env['LINKFLAGS'] = []
env['ARFLAGS'] = []
env['OBJSUFFIX'] = '.o'
env['PROGSUFFIX'] = '.elf'
env['LIBSUFFIX'] = ''
env['LIBDIRPREFIX'] = '-L'

# To use a library, use the -l switch. Linker expands it as libLibraryName.a
env['LIBLINKPREFIX'] = '-l'

#env['_LIBDIRFLAGS'] = '${_concat(LIBDIRPREFIX, LIBPATH, LIBDIRSUFFIX, __env__)}'
#env['_CPPINCFLAGS'] = '${_concat(INCPREFIX, CPPPATH, INCSUFFIX, __env__)}'

env['LINKCOM'] = "$CC $LINKFLAGS $_CCCOMCOM -o $TARGET $SOURCES $_LIBDIRFLAGS $_LIBFLAGS"
env['ARCOM'] = "$AR $ARFLAGS $TARGET $SOURCES"
env['CCCOM'] = "$CC -o $TARGET -c $CFLAGS $CCFLAGS $_CCCOMCOM $SOURCES"

env.Append(CCFLAGS = '-mmcu=atmega32u4')
env.Append(CCFLAGS = '-g')

env.Append(LINKFLAGS = '-Wl,-Map=${TARGET}.map')
env.Append(LINKFLAGS = '-Wl,--gc-sections')
env.Append(LINKFLAGS = '-mmcu=atmega32u4')

env.Append(ARFLAGS = 'rcs')

# print (env.Dump())

Export('env')