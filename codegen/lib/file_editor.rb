def insert_target_line(target_file, target_line, original_line)
  lines = File.readlines(target_file)
  index = lines.index(target_line)
  if !index.nil?
      puts "Line is already present, file: #{target_file}  line: #{target_line}"
      return true
  end
  index = lines.index(original_line)
  if index.nil?
      puts "WARNING: Could not find line! file: #{target_file}  line: #{original_line}"
      return false
  end
  lines.insert(index, target_line)
  File.open(target_file, "w+") do |f|
      f.puts(lines)
  end
  puts "Updated file: #{target_file}  new line: #{target_line}"
  return true
end
