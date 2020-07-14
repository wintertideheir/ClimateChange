env = Environment()
env.Append(CFLAGS='-std=c11')
env.VariantDir('bin', 'src', duplicate=0)

env.Program(target='bin/climatechange',
            source=['bin/climatechange.c', 'bin/geometry.c',
                    'bin/drawing.c', 'bin/shaders.c'],
            LIBS=['m', 'glfw', 'GL', 'GLEW'])
