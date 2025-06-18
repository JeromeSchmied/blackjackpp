project('blackjackpp', 'cpp',
  version: '0.1',
  meson_version: '>=1.1.0',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
    # 'werror=true',
    'optimization=g',
    'debug=true',
    'b_ndebug=if-release'
  ]
)

# dep: [name, github_repo, include_dir]
gh_deps = {
  'cpptui': ['csboo/cpptui', 'cfa92dfdf69c6218f9cf95dac70cf8201a350184', '.'],
  'nlohmann_json': ['nlohmann/json', 'b451735fe7bb3283336f934b3f6cd3f484f73649', 'single_include'],
}

build_client = get_option('client')
build_server = get_option('server')
system_deps = get_option('system-deps')

if build_client
  gh_deps += {'cpp-httplib': ['yhirose/cpp-httplib', '3a1f379e751ef6555f06c5c1ef367a6fce26722c', '.']}
  message('building the client')
endif

if build_server
  gh_deps += {'crow': ['crowcpp/crow', 'fc3a02307d2ae387561e4d43fe58b782cb63de2d', 'include']}
  message('building the server')
endif

fs = import('fs')
git = find_program('git', required: true)
external_dir = '.' / 'external'

dep_map = {}

foreach name, details : gh_deps
  repo = details[0]
  commit = details[1]
  inc_subdir = details[2]

  sys_dep = dependency(name, required: false)
  if system_deps and sys_dep.found()
    message(name + ' found on system.')
    dep_map += {name: sys_dep}
  else
    dep_dir = external_dir / name
    if not fs.exists(dep_dir)
      message(name + ' not found, cloning from https://github.com/' + repo + ' @' + commit)
      run_command(git, 'clone', '--revision=' + commit, '--depth=1', 'https://github.com/' + repo, dep_dir, check: true)
    else
      message(name + ' already cloned to: ' + dep_dir)
    endif
    inc = include_directories(dep_dir / inc_subdir)
    dep_map += {name: declare_dependency(include_directories: inc)}
  endif
endforeach


lib_includes = include_directories('src/lib')
libblackjackpp = declare_dependency(include_directories: lib_includes, dependencies: [dep_map['nlohmann_json'], dep_map['cpptui']])

# these guys need the dependencies ready
if build_client
  ws2_32 = []
  if host_machine.system() == 'windows'
    cc = meson.get_compiler('cpp')
    ws2_32 = cc.find_library('ws2_32', required: true)
  endif
  executable('blackjackpp-client', ['src/client/main.cpp'], dependencies: [libblackjackpp, ws2_32, dep_map['cpp-httplib']], install: true)
endif
if build_server
  asio_dep = dependency('asio', required: true)

  sources = ['src/server/main.cpp']
  executable('blackjackpp-server', sources,
    dependencies : [asio_dep, libblackjackpp, dep_map['crow']],
    install : true
  )
endif
