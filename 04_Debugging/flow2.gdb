set pagination off

set $counter = 0
break range.c:29

commands
  silent
  set $counter = $counter + 1
  if $counter >= 28 && $counter <= 35
    echo @@@ start:
    print start
    echo @@@ end:
    print end  
    echo @@@ step:
    print step
    echo @@@ current:
    print i
  end
  if $counter < 35
    continue
  end
end

run -100 100 3 > /dev/null
quit