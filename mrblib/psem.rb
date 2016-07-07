class PSem
  class << self
    def open(name, options={})
      if mode = options[:mode]
        if mode.is_a? String
          _mode = PMQ::O_CREAT
          (0...mode.length).each do |idx|
            case mode[idx]
            when "r"
              _mode |= PMQ::O_RDONLY
            when "w"
              _mode |= PMQ::O_WRONLY
            else
              raise "Invalid mode letter: #{mode[idx]}"
            end
          end
          mode = _mode
        end
      end
      mode ||= PMQ::O_CREAT|PMQ::O_RDWR
      value = options[:initvalue] || options[:value] || 0

      new(name, mode, value)
    end
    alias new_with_name open

    def new_without_name(options={})
      mode  = options[:shared] ? 1 : 0
      value = options[:initvalue] || options[:value] || 0

      new(nil, mode, value)
    end
  end

  def inspect
    "#<PSem name=#{self.name.inspect} value=#{self.value}>"
  end
end
