project('nkc', 'cpp',
  version : '0.1',
  default_options : ['cpp_std=c++17'])

llvm_config = find_program('llvm-config', required : true)

link_args = run_command(
  llvm_config,
  '--cxxflags',
  '--ldflags',
  '--system-libs',
  '--libs',
  'core').stdout().strip().split()

incdir = include_directories(
  'src/include',
  run_command(
    llvm_config,
    '--includedir').stdout().strip())

ntokenize = subproject('ntokenize')
ntokenize_dep = ntokenize.get_variable('ntokenize_dep')

lib = static_library('nkc',
  'src/lib/tokenize/lex.cc',
  'src/lib/tokenize/main.cc',
  'src/lib/parse/binop.cc',
  'src/lib/parse/main.cc',
  'src/lib/codegen.cc',
  dependencies: ntokenize_dep,
  include_directories: incdir,
  install: false)

exe = executable('nkc',
  'src/main.cc',
  link_with: lib,
  link_args: link_args,
  dependencies: ntokenize_dep,
  include_directories: incdir,
  install : true)
