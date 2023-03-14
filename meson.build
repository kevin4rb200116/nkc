project('nkc', 'cpp',
  version : '0.1',
  default_options : ['cpp_std=c++17'])

incdir = include_directories('src/include')

ntokenize = subproject('ntokenize')
ntokenize_dep = ntokenize.get_variable('ntokenize_dep')

lib = static_library('tokenize',
  'src/lex.cc',
  'src/tokenize.cc',
  dependencies: ntokenize_dep,
  include_directories: incdir,
  install: false)

exe = executable('nkc',
  'src/main.cc',
  link_with: lib,
  dependencies: ntokenize_dep,
  include_directories: incdir,
  install : true)
