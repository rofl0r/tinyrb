if fork
    sleep 2
    puts "in parent"
else
    puts "in child"
end
puts "exit"
# => in child
# => exit
# => in parent
# => exit
