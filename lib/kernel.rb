module Kernel
    def puts(*args)
        out = []
        args.each do |x|
            out << x.to_s
            out << "\n"
        end
        $stdout.syswrite(out.join)
    end
end
