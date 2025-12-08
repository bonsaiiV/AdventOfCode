project('aoc', 'c',
  #default_options : ['warning_level=3', 'b_sanitize=address'])
  default_options : ['warning_level=3'])

include_dir = include_directories('include')
subdir('src')

cc = meson.get_compiler('c')
lib_m = cc.find_library('m', required : false)

exe = executable('aoc',
  ['main.c', sources],
  include_directories : [include_dir],
  dependencies : [lib_m],
  link_with : [lib_data])
