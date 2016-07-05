MRuby::Gem::Specification.new('mruby-posixmq') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Uchio Kondo'

  spec.linker.flags << '-lrt'
end
