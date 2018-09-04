class Array
  include Enumerable
  
  def each
    i = 0
    while i < size
      yield self[i]
      i = i + 1
    end
  end
  
  def first
    self[0]
  end
  
  def join(sep="")
    s = ""
    each do |i|
      s << sep unless i == first
      s << i.to_s
    end
    s
  end
  
  def to_s
    join
  end
  
  def inspect
    str = map do |i|
      i.inspect
    end
    "[" + str.join(", ") + "]"
  end

  def +(other)
    result = []
    self.each do |x|
      result << x
    end
    other.each do |x|
      result << x
    end
    result
  end

  def -(other)
    skip_value = {}
    other.each do |x|
        skip_value[x] = true
    end
    result = []
    self.each do |x|
        result << x unless skip_value[x]
    end
    result
  end
end
