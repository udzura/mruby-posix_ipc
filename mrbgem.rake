MRuby::Gem::Specification.new('mruby-posix_ipc') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Uchio Kondo'

  spec.linker.flags << '-lrt'
end
