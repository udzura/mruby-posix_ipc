begin

sem = PSem.open('/job_by_3', value: 3)

pids = (0..10).map do |i|
  Process.fork do
    puts "Waiting job: #{i}"
    sem.wait
    puts "Enter job: #{i}"
    sleep rand(3)
    puts "Done: #{i}"
    puts "(Now sem.value = #{sem.value})"
    sem.post
  end
end

pids.each do |pid|
  Process.waitpid pid
end

puts "Exit all"

ensure

sem.unlink

end
