project('wiki-structs', 'cpp')

deps = [
    dependency('catch2')
]

src = [
    'test/main.cpp',
    'test/test_binary_heap.cpp',
    'test/test_fibonacci_heap.cpp'
]

inc = include_directories('src')

e = executable('structs', src, dependencies: deps, include_directories: inc)

test('Test Structs', e)

