file = "/tmp/io-test.txt"
fd = IO.sysopen(file, "w")
IO.new(fd).syswrite("this is a test")
fd = IO.sysopen(file, "r")
puts IO.new(fd).sysread(20)
# => this is a test
