MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gembox 'default'
  conf.gem mgem: 'mruby-process'
  conf.gem '../mruby-posix_ipc'
end
