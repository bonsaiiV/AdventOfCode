project('aoc', 'c',
  default_options : ['warning_level=3'])

include_dir = include_directories('include')
subdir('src')
exe = executable('aoc', ['main.c', sources], include_directories : [include_dir])
