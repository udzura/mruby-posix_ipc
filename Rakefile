MRUBY_CONFIG=File.expand_path(ENV["MRUBY_CONFIG"]) rescue nil
MRUBY_VERSION=ENV["MRUBY_VERSION"] || "2.0.1"

file :mruby do
  cmd =  "git clone --depth=1 git://github.com/mruby/mruby.git"
  if MRUBY_VERSION != 'master'
    cmd << " && cd mruby"
    cmd << " && git fetch --tags && git checkout $(git rev-parse #{MRUBY_VERSION})"
  end
  sh cmd
end

desc "compile binary"
task :compile => :mruby do
  conf = MRUBY_CONFIG || File.expand_path("./build_config.rb")
  sh "cd mruby && MRUBY_CONFIG=#{conf} rake all"
end

desc "test"
task :test => :mruby do
  conf = MRUBY_CONFIG || File.expand_path("./.travis_build_config.rb")
  sh "cd mruby && MRUBY_CONFIG=#{conf} rake all test"
end

desc "cleanup"
task :clean do
  exit 0 unless File.directory?('mruby')
  sh "cd mruby && rake deep_clean"
end

task :default => :compile
