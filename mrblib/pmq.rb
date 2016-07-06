class PMQ
  def self.create(name, options={})
    queue_size = options[:queue_size] || 1
    message_size = options[:message_size] || -1
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

    new(name, mode, queue_size, message_size)
  end

  alias size maxmsg

  def inspect
    "#<PMQ name=#{self.name} size=#{self.size}>"
  end
end
