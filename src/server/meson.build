project('blackjackpp-server', 'cpp',
  version : '0.1.0',
  default_options : [
    'cpp_std=c++20',
    'warning_level=3',
    'optimization=g',
    'debug=true',
    'b_ndebug=if-release'
  ]
)


crow_dep = dependency('crow', required: true)
asio_dep = dependency('asio', required: true)

sources = ['src/main.cpp', '../card.cpp', '../dealer.cpp', '../deck.cpp', '../game.cpp', '../human.cpp', '../player.cpp', '../print.cpp', '../statistics.cpp']
hdrs = include_directories('./headers', '../headers', '../../external/cpptui', './external/Crow/include')

proj_name = 'blackjackpp-server'
executable(proj_name, sources,
  dependencies : [crow_dep, asio_dep],
  include_directories: hdrs,
  install : true
)
