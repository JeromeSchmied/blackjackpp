project('blackjackpp-client', 'cpp',
  version : '0.1',
  default_options : [
    'cpp_std=c++20',
    'warning_level=3',
    # 'werror=true',
    'optimization=g',
    'debug=true',
    'b_ndebug=if-release'
  ]
)

fs = import('fs')

# include_dir = include_directories('include')
crow_dir = meson.project_source_root() / 'external' / 'Crow'

git = find_program('git', required: true)

if not fs.exists(crow_dir)
  message('Cloning CrowCpp at master...')
  run_command(git, 'clone', 'https://github.com/CrowCpp/Crow', '--depth=1', crow_dir, check: true)
else
  message('CrowCpp exists!')
endif

message('CrowCpp cloned to: ' + crow_dir)

main_target_name = 'blackjackpp-client'
sources = ['src/main.cpp', '../card.cpp', '../dealer.cpp', '../deck.cpp', '../game.cpp', '../human.cpp', '../player.cpp', '../print.cpp', '../statistics.cpp']
hdrs = include_directories('./headers', '../headers', '../../external/cpptui', './external/Crow/include')

executable(main_target_name, sources, include_directories: hdrs, install: true)
