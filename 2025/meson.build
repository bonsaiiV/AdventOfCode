project('aoc', 'c',
  #default_options : ['warning_level=3', 'b_sanitize=address'])
  default_options : ['warning_level=3'])

include_dir = include_directories('include')
subdir('src')
exe = executable('aoc',
  ['main.c', sources],
  include_directories : [include_dir],
  link_with : [lib_data])
