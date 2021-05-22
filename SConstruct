import sys

env = Environment()

env.Replace(CPPPATH=['.', 'include', 'libs/gtest-1.6.0', 'libs/gtest-1.6.0/include'])
env.Replace(CXXFLAGS=['-DGTEST_USE_OWN_TR1_TUPLE'])

if sys.platform == 'darwin':
    env.Replace(CXX='clang++')
    env.Append(CXXFLAGS=['-std=gnu++11', '-stdlib=libc++'])
    env.Append(LINKFLAGS=['-stdlib=libc++'])

env.VariantDir('build/gtest', 'libs/gtest-1.6.0/src', duplicate=0)
env.VariantDir('build/src', 'src', duplicate=1)
env.VariantDir('build/test', 'test', duplicate=0)

sourceFiles = list(
    set(env.Glob('build/gtest/gtest-all.cc')) |
    set(env.Glob('build/gtest/gtest_main.cc')) |
    set(env.Glob('build/src/*.cpp')) |
    set(env.Glob('build/test/*.cpp')))

tests = env.Program('bin/test_runner', sourceFiles)
