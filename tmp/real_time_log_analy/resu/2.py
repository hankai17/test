data = struct.pack('=H2512s',3,'ed2k://|file|[%E9%A2%86%E5%9C%9F%E4%B9%8B%E6%88%983-%E8%A7%89%E9%86%92].Dominions.3.keygen.html|5995|D554ED2FF0E30F9F7F32E3E0B1C68843|h=H4UAIIYODRDE5ZF3OPQC4GWKKVQOLVFP|/')
    #data = struct.pack('H512s',2,'ed2k://|file|[哈哈%E4%B9%8B%E6%88%983-%E8%A7%89%E9%86%92].Dominions.3.keygen.html|5995|D554ED2FF0E30F9F7F32E3E0B1C68843|h=H4UAIIYODRDE5ZF3OPQC4GWKKVQOLVFP|/')
    #data = struct.pack('HHHHH40s20s',0x8,0,socket.ntohs(1),0,25600,'0123456789012345678901234567890123456789','cdddd')
    #data = struct.pack('HHHHH40s20s',0x8,0,socket.ntohs(1),0,256,'0123456789012345678901234567890123456789','cdddd')

    s.sendto(data,(host,port))
    #message, address = s.recvfrom(8192)
    print "Looking for replies; press Ctrl-C or Ctrl-Break to stop."
    #sys.stdin.readline().strip()
    #print "\n"
    time.sleep(1)
    

