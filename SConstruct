env = Environment(CPPPATH='/usr/include')
env.Append(CFLAGS='-std=c11')
env.VariantDir('bin', 'src', duplicate=0)

env.Program(target='bin/climatechange',
            source=['bin/climatechange.c', 'bin/geometry.c'], LIBS='m')
