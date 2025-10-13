set pagination off

break range.c:29
break range.c:33

commands 1
  silent
  if i % 5 == 0
    echo @@@ start:
    print start
    echo @@@ end:
    print end  
    echo @@@ step:
    print step
    echo @@@ current:
    print i
  end
  continue
end

commands 2
  silent
  if i % 5 == 0
    echo @@@ start:
    print start
    echo @@@ end:
    print end  
    echo @@@ step:
    print step
    echo @@@ current:
    print i
  end
  continue
end

run 1 12 > /dev/null
quit