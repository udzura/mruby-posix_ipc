MRuby::Gem::Specification.new('mruby-posix_ipc') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Uchio Kondo'

  spec.linker.libraries << 'rt'
  # spec.cc.flags << '-lrt'
  # spec.linker.flags << '-lrt'
end
