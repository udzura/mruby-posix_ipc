MRuby::Gem::Specification.new('mruby-posix_ipc') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Uchio Kondo'

  spec.linker.libraries << 'rt'

  spec.add_test_dependency 'mruby-print', core: 'mruby-print'
  spec.add_test_dependency 'mruby-process', mgem: 'mruby-process'
end
